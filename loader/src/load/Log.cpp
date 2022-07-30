#include <Geode/Geode.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/utils/casts.hpp>
#include <InternalLoader.hpp>
#include <iomanip>

std::ostream& operator<<(std::ostream& os, Mod* mod) {
    if (mod) {
        os << "{ Mod, " + std::string(mod->getName()) + " }";
    } else {
        os << "{ Mod, null }";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, cocos2d::CCObject* obj) {
    if (obj) {
        os << "{ " + std::string(typeid(*obj).name()) + ", " + utils::intToHex(obj)  + " }";
    } else {
        os << "{ CCObject, null }";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, cocos2d::CCArray* arr) {
    os << "[";
    if (arr && arr->count() > 0) {
    	auto last = arr->objectAtIndex(arr->count()-1);
	    for (auto obj : ccArrayToVector<cocos2d::CCObject*>(arr)) {
	    	os << obj;
	    	if (obj != last) os << ", ";
	    }
    }
    else os << "empty";
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, cocos2d::CCPoint const& pos) {
    os << pos.x << ", " << pos.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, cocos2d::CCSize const& size) {
    os << size.width << " : " << size.height;
    return os;
}

std::ostream& operator<<(std::ostream& os, cocos2d::CCRect const& rect) {
    os << rect.origin.x << ", " << rect.origin.y << " | " << rect.size.width << " : " << rect.size.height;
    return os;
}

std::ostream& operator<<(std::ostream& os, cocos2d::ccColor3B const& color) {
    os << color.r << ", " << color.g << ", " << color.b;
    return os;
}

std::ostream& operator<<(std::ostream& os, cocos2d::ccColor4B const& color) {
    os << color.r << ", " << color.g << ", " << color.b << ", " << color.a;
    return os;
}

USE_GEODE_NAMESPACE();


log_clock::time_point LogPtr::getTime() const {
    return m_time;
}

std::string LogPtr::getTimeString() const {
    return timePointAsString(m_time);
}

Mod* LogPtr::getSender() const {
    return m_sender;
}

Severity LogPtr::getSeverity() const {
    return m_severity;
}

LogPtr::~LogPtr() {}

std::string LogPtr::toString(bool logTime) const {
    std::stringstream res;

    if (logTime) {
        const auto t = std::chrono::system_clock::to_time_t(this->m_time);
        tm obj;
        #ifdef _MSC_VER
        localtime_s(&obj, &t);
        #else
        obj = *std::localtime(&t);
        #endif
        res << '[' << std::put_time(&obj, "%H:%M:%S") << "] ";
    }
    res << '[';
    if (this->m_sender) {
        res << this->m_sender->getName();
    } else {
        res << '?';
    }
    res << "]:";

    for (LogMetadata* i : this->m_data) {
        res << ' ' << i->m_repr;
    }

    return res.str();
}

std::vector<LogMetadata*> const& LogPtr::getData() const {
    return m_data;
}

std::string geode::generateLogName() {
    std::stringstream tmp;
    tmp << "Geode_" 
        << std::chrono::duration_cast<std::chrono::seconds>(log_clock::now().time_since_epoch()).count()
        << ".log";
    return tmp.str();
}

void Log::flush() {
    this->m_logptr->m_data.push_back(new LogMetadata(this->m_stream.str()));
    Loader::get()->pushLog(this->m_logptr);

    // Loader manages this memory now
    this->m_logptr = nullptr;
    this->m_stream.str("");
}

Log::~Log() {
    this->flush();
    #ifdef GEODE_PLATFORM_CONSOLE
    std::cout << std::endl;
    #endif
}

Log& Log::operator<<(Severity::type s) {
    this->m_logptr->m_severity = s;
    return *this;
}

Log& Log::operator<<(Severity s) {
    this->m_logptr->m_severity = s;
    return *this;
}

Log& Log::operator<<(ostream_fn_type t) {
    if (t == reinterpret_cast<ostream_fn_type>(&std::endl<char, std::char_traits<char>>)) {
        LogPtr* newlog = new LogPtr(*this->m_logptr);
        this->flush();
        this->m_logptr = newlog;
    } else {
        this->m_stream << t;
    }

    return *this;
}

Log& operator<<(Log& l, Mod* m) {
    return l.streamMeta<ModMeta>(m);
}
Log& operator<<(Log& l, cocos2d::CCObject* o) {
    return l.streamMeta<CCObjectMeta>(o);
}
Log& operator<<(Log& l, cocos2d::CCArray* a) {
    return l.streamMeta<CCArrayMeta>(a);
}

CCObjectMeta::CCObjectMeta(cocos2d::CCObject* obj) : LogMetadata("") {
    CC_SAFE_RETAIN(obj);
    m_obj = obj;
}

CCObjectMeta::CCObjectMeta(std::string const& r, cocos2d::CCObject* obj) : LogMetadata(r) {
    CC_SAFE_RETAIN(obj);
    m_obj = obj;
}

CCObjectMeta::~CCObjectMeta() {
    CC_SAFE_RELEASE(m_obj);
} 

CCArrayMeta::CCArrayMeta(cocos2d::CCArray* arr) : LogMetadata("") {
    CC_SAFE_RETAIN(arr);
    m_arr = arr;
}

CCArrayMeta::CCArrayMeta(std::string const& r, cocos2d::CCArray* arr) : LogMetadata(r) {
    CC_SAFE_RETAIN(arr);
    m_arr = arr;
}

CCArrayMeta::~CCArrayMeta() {
    CC_SAFE_RELEASE(m_arr);
} 


