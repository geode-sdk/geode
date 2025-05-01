#include <crashlog.hpp>

#include <Geode/utils/string.hpp>
#include <array>
#include <thread>
#include <filesystem>
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <algorithm>

#include <mach-o/dyld_images.h>
#include <mach-o/dyld.h>
#define CommentType CommentTypeDummy
#import <Foundation/Foundation.h>
#undef CommentType

using namespace geode::prelude;

// https://gist.github.com/jvranish/4441299

static constexpr size_t FRAME_SIZE = 64;
static std::mutex s_mutex;
static std::condition_variable s_cv;
static int s_signal = 0;
static siginfo_t* s_siginfo = nullptr;
static ucontext_t* s_context = nullptr;
static size_t s_backtraceSize = 0;
static std::array<void*, FRAME_SIZE> s_backtrace;
static int s_crashIter = 0;

static std::string_view getSignalCodeString() {
    switch(s_signal) {
        case SIGSEGV: return "SIGSEGV: Segmentation Fault";
        case SIGINT: return "SIGINT: Interactive attention signal, (usually ctrl+c)";
        case SIGFPE:
            switch(s_siginfo->si_code) {
                case FPE_INTDIV: return "SIGFPE: (integer divide by zero)";
                case FPE_INTOVF: return "SIGFPE: (integer overflow)";
                case FPE_FLTDIV: return "SIGFPE: (floating-point divide by zero)";
                case FPE_FLTOVF: return "SIGFPE: (floating-point overflow)";
                case FPE_FLTUND: return "SIGFPE: (floating-point underflow)";
                case FPE_FLTRES: return "SIGFPE: (floating-point inexact result)";
                case FPE_FLTINV: return "SIGFPE: (floating-point invalid operation)";
                case FPE_FLTSUB: return "SIGFPE: (subscript out of range)";
                default: return "SIGFPE: Arithmetic Exception";
            }
        case SIGILL:
            switch(s_siginfo->si_code) {
                case ILL_ILLOPC: return "SIGILL: (illegal opcode)";
                case ILL_ILLOPN: return "SIGILL: (illegal operand)";
                case ILL_ILLADR: return "SIGILL: (illegal addressing mode)";
                case ILL_ILLTRP: return "SIGILL: (illegal trap)";
                case ILL_PRVOPC: return "SIGILL: (privileged opcode)";
                case ILL_PRVREG: return "SIGILL: (privileged register)";
                case ILL_COPROC: return "SIGILL: (coprocessor error)";
                case ILL_BADSTK: return "SIGILL: (internal stack error)";
                default: return "SIGILL: Illegal Instruction";
            }
        case SIGTERM: return "SIGTERM: a termination request was sent to the program";
        case SIGABRT: return "SIGABRT: usually caused by an abort() or assert()";
        case SIGBUS: return "SIGBUS: Bus error (bad memory access)";
        default: return "Unknown signal code";
    }
}

static std::string getImageName(struct dyld_image_info const* image) {
    if (image == nullptr) {
        return "<Unknown>";
    }
    std::string imageName = image->imageFilePath;
    if (imageName.empty()) {
        imageName = "<Unknown>";
    }
    return imageName;
}

// https://stackoverflow.com/questions/28846503/getting-sizeofimage-and-entrypoint-of-dylib-module
size_t getImageSize(struct mach_header_64 const* header) {
    if (header == nullptr) {
        return 0;
    }
    size_t sz = 0;

    auto lc = (struct load_command const*) (header + 1);
    for (uint32_t i = 0; i < header->ncmds; i++) {
        auto seg = (struct segment_command_64 const*) lc;
        if (lc->cmd == LC_SEGMENT_64 && strcmp(seg->segname, SEG_PAGEZERO) != 0) {
            sz += seg->vmsize; // Size of segments
        }
        lc = (struct load_command const*) ((char *) lc + lc->cmdsize);
    }
    return sz;
}

static std::vector<struct dyld_image_info const*> getAllImages() {
    std::vector<struct dyld_image_info const*> images;
    struct task_dyld_info dyldInfo;
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    if (task_info(mach_task_self(), TASK_DYLD_INFO, (task_info_t)&dyldInfo, &count) == KERN_SUCCESS) {
        struct dyld_all_image_infos* imageInfos = (struct dyld_all_image_infos*)dyldInfo.all_image_info_addr;

        for (size_t i = 0; i < imageInfos->infoArrayCount; ++i) {
            images.push_back(&imageInfos->infoArray[i]);
        }
    }

    return images;
}

static struct dyld_image_info const* imageFromAddress(void const* addr) {
    if (addr == nullptr) {
        return nullptr;
    }

    auto loadedImages = getAllImages();
    std::sort(loadedImages.begin(), loadedImages.end(), [](auto const a, auto const b) {
        return (uintptr_t)a->imageLoadAddress < (uintptr_t)b->imageLoadAddress;
    });
    auto iter = std::upper_bound(loadedImages.begin(), loadedImages.end(), addr, [](auto const addr, auto const image) {
        return (uintptr_t)addr < (uintptr_t)image->imageLoadAddress;
    });

    if (iter == loadedImages.begin()) {
        return nullptr;
    }
    --iter;

    auto image = *iter;
    auto imageSize = getImageSize((struct mach_header_64 const*)image->imageLoadAddress);
    auto imageAddress = (uintptr_t)image->imageLoadAddress;
    if ((uintptr_t)addr >= imageAddress && (uintptr_t)addr < imageAddress + imageSize) {
        return image;
    }
    return nullptr;
}

static Mod* modFromAddress(void const* addr) {
    if (addr == nullptr) {
        return nullptr;
    }
    auto image = imageFromAddress(addr);
    if (image == nullptr) {
        return nullptr;
    }

    std::filesystem::path imagePath = getImageName(image);
    if (!std::filesystem::exists(imagePath)) {
        return nullptr;
    }
    auto geodePath = dirs::getGameDir() / "Frameworks" / "Geode.dylib";
    if (std::filesystem::equivalent(imagePath, geodePath)) {
        return Mod::get();
    }

    for (auto& mod : Loader::get()->getAllMods()) {
        if (!mod->isEnabled() || !std::filesystem::exists(mod->getBinaryPath())) {
            continue;
        }
        if (std::filesystem::equivalent(imagePath, mod->getBinaryPath())) {
            return mod;
        }
    }
    return nullptr;
}

static std::string getInfo(void* address, Mod* faultyMod) {
    std::stringstream stream;
    stream << "Faulty Lib: " << getImageName(imageFromAddress(address)) << "\n";
    stream << "Faulty Mod: " << (faultyMod ? faultyMod->getID() : "<Unknown>") << "\n";
    stream << "Instruction Address: " << address << "\n";
    stream << "Signal Code: " << std::hex << s_signal << " (" << getSignalCodeString() << ")" << std::dec << "\n";
    return stream.str();
}

extern "C" void signalHandler(int signal, siginfo_t* signalInfo, void* vcontext) {
    if (s_crashIter > 3) {
        // the crashlog crashed too much so maybe we shouldn't try
        std::abort();
    }
    s_crashIter++;

	auto context = reinterpret_cast<ucontext_t*>(vcontext);
	s_backtraceSize = backtrace(s_backtrace.data(), FRAME_SIZE);

    	// for some reason this is needed, dont ask me why
	#ifdef GEODE_IS_INTEL_MAC
	s_backtrace[2] = reinterpret_cast<void*>(context->uc_mcontext->__ss.__rip);
	#else
	s_backtrace[2] = reinterpret_cast<void*>(context->uc_mcontext->__ss.__pc);
	#endif
	if (s_backtraceSize < FRAME_SIZE) {
		s_backtrace[s_backtraceSize] = nullptr;
	}

    {
        std::unique_lock<std::mutex> lock(s_mutex);
        s_signal = signal;
        s_siginfo = signalInfo;
        s_context = context;
    }

    s_cv.notify_all();
    std::unique_lock<std::mutex> lock(s_mutex);
    s_cv.wait(lock, [] { return s_signal == 0; });

    s_crashIter--;
	std::_Exit(EXIT_FAILURE);
}

// https://stackoverflow.com/questions/8278691/how-to-fix-backtrace-line-number-error-in-c
std::string executeCommand(std::string const& cmd) {
    std::stringstream stream;
    std::array<char, 1024> buf;

    if (FILE* ptr = popen(cmd.c_str(), "r")) {
        while (fgets( buf.data(), buf.size(), ptr ) != NULL) {
            stream << buf.data();
        }
        pclose(ptr);
    }

    return stream.str();
}

std::string addr2Line() {
    std::stringstream stream;
    stream << "atos -p " << getpid() << " ";
    for (int i = 1; i < s_backtraceSize; ++i) {
        stream << s_backtrace[i] << " ";
    }
    // std::cout << "command: " << stream.str() << std::endl;
    return executeCommand(stream.str());
}

static std::string getStacktrace() {
    std::stringstream stacktrace;

    auto messages = backtrace_symbols(s_backtrace.data(), s_backtraceSize);
	if (s_backtraceSize < FRAME_SIZE) {
		messages[s_backtraceSize] = nullptr;
	}

    std::stringstream lines(addr2Line());

    for (int i = 1; i < s_backtraceSize; ++i) {
        auto message = std::string(messages[i]);

        auto stream = std::stringstream(message);
        int index;
        std::string binary;
        uintptr_t address;
        std::string function;
        uintptr_t offset;
        std::string line;

        stream >> index;

        if (!lines.eof()) {
            std::getline(lines, line);
        }
        std::getline(stream, binary);
        auto cutoff = binary.find("0x");
        stream = std::stringstream(binary.substr(cutoff));
        binary = geode::utils::string::trim(binary.substr(0, cutoff));
        stream >> std::hex >> address >> std::dec;

        if (!line.empty()) {
            // log::debug("address: {}", address);
            auto image = imageFromAddress(reinterpret_cast<void*>(address));
            // log::debug("image: {}", image);
            stacktrace << " - " << std::showbase << std::hex;

            if (image) {
                auto baseAddress = image->imageLoadAddress;
                auto imageName = getImageName(image);
                stacktrace << imageName << " + " << (address - (uintptr_t)baseAddress);   
            }
            else {
                stacktrace << address;
            }
            stacktrace << std::dec;
            stacktrace << ": " << line << "\n";
        }
        else {
            std::getline(stream, function);
            cutoff = function.find("+");
            stream = std::stringstream(function.substr(cutoff));
            stream >> offset;
            function = geode::utils::string::trim(function.substr(0, cutoff));

            {
                int status;
                auto demangle = abi::__cxa_demangle(function.c_str(), 0, 0, &status);
                if (status == 0) {
                    function = demangle;
                }
                free(demangle);
            }
            
            stacktrace << "- " << binary;
            stacktrace << " @ " << std::showbase << std::hex << address << std::dec;
            stacktrace << " (" << function << " + " << offset << ")\n";
        }
    }

    free(messages);

    return stacktrace.str();
}

static std::string getRegisters() {
    std::stringstream registers;

    auto context = s_context;
    auto& ss = context->uc_mcontext->__ss;

    // geez
    registers << std::showbase << std::hex /*<< std::setfill('0') << std::setw(16) */;
    #ifdef GEODE_IS_INTEL_MAC
    registers << "rax: " << ss.__rax << "\n";
    registers << "rbx: " << ss.__rbx << "\n";
    registers << "rcx: " << ss.__rcx << "\n";
    registers << "rdx: " << ss.__rdx << "\n";
    registers << "rdi: " << ss.__rdi << "\n";
    registers << "rsi: " << ss.__rsi << "\n";
    registers << "rbp: " << ss.__rbp << "\n";
    registers << "rsp: " << ss.__rsp << "\n";
    registers << "r8: " << ss.__r8 << "\n";
    registers << "r9: " << ss.__r9 << "\n";
    registers << "r10: " << ss.__r10 << "\n";
    registers << "r11: " << ss.__r11 << "\n";
    registers << "r12: " << ss.__r12 << "\n";
    registers << "r13: " << ss.__r13 << "\n";
    registers << "r14: " << ss.__r14 << "\n";
    registers << "r15: " << ss.__r15 << "\n";
    registers << "rip: " << ss.__rip << "\n";
    registers << "rflags: " << ss.__rflags << "\n";
    registers << "cs: " << ss.__cs << "\n";
    registers << "fs: " << ss.__fs << "\n";
    registers << "gs: " << ss.__gs << "\n";
    #else // m1
    registers << "x0: " << ss.__x[0] << "\n";
    registers << "x1: " << ss.__x[1] << "\n";
    registers << "x2: " << ss.__x[2] << "\n";
    registers << "x3: " << ss.__x[3] << "\n";
    registers << "x4: " << ss.__x[4] << "\n";
    registers << "x5: " << ss.__x[5] << "\n";
    registers << "x6: " << ss.__x[6] << "\n";
    registers << "x7: " << ss.__x[7] << "\n";
    registers << "x8: " << ss.__x[8] << "\n";
    registers << "x9: " << ss.__x[9] << "\n";
    registers << "x10: " << ss.__x[10] << "\n";
    registers << "x11: " << ss.__x[11] << "\n";
    registers << "x12: " << ss.__x[12] << "\n";
    registers << "x13: " << ss.__x[13] << "\n";
    registers << "x14: " << ss.__x[14] << "\n";
    registers << "x15: " << ss.__x[15] << "\n";
    registers << "x16: " << ss.__x[16] << "\n";
    registers << "x17: " << ss.__x[17] << "\n";
    registers << "x18: " << ss.__x[18] << "\n";
    registers << "x19: " << ss.__x[19] << "\n";
    registers << "x20: " << ss.__x[20] << "\n";
    registers << "x21: " << ss.__x[21] << "\n";
    registers << "x22: " << ss.__x[22] << "\n";
    registers << "x23: " << ss.__x[23] << "\n";
    registers << "x24: " << ss.__x[24] << "\n";
    registers << "x25: " << ss.__x[25] << "\n";
    registers << "x26: " << ss.__x[26] << "\n";
    registers << "x27: " << ss.__x[27] << "\n";
    registers << "x28: " << ss.__x[28] << "\n";
    registers << "fp: " << ss.__fp << "\n";
    registers << "lr: " << ss.__lr << "\n";
    registers << "sp: " << ss.__sp << "\n";
    registers << "pc: " << ss.__pc << "\n";
    registers << "cpsr: " << ss.__cpsr << "\n";
    #endif

    return registers.str();
}

static void handlerThread() {
    std::unique_lock<std::mutex> lock(s_mutex);
    s_cv.wait(lock, [] { return s_signal != 0; });

    #ifdef GEODE_IS_INTEL_MAC
    auto signalAddress = reinterpret_cast<void*>(s_context->uc_mcontext->__ss.__rip);
    #else // m1
    auto signalAddress = reinterpret_cast<void*>(s_context->uc_mcontext->__ss.__pc);
    #endif
    // Mod* faultyMod = nullptr;
    // for (int i = 1; i < s_backtraceSize; ++i) {
    //     auto mod = modFromAddress(s_backtrace[i]);
    //     if (mod != nullptr) {
    //         faultyMod = mod;
    //         break;
    //     }
    // }
    Mod* faultyMod = modFromAddress(signalAddress);

    auto text = crashlog::writeCrashlog(faultyMod, getInfo(signalAddress, faultyMod), getStacktrace(), getRegisters());

    log::error("Geode crashed!\n{}", text);
    
    s_signal = 0;
    s_cv.notify_all();

    log::debug("Notified");
}

static bool s_lastLaunchCrashed;

bool crashlog::setupPlatformHandler() {
    struct sigaction action;
    action.sa_sigaction = &signalHandler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);
    sigaction(SIGSEGV, &action, nullptr);
    // I'd rather not track interrupt lol
    // sigaction(SIGINT, &action, nullptr);
    sigaction(SIGFPE, &action, nullptr);
    sigaction(SIGILL, &action, nullptr);
    sigaction(SIGTERM, &action, nullptr);
    sigaction(SIGABRT, &action, nullptr);
    sigaction(SIGBUS, &action, nullptr);

    std::thread(&handlerThread).detach();
	
    auto lastCrashedFile = crashlog::getCrashLogDirectory() / "last-crashed";
    if (std::filesystem::exists(lastCrashedFile)) {
        s_lastLaunchCrashed = true;
        try {
            std::filesystem::remove(lastCrashedFile);
        }
        catch (...) {
        }
    }
    return true;
}

void crashlog::setupPlatformHandlerPost() {}

bool crashlog::didLastLaunchCrash() {
    return s_lastLaunchCrashed;
}

std::filesystem::path crashlog::getCrashLogDirectory() {
    return dirs::getGeodeDir() / "crashlogs";
}
