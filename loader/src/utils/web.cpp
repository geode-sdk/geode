#include <Geode/cocos/platform/IncludeCurl.h>
#include <Geode/loader/Loader.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/web.hpp>
#include <json.hpp>
#include <thread>

USE_GEODE_NAMESPACE();
using namespace web;

namespace geode::utils::fetch {
    static size_t writeBytes(char* data, size_t size, size_t nmemb, void* str) {
        as<ByteVector*>(str)->insert(as<ByteVector*>(str)->end(), data, data + size * nmemb);
        return size * nmemb;
    }

    static size_t writeString(char* data, size_t size, size_t nmemb, void* str) {
        as<std::string*>(str)->append(data, size * nmemb);
        return size * nmemb;
    }

    static size_t writeBinaryData(char* data, size_t size, size_t nmemb, void* file) {
        as<std::ostream*>(file)->write(data, size * nmemb);
        return size * nmemb;
    }

    static int progress(void* ptr, double total, double now, double, double) {
        return (*as<web::FileProgressCallback*>(ptr))(now, total) != true;
    }
}

Result<> web::fetchFile(
    std::string const& url, ghc::filesystem::path const& into, FileProgressCallback prog
) {
    auto curl = curl_easy_init();

    if (!curl) return Err("Curl not initialized!");

    std::ofstream file(into, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        return Err("Unable to open output file");
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::fetch::writeBinaryData);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");
    if (prog) {
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, utils::fetch::progress);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &prog);
    }
    auto res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return Err("Fetch failed: " + std::string(curl_easy_strerror(res)));
    }

    char* ct;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
    if ((res == CURLE_OK) && ct) {
        curl_easy_cleanup(curl);
        return Ok();
    }
    curl_easy_cleanup(curl);
    return Err("Error getting info: " + std::string(curl_easy_strerror(res)));
}

Result<ByteVector> web::fetchBytes(std::string const& url) {
    auto curl = curl_easy_init();

    if (!curl) return Err("Curl not initialized!");

    ByteVector ret;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::fetch::writeBytes);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");
    auto res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return Err("Fetch failed");
    }

    char* ct;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
    if ((res == CURLE_OK) && ct) {
        curl_easy_cleanup(curl);
        return Ok(ret);
    }
    curl_easy_cleanup(curl);
    return Err("Error getting info: " + std::string(curl_easy_strerror(res)));
}

Result<json::Value> web::fetchJSON(std::string const& url) {
    std::string res;
    GEODE_UNWRAP_INTO(res, fetch(url));
    try {
        return Ok(json::parse(res));
    }
    catch (std::exception& e) {
        return Err(e.what());
    }
}

Result<std::string> web::fetch(std::string const& url) {
    auto curl = curl_easy_init();

    if (!curl) return Err("Curl not initialized!");

    std::string ret;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::fetch::writeString);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");
    auto res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return Err("Fetch failed");
    }

    char* ct;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
    if ((res == CURLE_OK) && ct) {
        curl_easy_cleanup(curl);
        return Ok(ret);
    }
    curl_easy_cleanup(curl);
    return Err("Error getting info: " + std::string(curl_easy_strerror(res)));
}

class SentAsyncWebRequest::Impl {
private:
    enum class Status {
        Paused,
        Running,
        Finished,
        Cancelled,
        CleanedUp,
    };
    std::string m_id;
    std::string m_url;
    std::vector<AsyncThen> m_thens;
    std::vector<AsyncExpectCode> m_expects;
    std::vector<AsyncProgress> m_progresses;
    std::vector<AsyncCancelled> m_cancelleds;
    Status m_status = Status::Paused;
    std::atomic<bool> m_paused = true;
    std::atomic<bool> m_cancelled = false;
    std::atomic<bool> m_finished = false;
    std::atomic<bool> m_cleanedUp = false;
    std::condition_variable m_statusCV;
    std::mutex m_statusMutex;
    SentAsyncWebRequest* m_self;

    mutable std::mutex m_mutex;
    std::variant<std::monostate, std::ostream*, ghc::filesystem::path> m_target =
        std::monostate();
    std::vector<std::string> m_httpHeaders;

    template <class T>
    friend class AsyncWebResult;
    friend class AsyncWebRequest;

    void pause();
    void resume();
    void error(std::string const& error, int code);
    void doCancel();

public:
    Impl(SentAsyncWebRequest* self, AsyncWebRequest const&, std::string const& id);
    void cancel();
    bool finished() const;

    friend class SentAsyncWebRequest;
};

static std::unordered_map<std::string, SentAsyncWebRequestHandle> RUNNING_REQUESTS{};
static std::mutex RUNNING_REQUESTS_MUTEX;

SentAsyncWebRequest::Impl::Impl(SentAsyncWebRequest* self, AsyncWebRequest const& req, std::string const& id) :
    m_id(id), m_url(req.m_url), m_target(req.m_target), m_httpHeaders(req.m_httpHeaders) {

#define AWAIT_RESUME()    \
    {\
        auto lock = std::unique_lock(m_statusMutex);\
        m_statusCV.wait(lock, [this]() { \
            return !m_paused; \
        });\
        if (m_cancelled) {\
            this->doCancel();\
            return;\
        }\
    }\

    if (req.m_then) m_thens.push_back(req.m_then);
    if (req.m_progress) m_progresses.push_back(req.m_progress);
    if (req.m_cancelled) m_cancelleds.push_back(req.m_cancelled);
    if (req.m_expect) m_expects.push_back(req.m_expect);

    std::thread([this]() {
        AWAIT_RESUME();

        auto curl = curl_easy_init();
        if (!curl) {
            return this->error("Curl not initialized", -1);
        }

        // resulting byte array
        ByteVector ret;
        // output file if downloading to file. unique_ptr because not always
        // initialized but don't wanna manually managed memory
        std::unique_ptr<std::ofstream> file = nullptr;

        // into file
        if (std::holds_alternative<ghc::filesystem::path>(m_target)) {
            file = std::make_unique<std::ofstream>(
                std::get<ghc::filesystem::path>(m_target), std::ios::out | std::ios::binary
            );
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file.get());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::fetch::writeBinaryData);
        }
        // into stream
        else if (std::holds_alternative<std::ostream*>(m_target)) {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, std::get<std::ostream*>(m_target));
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::fetch::writeBinaryData);
        }
        // into memory
        else {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::fetch::writeBytes);
        }
        curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
        // No need to verify SSL, we trust our domains :-)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        // Github User Agent
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");
        // Track progress
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        // Follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        // Fail if response code is 4XX or 5XX
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

        curl_slist* headers = nullptr;
        for (auto& header : m_httpHeaders) {
            headers = curl_slist_append(headers, header.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        struct ProgressData {
            SentAsyncWebRequest::Impl* self;
            std::ofstream* file;
        } data{this, file.get()};

        curl_easy_setopt(
            curl,
            CURLOPT_PROGRESSFUNCTION,
            +[](void* ptr, double total, double now, double, double) -> int {
                auto data = static_cast<ProgressData*>(ptr);
                auto lock = std::unique_lock(data->self->m_statusMutex);
                data->self->m_statusCV.wait(lock, [data]() { 
                    return !data->self->m_paused; 
                });
                if (data->self->m_cancelled) {
                    if (data->file) {
                        data->file->close();
                    }
                    return 1;
                }
                Loader::get()->queueInGDThread([self = data->self, now, total]() {
                    std::lock_guard _(self->m_mutex);
                    for (auto& prog : self->m_progresses) {
                        prog(*self->m_self, now, total);
                    }
                });
                return 0;
            }
        );
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &data);
        auto res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            long code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
            curl_easy_cleanup(curl);
            return this->error("Fetch failed: " + std::string(curl_easy_strerror(res)), code);
        }
        curl_easy_cleanup(curl);

        AWAIT_RESUME();

        // if something is still holding a handle to this
        // request, then they may still cancel it
        m_finished = true;

        Loader::get()->queueInGDThread([this, ret]() {
            std::lock_guard _(m_mutex);
            for (auto& then : m_thens) {
                then(*m_self, ret);
            }
            std::lock_guard __(RUNNING_REQUESTS_MUTEX);
            RUNNING_REQUESTS.erase(m_id);
        });
    }).detach();
}

void SentAsyncWebRequest::Impl::doCancel() {
    if (m_cleanedUp) return;
    m_cleanedUp = true;

    // remove file if downloaded to one
    if (std::holds_alternative<ghc::filesystem::path>(m_target)) {
        auto path = std::get<ghc::filesystem::path>(m_target);
        if (ghc::filesystem::exists(path)) {
            try {
                ghc::filesystem::remove(path);
            }
            catch (...) {
            }
        }
    }

    Loader::get()->queueInGDThread([this]() {
        std::lock_guard _(m_mutex);
        for (auto& canc : m_cancelleds) {
            canc(*m_self);
        }
    });

    this->error("Request cancelled", -1);
}

void SentAsyncWebRequest::Impl::cancel() {
    m_cancelled = true;
    // if already finished, cancel anyway to clean up
    if (m_finished) {
        this->doCancel();
    }
}

void SentAsyncWebRequest::Impl::pause() {
    m_paused = true;
    m_statusCV.notify_all();
}

void SentAsyncWebRequest::Impl::resume() {
    m_paused = false;
    m_statusCV.notify_all();
}

bool SentAsyncWebRequest::Impl::finished() const {
    return m_finished;
}

void SentAsyncWebRequest::Impl::error(std::string const& error, int code) {
    auto lock = std::unique_lock(m_statusMutex);
    m_statusCV.wait(lock, [this]() { 
        return !m_paused; 
    });
    Loader::get()->queueInGDThread([this, error, code]() {
        {
            std::lock_guard _(m_mutex);
            for (auto& expect : m_expects) {
                expect(error, code);
            }
        }
        std::lock_guard _(RUNNING_REQUESTS_MUTEX);
        RUNNING_REQUESTS.erase(m_id);
    });
}

SentAsyncWebRequest::SentAsyncWebRequest() : m_impl() {}
SentAsyncWebRequest::~SentAsyncWebRequest() {}

std::shared_ptr<SentAsyncWebRequest> SentAsyncWebRequest::create(AsyncWebRequest const& request, std::string const& id) {
    auto ret = std::make_shared<SentAsyncWebRequest>();
    ret->m_impl = std::move(std::make_shared<SentAsyncWebRequest::Impl>(ret.get(), request, id));
    return ret;
}
void SentAsyncWebRequest::doCancel() {
    return m_impl->doCancel();
}

void SentAsyncWebRequest::cancel() {
    return m_impl->cancel();
}

void SentAsyncWebRequest::pause() {
    return m_impl->pause();
}

void SentAsyncWebRequest::resume() {
    return m_impl->resume();
}

bool SentAsyncWebRequest::finished() const {
    return m_impl->finished();
}

void SentAsyncWebRequest::error(std::string const& error, int code) {
    return m_impl->error(error, code);
}

AsyncWebRequest& AsyncWebRequest::join(std::string const& requestID) {
    m_joinID = requestID;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::header(std::string const& header) {
    m_httpHeaders.push_back(header);
    return *this;
}

AsyncWebResponse AsyncWebRequest::fetch(std::string const& url) {
    m_url = url;
    return AsyncWebResponse(*this);
}

AsyncWebRequest& AsyncWebRequest::expect(AsyncExpect handler) {
    m_expect = [handler](std::string const& info, auto) {
        return handler(info);
    };
    return *this;
}

AsyncWebRequest& AsyncWebRequest::expect(AsyncExpectCode handler) {
    m_expect = handler;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::progress(AsyncProgress progress) {
    m_progress = progress;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::cancelled(AsyncCancelled cancelledFunc) {
    m_cancelled = cancelledFunc;
    return *this;
}

SentAsyncWebRequestHandle AsyncWebRequest::send() {
    if (m_sent) return nullptr;
    m_sent = true;

    std::lock_guard __(RUNNING_REQUESTS_MUTEX);

    // pause all running requests
    for (auto& [_, req] : RUNNING_REQUESTS) {
        req->pause();
    }

    SentAsyncWebRequestHandle ret;

    static size_t COUNTER = 0;
    if (m_joinID && RUNNING_REQUESTS.count(m_joinID.value())) {
        auto& req = RUNNING_REQUESTS.at(m_joinID.value());
        std::lock_guard _(req->m_impl->m_mutex);
        if (m_then) req->m_impl->m_thens.push_back(m_then);
        if (m_progress) req->m_impl->m_progresses.push_back(m_progress);
        if (m_expect) req->m_impl->m_expects.push_back(m_expect);
        if (m_cancelled) req->m_impl->m_cancelleds.push_back(m_cancelled);
        ret = req;
    }
    else {
        auto id = m_joinID.value_or("__anon_request_" + std::to_string(COUNTER++));
        ret = SentAsyncWebRequest::create(*this, id);
        RUNNING_REQUESTS.insert({id, ret});
    }

    // resume all running requests
    for (auto& [_, req] : RUNNING_REQUESTS) {
        req->resume();
    }

    return ret;
}

AsyncWebRequest::~AsyncWebRequest() {
    this->send();
}

AsyncWebResult<std::monostate> AsyncWebResponse::into(std::ostream& stream) {
    m_request.m_target = &stream;
    return this->as(+[](ByteVector const&) -> Result<std::monostate> {
        return Ok(std::monostate());
    });
}

AsyncWebResult<std::monostate> AsyncWebResponse::into(ghc::filesystem::path const& path) {
    m_request.m_target = path;
    return this->as(+[](ByteVector const&) -> Result<std::monostate> {
        return Ok(std::monostate());
    });
}

AsyncWebResult<std::string> AsyncWebResponse::text() {
    return this->as(+[](ByteVector const& bytes) -> Result<std::string> {
        return Ok(std::string(bytes.begin(), bytes.end()));
    });
}

AsyncWebResult<ByteVector> AsyncWebResponse::bytes() {
    return this->as(+[](ByteVector const& bytes) -> Result<ByteVector> {
        return Ok(bytes);
    });
}

AsyncWebResult<json::Value> AsyncWebResponse::json() {
    return this->as(+[](ByteVector const& bytes) -> Result<json::Value> {
        try {
            return Ok(json::parse(std::string(bytes.begin(), bytes.end())));
        }
        catch (std::exception& e) {
            return Err(std::string(e.what()));
        }
    });
}
