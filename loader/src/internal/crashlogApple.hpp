#include "crashlogUnix.hpp"
#include <Geode/utils/ZStringView.hpp>

#include <mach-o/dyld_images.h>
#include <mach-o/dyld.h>

using namespace geode::prelude;
using namespace crashlog;

static constexpr size_t FRAME_SIZE = 128;
static int s_signal = 0;
static siginfo_t* s_siginfo = nullptr;
static ucontext_t* s_context = nullptr;
static size_t s_backtraceSize = 0;
static std::array<void*, FRAME_SIZE> s_backtrace;

static CrashContext g_context;

// https://stackoverflow.com/questions/28846503/getting-sizeofimage-and-entrypoint-of-dylib-module
static size_t getImageSize(struct mach_header_64 const* header) {
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

std::vector<Image> CrashContext::getImages() {
    std::vector<Image> images;
    struct task_dyld_info dyldInfo;
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    if (task_info(mach_task_self(), TASK_DYLD_INFO, (task_info_t)&dyldInfo, &count) == KERN_SUCCESS) {
        struct dyld_all_image_infos* imageInfos = (struct dyld_all_image_infos*)dyldInfo.all_image_info_addr;

        for (size_t i = 0; i < imageInfos->infoArrayCount; ++i) {
            auto img = imageInfos->infoArray[i];
            images.push_back({
                (uintptr_t)img.imageLoadAddress,
                img.imageFilePath,
                getImageSize((struct mach_header_64 const*)img.imageLoadAddress)
            });
        }
    }

    return images;
}

std::vector<Register> CrashContext::getRegisters() {
    auto& ss = s_context->uc_mcontext->__ss;
    std::vector<Register> registers;

    // geez
#ifdef GEODE_IS_INTEL_MAC
    registers.push_back({ "RAX", ss.__rax });
    registers.push_back({ "RBX", ss.__rbx });
    registers.push_back({ "RCX", ss.__rcx });
    registers.push_back({ "RDX", ss.__rdx });
    registers.push_back({ "RBP", ss.__rbp });
    registers.push_back({ "RSP", ss.__rsp });
    registers.push_back({ "RDI", ss.__rdi });
    registers.push_back({ "RSI", ss.__rsi });
    registers.push_back({ "RIP", ss.__rip });
    registers.push_back({ "R8",  ss.__r8  });
    registers.push_back({ "R9",  ss.__r9  });
    registers.push_back({ "R10", ss.__r10 });
    registers.push_back({ "R11", ss.__r11 });
    registers.push_back({ "R12", ss.__r12 });
    registers.push_back({ "R13", ss.__r13 });
    registers.push_back({ "R14", ss.__r14 });
    registers.push_back({ "R15", ss.__r15 });
    registers.push_back({ "RFLAGS", ss.__rflags });
    registers.push_back({ "CS", ss.__cs });
    registers.push_back({ "FS", ss.__fs });
    registers.push_back({ "GS", ss.__gs });
#else // m1 / ios
    registers.push_back({ "x0", ss.__x[0] });
    registers.push_back({ "x1", ss.__x[1] });
    registers.push_back({ "x2", ss.__x[2] });
    registers.push_back({ "x3", ss.__x[3] });
    registers.push_back({ "x4", ss.__x[4] });
    registers.push_back({ "x5", ss.__x[5] });
    registers.push_back({ "x6", ss.__x[6] });
    registers.push_back({ "x7", ss.__x[7] });
    registers.push_back({ "x8", ss.__x[8] });
    registers.push_back({ "x9", ss.__x[9] });
    registers.push_back({ "x10", ss.__x[10] });
    registers.push_back({ "x11", ss.__x[11] });
    registers.push_back({ "x12", ss.__x[12] });
    registers.push_back({ "x13", ss.__x[13] });
    registers.push_back({ "x14", ss.__x[14] });
    registers.push_back({ "x15", ss.__x[15] });
    registers.push_back({ "x16", ss.__x[16] });
    registers.push_back({ "x17", ss.__x[17] });
    registers.push_back({ "x18", ss.__x[18] });
    registers.push_back({ "x19", ss.__x[19] });
    registers.push_back({ "x20", ss.__x[20] });
    registers.push_back({ "x21", ss.__x[21] });
    registers.push_back({ "x22", ss.__x[22] });
    registers.push_back({ "x23", ss.__x[23] });
    registers.push_back({ "x24", ss.__x[24] });
    registers.push_back({ "x25", ss.__x[25] });
    registers.push_back({ "x26", ss.__x[26] });
    registers.push_back({ "x27", ss.__x[27] });
    registers.push_back({ "x28", ss.__x[28] });
    registers.push_back({ "fp", ss.__fp});
    registers.push_back({ "lr", ss.__lr });
    registers.push_back({ "sp", ss.__sp });
    registers.push_back({ "pc", ss.__pc });
    registers.push_back({ "cpsr", ss.__cpsr });
#endif

    return registers;
}

// https://stackoverflow.com/questions/8278691/how-to-fix-backtrace-line-number-error-in-c
static std::string executeCommand(ZStringView cmd) {
    StringBuffer<> stream;
    std::array<char, 1024> buf;

    if (FILE* ptr = popen(cmd.c_str(), "r")) {
        while (fgets( buf.data(), buf.size(), ptr ) != NULL) {
            stream.append("{}", buf.data());
        }
        pclose(ptr);
    }

    return stream.str();
}

static std::string addr2Line() {
    StringBuffer<> stream;
    stream.append("atos -p {} ", getpid());
    for (int i = 1; i < s_backtraceSize; ++i) {
        stream.append("{} ", s_backtrace[i]);
    }
    // std::cout << "command: " << stream.str() << std::endl;
    return executeCommand(stream.str());
}

static uint64_t decodeULEB128(const uint8_t*& p) {
    uint64_t result = 0;
    uint64_t shift = 0;
    do {
        result |= (uint64_t(*p & 0x7f) << shift);
        shift += 7;
    } while (*p++ & 0x80);
    return result;
}

static std::vector<uintptr_t> const& getFunctionStarts() {
    static std::vector<uintptr_t> funcs = []() {
        std::vector<uintptr_t> starts;

        auto image = g_context.imageFromAddress(reinterpret_cast<void*>(base::get()));
        if (!image) return starts;

        auto header = reinterpret_cast<const struct mach_header_64*>(image->address);
        uintptr_t linkeditAddr = 0;
        uintptr_t linkeditFileOff = 0;
        uintptr_t textVMAddr = 0;
        uint32_t dataOff = 0, dataSize = 0;

        auto cmd = reinterpret_cast<const struct load_command*>(header + 1);
        for (uint32_t i = 0; i < header->ncmds; i++) {
            if (cmd->cmd == LC_SEGMENT_64) {
                auto seg = reinterpret_cast<const struct segment_command_64*>(cmd);
                if (strcmp(seg->segname, SEG_LINKEDIT) == 0) {
                    linkeditAddr = seg->vmaddr;
                    linkeditFileOff = seg->fileoff;
                } else if (strcmp(seg->segname, SEG_TEXT) == 0) {
                    textVMAddr = seg->vmaddr;
                }
            } else if (cmd->cmd == LC_FUNCTION_STARTS) {
                auto ldc = reinterpret_cast<const struct linkedit_data_command*>(cmd);
                dataOff = ldc->dataoff;
                dataSize = ldc->datasize;
            }
            cmd = reinterpret_cast<const struct load_command*>((const char*)cmd + cmd->cmdsize);
        }

        if (!linkeditAddr || !dataOff) return starts;

        uintptr_t slide = reinterpret_cast<uintptr_t>(header) - textVMAddr;
        const uint8_t* p = reinterpret_cast<const uint8_t*>(linkeditAddr + slide + (dataOff - linkeditFileOff));
        const uint8_t* end = p + dataSize;

        uintptr_t currentOffset = 0;
        while (p < end && *p) {
            uint64_t delta = decodeULEB128(p);
            if (delta == 0) break;
            currentOffset += delta;
            starts.push_back(currentOffset);
        }

        return starts;
    }();
    return funcs;
}

