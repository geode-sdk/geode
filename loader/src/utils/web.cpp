#define CURL_STATICLIB
#include <curl/curl.h>

#include <Geode/loader/Loader.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/web.hpp>
#include <matjson.hpp>
#include <thread>

using namespace geode::prelude;
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
    std::string const& url, std::filesystem::path const& into, FileProgressCallback prog
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
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::fetch::writeBytes);
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

Result<matjson::Value> web::fetchJSON(std::string const& url) {
    std::string data;
    GEODE_UNWRAP_INTO(data, fetch(url));
    std::string error;
    auto res = matjson::parse(data, error);
    if (error.size() > 0) {
        return Err("Error parsing JSON: " + error);
    }
    return Ok(res.value());
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
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
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
    std::unordered_map<std::string, std::string> m_responseHeader;
    Status m_status = Status::Paused;
    std::atomic<bool> m_paused = true;
    std::atomic<bool> m_cancelled = false;
    std::atomic<bool> m_finished = false;
    std::atomic<bool> m_cleanedUp = false;
    std::condition_variable m_statusCV;
    std::mutex m_statusMutex;
    SentAsyncWebRequest* m_self;

    mutable std::mutex m_mutex;
    std::string m_userAgent;
    std::string m_customRequest;
    bool m_isPostRequest = false;
    std::string m_postFields;
    bool m_isJsonRequest = false;
    bool m_sent = false;
    std::variant<std::monostate, std::ostream*, std::filesystem::path> m_target;
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

    std::string getResponseHeader(std::string_view header) const {
        auto it = m_responseHeader.find(std::string(header));
        if (it == m_responseHeader.end()) return "";
        return it->second;
    }

    friend class SentAsyncWebRequest;
};

class AsyncWebRequest::Impl {
public:
    std::optional<std::string> m_joinID;
    std::string m_url;
    AsyncThen m_then = nullptr;
    AsyncExpectCode m_expect = nullptr;
    AsyncProgress m_progress = nullptr;
    AsyncCancelled m_cancelled = nullptr;
    std::string m_userAgent;
    std::string m_customRequest;
    bool m_isPostRequest = false;
    std::string m_postFields;
    bool m_isJsonRequest = false;
    bool m_sent = false;
    std::variant<std::monostate, std::ostream*, std::filesystem::path> m_target;
    std::vector<std::string> m_httpHeaders;
    std::chrono::seconds m_timeoutSeconds;

    SentAsyncWebRequestHandle send(AsyncWebRequest&);
};

static std::unordered_map<std::string, SentAsyncWebRequestHandle> RUNNING_REQUESTS{};
static std::mutex RUNNING_REQUESTS_MUTEX;

SentAsyncWebRequest::Impl::Impl(SentAsyncWebRequest* self, AsyncWebRequest const& req, std::string const& id) :
    m_self(self),
    m_id(id),
    m_url(req.m_impl->m_url),
    m_target(req.m_impl->m_target),
    m_userAgent(req.m_impl->m_userAgent),
    m_customRequest(req.m_impl->m_customRequest),
    m_isPostRequest(req.m_impl->m_isPostRequest),
    m_postFields(req.m_impl->m_postFields),
    m_isJsonRequest(req.m_impl->m_isJsonRequest),
    m_sent(req.m_impl->m_sent),
    m_httpHeaders(req.m_impl->m_httpHeaders) {

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

    if (req.m_impl->m_then) m_thens.push_back(req.m_impl->m_then);
    if (req.m_impl->m_progress) m_progresses.push_back(req.m_impl->m_progress);
    if (req.m_impl->m_cancelled) m_cancelleds.push_back(req.m_impl->m_cancelled);
    if (req.m_impl->m_expect) m_expects.push_back(req.m_impl->m_expect);

    auto timeoutSeconds = req.m_impl->m_timeoutSeconds;

    std::thread([this, timeoutSeconds]() {
        thread::setName("Curl Request");

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
        if (std::holds_alternative<std::filesystem::path>(m_target)) {
            file = std::make_unique<std::ofstream>(
                std::get<std::filesystem::path>(m_target), std::ios::out | std::ios::binary
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
        // User Agent
        curl_easy_setopt(curl, CURLOPT_USERAGENT, m_userAgent.c_str());

        // Headers
        curl_slist* headers = nullptr;
        for (auto& header : m_httpHeaders) {
            headers = curl_slist_append(headers, header.c_str());
        }

        // Post request
        if (m_isPostRequest || m_customRequest.size()) {
            if (m_isPostRequest) {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
            }
            else {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, m_customRequest.c_str());
            }
            if (m_isJsonRequest) {
                headers = curl_slist_append(headers, "Content-Type: application/json");
            }
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_postFields.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, m_postFields.size());
        }

        // Timeout
        if (timeoutSeconds.count()) {
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSeconds.count());
        }

        // Track progress
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        // Follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        // Fail if response code is 4XX or 5XX
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L); // we will handle http errors manually

        // Headers end
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        struct ProgressData {
            SentAsyncWebRequest::Impl* self;
            std::ofstream* file;
        } data{this, file.get()};

        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &data);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, (+[](char* buffer, size_t size, size_t nitems, void* ptr){
            auto data = static_cast<ProgressData*>(ptr);
            std::unordered_map<std::string, std::string> headers;
            std::string line;
            std::stringstream ss(std::string(buffer, size * nitems));
            while (std::getline(ss, line)) {
                auto colon = line.find(':');
                if (colon == std::string::npos) continue;
                auto key = line.substr(0, colon);
                auto value = line.substr(colon + 2);
                if (value.ends_with('\r')) {
                    value = value.substr(0, value.size() - 1);
                }
                data->self->m_responseHeader[key] = value;
            }
            return size * nitems;
        }));

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

                Loader::get()->queueInMainThread([self = data->self, now, total]() {
                    std::unique_lock<std::mutex> l(self->m_mutex);
                    for (auto& prog : self->m_progresses) {
                        l.unlock();
                        prog(*self->m_self, now, total);
                        l.lock();
                    }
                });
                return 0;
            }
        );
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &data);
        auto res = curl_easy_perform(curl);

        // free the header list
        curl_slist_free_all(headers);

        long code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            if (m_cancelled) {
                return this->doCancel();
            } else {
                return this->error("Fetch failed: " + std::string(curl_easy_strerror(res)), code);
            }
        }
        if (code >= 400 && code < 600) {
            std::string response_str(ret.begin(), ret.end());
            curl_easy_cleanup(curl);
            return this->error(response_str, code);
        }
        curl_easy_cleanup(curl);

        AWAIT_RESUME();

        // if something is still holding a handle to this
        // request, then they may still cancel it
        m_finished = true;

        Loader::get()->queueInMainThread([this, ret]() {
            std::unique_lock<std::mutex> l(m_mutex);
            for (auto& then : m_thens) {
                l.unlock();
                then(*m_self, ret);
                l.lock();
            }
            // Delay the destruction of SentAsyncWebRequest till the next frame
            // otherwise we'd have an use-after-free
            Loader::get()->queueInMainThread([m_id = m_id] {
                std::lock_guard __(RUNNING_REQUESTS_MUTEX);
                RUNNING_REQUESTS.erase(m_id);
            });
        });
    }).detach();
}

void SentAsyncWebRequest::Impl::doCancel() {
    if (m_cleanedUp) return;
    m_cleanedUp = true;

    // remove file if downloaded to one
    if (std::holds_alternative<std::filesystem::path>(m_target)) {
        auto path = std::get<std::filesystem::path>(m_target);
        if (std::filesystem::exists(path)) {
            std::error_code ec;
            std::filesystem::remove(path, ec);
        }
    }

    Loader::get()->queueInMainThread([this]() {
        std::unique_lock<std::mutex> l(m_mutex);
        for (auto& canc : m_cancelleds) {
            l.unlock();
            canc(*m_self);
            l.lock();
        }
    });
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
    Loader::get()->queueInMainThread([this, error, code]() {
        {
            std::unique_lock<std::mutex> l(m_mutex);
            for (auto& expect : m_expects) {
                l.unlock();
                expect(error, code);
                l.lock();
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
std::string SentAsyncWebRequest::getResponseHeader(std::string_view header) const {
    return m_impl->getResponseHeader(header);
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

AsyncWebRequest::AsyncWebRequest() {
    m_impl = std::make_unique<AsyncWebRequest::Impl>();
}

AsyncWebRequest::~AsyncWebRequest() {
    this->send();
}

AsyncWebRequest& AsyncWebRequest::setThen(AsyncThen then) {
    m_impl->m_then = then;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::join(std::string_view const requestID) {
    m_impl->m_joinID = requestID;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::userAgent(std::string_view const userAgent) {
    m_impl->m_userAgent = userAgent;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::contentType(std::string_view const contentType) {
    return this->header("Content-Type", contentType);
}

AsyncWebRequest& AsyncWebRequest::postRequest() {
    m_impl->m_isPostRequest = true;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::method(std::string_view const request) {
    m_impl->m_customRequest = request;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::bodyRaw(std::string_view const fields) {
    m_impl->m_postFields = fields;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::body(matjson::Value const& fields) {
    m_impl->m_isJsonRequest = true;
    return this->bodyRaw(fields.dump(matjson::NO_INDENTATION));
}

AsyncWebRequest& AsyncWebRequest::timeout(std::chrono::seconds seconds) {
    m_impl->m_timeoutSeconds = seconds;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::header(std::string_view const header) {
    std::string str(header);
    // remove \r and \n
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) {
        return c == '\r' || c == '\n';
    }), str.end());
    m_impl->m_httpHeaders.push_back(str);
    return *this;
}

AsyncWebRequest& AsyncWebRequest::header(std::string_view const headerName, std::string_view const headerValue) {
    return this->header(fmt::format("{}: {}", headerName, headerValue));
}

AsyncWebResponse AsyncWebRequest::get(std::string_view const url) {
    this->method("GET");
    return this->fetch(url);
}

AsyncWebResponse AsyncWebRequest::post(std::string_view const url) {
    this->method("POST");
    return this->fetch(url);
}

AsyncWebResponse AsyncWebRequest::put(std::string_view const url) {
    this->method("PUT");
    return this->fetch(url);
}

AsyncWebResponse AsyncWebRequest::patch(std::string_view const url) {
    this->method("PATCH");
    return this->fetch(url);
}

AsyncWebResponse AsyncWebRequest::fetch(std::string_view const url) {
    m_impl->m_url = url;
    return AsyncWebResponse(*this);
}

AsyncWebRequest& AsyncWebRequest::expect(AsyncExpect handler) {
    m_impl->m_expect = [handler](std::string const& info, auto) {
        return handler(info);
    };
    return *this;
}

AsyncWebRequest& AsyncWebRequest::expect(AsyncExpectCode handler) {
    m_impl->m_expect = handler;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::progress(AsyncProgress progress) {
    m_impl->m_progress = progress;
    return *this;
}

AsyncWebRequest& AsyncWebRequest::cancelled(AsyncCancelled cancelledFunc) {
    m_impl->m_cancelled = cancelledFunc;
    return *this;
}

SentAsyncWebRequestHandle AsyncWebRequest::send() {
    return m_impl->send(*this);
}

SentAsyncWebRequestHandle AsyncWebRequest::Impl::send(AsyncWebRequest& reqObj) {
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
        ret = SentAsyncWebRequest::create(reqObj, id);
        RUNNING_REQUESTS.insert({id, ret});
    }

    // resume all running requests
    for (auto& [_, req] : RUNNING_REQUESTS) {
        req->resume();
    }

    return ret;
}

AsyncWebResult<std::monostate> AsyncWebResponse::into(std::ostream& stream) {
    m_request.m_impl->m_target = &stream;
    return this->as(+[](ByteVector const&) -> Result<std::monostate> {
        return Ok(std::monostate());
    });
}

AsyncWebResult<std::monostate> AsyncWebResponse::into(std::filesystem::path const& path) {
    m_request.m_impl->m_target = path;
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

AsyncWebResult<matjson::Value> AsyncWebResponse::json() {
    return this->as(+[](ByteVector const& bytes) -> Result<matjson::Value> {
        std::string error;
        auto res = matjson::parse(std::string(bytes.begin(), bytes.end()), error);
        if (error.size() > 0) {
            return Err("Error parsing JSON: " + error);
        }
        return Ok(res.value());
    });
}
