#include <Geode/cocos/platform/IncludeCurl.h>
#include <Geode/loader/Loader.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/fetch.hpp>
#include <Geode/utils/vector.hpp>
#include <thread>

USE_GEODE_NAMESPACE();
using namespace web;

namespace geode::utils::fetch {
    static size_t writeBytes(char* data, size_t size, size_t nmemb, void* str) {
        as<byte_array*>(str)->insert(as<byte_array*>(str)->end(), data, data + size * nmemb);
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

Result<byte_array> web::fetchBytes(std::string const& url) {
    auto curl = curl_easy_init();

    if (!curl) return Err("Curl not initialized!");

    byte_array ret;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
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

Result<std::string> web::fetch(std::string const& url) {
    auto curl = curl_easy_init();

    if (!curl) return Err("Curl not initialized!");

    std::string ret;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
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

static std::unordered_map<std::string, SentAsyncWebRequestHandle> RUNNING_REQUESTS {};
static std::mutex RUNNING_REQUESTS_MUTEX;

SentAsyncWebRequest::SentAsyncWebRequest(AsyncWebRequest const& req, std::string const& id) :
    m_id(id), m_url(req.m_url), m_target(req.m_target), m_httpHeaders(req.m_httpHeaders) {
#define AWAIT_RESUME()    \
    while (m_paused) {}   \
    if (m_cancelled) {    \
        this->doCancel(); \
        return;           \
    }

    if (req.m_then) m_thens.push_back(req.m_then);
    if (req.m_progress) m_progresses.push_back(req.m_progress);
    if (req.m_cancelled) m_cancelleds.push_back(req.m_cancelled);
    if (req.m_expect) m_expects.push_back(req.m_expect);

    std::thread([this]() {
        AWAIT_RESUME();

        auto curl = curl_easy_init();
        if (!curl) {
            return this->error("Curl not initialized");
        }

        // resulting byte array
        byte_array ret;
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
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

        curl_slist* headers = nullptr;
        for (auto& header : m_httpHeaders) {
            headers = curl_slist_append(headers, header.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        struct ProgressData {
            SentAsyncWebRequest* self;
            std::ofstream* file;
        } data { this, file.get() };

        curl_easy_setopt(
            curl, CURLOPT_PROGRESSFUNCTION,
            +[](void* ptr, double total, double now, double, double) -> int {
                auto data = static_cast<ProgressData*>(ptr);
                while (data->self->m_paused) {}
                if (data->self->m_cancelled) {
                    if (data->file) {
                        data->file->close();
                    }
                    return 1;
                }
                Loader::get()->queueInGDThread([self = data->self, now, total]() {
                    std::lock_guard _(self->m_mutex);
                    for (auto& prog : self->m_progresses) {
                        prog(*self, now, total);
                    }
                });
                return 0;
            }
        );
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &data);
        auto res = curl_easy_perform(curl);

        char* ct;
        res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
        curl_easy_cleanup(curl);
        if ((res != CURLE_OK) || !ct) {
            return this->error("Fetch failed: " + std::string(curl_easy_strerror(res)));
        }

        AWAIT_RESUME();

        // if something is still holding a handle to this
        // request, then they may still cancel it
        m_finished = true;

        Loader::get()->queueInGDThread([this, ret]() {
            std::lock_guard _(m_mutex);
            for (auto& then : m_thens) {
                then(*this, ret);
            }
            std::lock_guard __(RUNNING_REQUESTS_MUTEX);
            RUNNING_REQUESTS.erase(m_id);
        });
    }).detach();
}

void SentAsyncWebRequest::doCancel() {
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
            canc(*this);
        }
    });

    this->error("Request cancelled");
}

void SentAsyncWebRequest::cancel() {
    m_cancelled = true;
    // if already finished, cancel anyway to clean up
    if (m_finished) {
        this->doCancel();
    }
}

void SentAsyncWebRequest::pause() {
    m_paused = true;
}

void SentAsyncWebRequest::resume() {
    m_paused = false;
}

bool SentAsyncWebRequest::finished() const {
    return m_finished;
}

void SentAsyncWebRequest::error(std::string const& error) {
    while (m_paused) {};
    Loader::get()->queueInGDThread([this, error]() {
        std::lock_guard _(m_mutex);
        for (auto& expect : m_expects) {
            expect(error);
        }
        std::lock_guard __(RUNNING_REQUESTS_MUTEX);
        RUNNING_REQUESTS.erase(m_id);
    });
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

AsyncWebRequest& AsyncWebRequest::expect(std::function<void(std::string const&)> handler) {
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
        std::lock_guard _(req->m_mutex);
        if (m_then) req->m_thens.push_back(m_then);
        if (m_progress) req->m_progresses.push_back(m_progress);
        if (m_expect) req->m_expects.push_back(m_expect);
        if (m_cancelled) req->m_cancelleds.push_back(m_cancelled);
        ret = req;
    }
    else {
        auto id = m_joinID.value_or("__anon_request_" + std::to_string(COUNTER++));
        ret = std::make_shared<SentAsyncWebRequest>(*this, id);
        RUNNING_REQUESTS.insert({ id, ret });
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
    return this->as(+[](byte_array const&) {
        return Ok(std::monostate());
    });
}

AsyncWebResult<std::monostate> AsyncWebResponse::into(ghc::filesystem::path const& path) {
    m_request.m_target = path;
    return this->as(+[](byte_array const&) {
        return Ok(std::monostate());
    });
}

AsyncWebResult<std::string> AsyncWebResponse::text() {
    return this->as(+[](byte_array const& bytes) {
        return Ok(std::string(bytes.begin(), bytes.end()));
    });
}

AsyncWebResult<byte_array> AsyncWebResponse::bytes() {
    return this->as(+[](byte_array const& bytes) {
        return Ok(bytes);
    });
}

AsyncWebResult<nlohmann::json> AsyncWebResponse::json() {
    return this->as(+[](byte_array const& bytes) -> Result<nlohmann::json> {
        try {
            return Ok(nlohmann::json::parse(bytes.begin(), bytes.end()));
        }
        catch (std::exception& e) {
            return Err(std::string(e.what()));
        }
    });
}
