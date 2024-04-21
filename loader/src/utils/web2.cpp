#include <Geode/cocos/platform/IncludeCurl.h>
#include <Geode/utils/web2.hpp>
#include <Geode/utils/map.hpp>
#include <sstream>

using namespace geode::prelude;
using namespace geode::utils::web;

class WebResponse::Impl {
public:
    int m_code;
    ByteVector m_data;
    std::unordered_map<std::string, std::string> m_headers;
};

WebResponse::WebResponse() : m_impl(std::make_shared<Impl>()) {}

bool WebResponse::ok() const {
    return 200 <= m_impl->m_code && m_impl->m_code < 300;
}
int WebResponse::code() const {
    return m_impl->m_code;
}

Result<std::string> WebResponse::string() const {
    return Ok(std::string(m_impl->m_data.begin(), m_impl->m_data.end()));
}
Result<matjson::Value> WebResponse::json() const {
    GEODE_UNWRAP_INTO(auto value, this->string());
    std::string error;
    auto res = matjson::parse(value, error);
    if (error.size() > 0) {
        return Err("Error parsing JSON: " + error);
    }
    return Ok(res.value());
}
ByteVector WebResponse::data() const {
    return m_impl->m_data;
}

std::vector<std::string> WebResponse::headers() const {
    return map::keys(m_impl->m_headers);
}

std::optional<std::string> WebResponse::header(std::string_view name) const {
    auto str = std::string(name);
    if (m_impl->m_headers.contains(str)) {
        return m_impl->m_headers.at(str);
    }
    return std::nullopt;
}

class WebProgress::Impl {
public:
    size_t m_downloadCurrent;
    size_t m_downloadTotal;
    size_t m_uploadCurrent;
    size_t m_uploadTotal;
};

WebProgress::WebProgress() : m_impl(std::make_shared<Impl>()) {}

size_t WebProgress::downloaded() const {
    return m_impl->m_downloadCurrent;
}
size_t WebProgress::downloadTotal() const {
    return m_impl->m_downloadTotal;
}
std::optional<float> WebProgress::downloadProgress() const {
    return downloadTotal() > 0 ? std::optional(downloaded() * 100.f / downloadTotal()) : std::nullopt;
}

size_t WebProgress::uploaded() const {
    return m_impl->m_uploadCurrent;
}
size_t WebProgress::uploadTotal() const {
    return m_impl->m_uploadTotal;
}
std::optional<float> WebProgress::uploadProgress() const {
    return uploadTotal() > 0 ? std::optional(uploaded() * 100.f / uploadTotal()) : std::nullopt;
}

class WebRequest::Impl {
public:
    std::string m_method;
    std::string m_url;
    std::unordered_map<std::string, std::string> m_headers;
    std::unordered_map<std::string, std::string> m_urlParameters;
    std::optional<std::string> m_userAgent;
    std::optional<ByteVector> m_body;
    std::optional<std::chrono::seconds> m_timeout;

    WebResponse makeError(int code, std::string const& msg) {
        auto res = WebResponse();
        res.m_impl->m_code = code;
        res.m_impl->m_data = toByteArray(msg);
        return res;
    }
};

WebRequest::WebRequest() : m_impl(std::make_shared<Impl>()) {}
WebRequest::~WebRequest() {}

// Encodes a url param
std::string urlParamEncode(std::string_view const input) {
    std::ostringstream ss;
    ss << std::hex << std::uppercase;
    for (char c : input) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            ss << c;
        } else {
            ss << '%' << static_cast<int>(c);
        }
    }
    return ss.str();
}

WebTask WebRequest::send2(std::string_view method, std::string_view url) {
    m_impl->m_method = method;
    m_impl->m_url = url;
    return WebTask::run([impl = m_impl](auto progress, auto hasBeenCancelled) -> WebTask::Result {
        // Init Curl
        auto curl = curl_easy_init();
        if (!curl) {
            return impl->makeError(-1, "Curl not initialized");
        }

        // todo: in the future, we might want to support downloading directly into 
        // files / in-memory streams like the old AsyncWebRequest class

        // Struct that holds values for the curl callbacks
        struct ResponseData {
            WebResponse response;
            Impl* impl;
            WebTask::PostProgress progress;
            WebTask::HasBeenCancelled hasBeenCancelled;
        } responseData = {
            .response = WebResponse(),
            .impl = impl.get(),
            .progress = progress,
            .hasBeenCancelled = hasBeenCancelled,
        };

        // Store downloaded response data into a byte vector
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char* data, size_t size, size_t nmemb, void* ptr) {
            auto& target = static_cast<ResponseData*>(ptr)->response.m_impl->m_data;
            target.insert(target.end(), data, data + size * nmemb);
            return size * nmemb;
        });

        // Set headers
        curl_slist* headers = nullptr;
        for (auto& [name, value] : impl->m_headers) {
            // Sanitize header name
            auto header = name;
            header.erase(std::remove_if(header.begin(), header.end(), [](char c) {
                return c == '\r' || c == '\n';
            }), header.end());
            // Append value
            header += ": " + value;
            headers = curl_slist_append(headers, header.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Add parameters to the URL and pass it to curl
        auto url = impl->m_url;
        bool first = true;
        for (auto param : impl->m_urlParameters) {
            url += (first ? "?" : "&") + urlParamEncode(param.first) + "=" + urlParamEncode(param.second);
            first = false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set request method
        if (impl->m_method != "GET") {
            if (impl->m_method == "POST") {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
            }
            else {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, impl->m_method.c_str());
            }
        }

        // Set body if provided
        if (impl->m_body) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, impl->m_body->data());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, impl->m_body->size());
        }
        
        // No need to verify SSL, we trust our domains :-)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

        // Set user agent if provided
        if (impl->m_userAgent) {
            curl_easy_setopt(curl, CURLOPT_USERAGENT, impl->m_userAgent->c_str());
        }

        // Set timeout
        if (impl->m_timeout) {
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, impl->m_timeout->count());
        }

        // Track progress
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

        // Follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

        // Do not fail if response code is 4XX or 5XX
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L);

        // Get headers from the response
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseData);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, (+[](char* buffer, size_t size, size_t nitems, void* ptr) {
            auto& headers = static_cast<ResponseData*>(ptr)->response.m_impl->m_headers;
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
                headers.insert_or_assign(key, value);
            }
            return size * nitems;
        }));

        // Track & post progress on the Promise
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &responseData);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, +[](void* ptr, double dtotal, double dnow, double utotal, double unow) -> int {
            auto data = static_cast<ResponseData*>(ptr);

            // Check for cancellation and abort if so
            if (data->hasBeenCancelled()) {
                return 1;
            }

            // Post progress to Promise listener
            auto progress = WebProgress();
            progress.m_impl->m_downloadTotal = dtotal;
            progress.m_impl->m_downloadCurrent = dnow;
            progress.m_impl->m_uploadTotal = utotal;
            progress.m_impl->m_uploadCurrent = unow;
            data->progress(std::move(progress));

            // Continue as normal
            return 0;
        });

        // Make the actual web request
        auto curlResponse = curl_easy_perform(curl);

        // Get the response code; note that this will be invalid if the 
        // curlResponse is not CURLE_OK
        long code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
        responseData.response.m_impl->m_code = static_cast<int>(code);

        // Free up curl memory
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        // Check if the request failed on curl's side or because of cancellation
        if (curlResponse != CURLE_OK) {
            if (hasBeenCancelled()) {
                return WebTask::Cancel();
            }
            else {
                return impl->makeError(-1, "Curl failed: " + std::string(curl_easy_strerror(curlResponse)));
            }
        }
        
        // Check if the response was an error code
        if (code >= 400 && code <= 600) {
            return std::move(responseData.response);
        }

        // Otherwise resolve with success :-)
        return std::move(responseData.response);
    });
}

WebPromise WebRequest::send(std::string_view method, std::string_view url) {
    m_impl->m_method = method;
    m_impl->m_url = url;
    return WebPromise([impl = m_impl](auto resolve, auto reject, auto progress, auto const& cancelled) {
        // Init Curl
        auto curl = curl_easy_init();
        if (!curl) {
            reject(impl->makeError(-1, "Curl not initialized"));
            return;
        }

        // todo: in the future, we might want to support downloading directly into 
        // files / in-memory streams like the old AsyncWebRequest class

        // Struct that holds values for the curl callbacks
        struct ResponseData {
            WebResponse response;
            Impl* impl;
            WebPromise::OnProgress progress;
            std::atomic_bool const& cancelled;
        } responseData = {
            .response = WebResponse(),
            .impl = impl.get(),
            .progress = progress,
            .cancelled = cancelled,
        };

        // Store downloaded response data into a byte vector
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char* data, size_t size, size_t nmemb, void* ptr) {
            auto& target = static_cast<ResponseData*>(ptr)->response.m_impl->m_data;
            target.insert(target.end(), data, data + size * nmemb);
            return size * nmemb;
        });

        // Set headers
        curl_slist* headers = nullptr;
        for (auto& [name, value] : impl->m_headers) {
            // Sanitize header name
            auto header = name;
            header.erase(std::remove_if(header.begin(), header.end(), [](char c) {
                return c == '\r' || c == '\n';
            }), header.end());
            // Append value
            header += ": " + value;
            headers = curl_slist_append(headers, header.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Add parameters to the URL and pass it to curl
        auto url = impl->m_url;
        bool first = true;
        for (auto param : impl->m_urlParameters) {
            url += (first ? "?" : "&") + urlParamEncode(param.first) + "=" + urlParamEncode(param.second);
            first = false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set request method
        if (impl->m_method != "GET") {
            if (impl->m_method == "POST") {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
            }
            else {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, impl->m_method.c_str());
            }
        }

        // Set body if provided
        if (impl->m_body) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, impl->m_body->data());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, impl->m_body->size());
        }
        
        // No need to verify SSL, we trust our domains :-)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

        // Set user agent if provided
        if (impl->m_userAgent) {
            curl_easy_setopt(curl, CURLOPT_USERAGENT, impl->m_userAgent->c_str());
        }

        // Set timeout
        if (impl->m_timeout) {
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, impl->m_timeout->count());
        }

        // Track progress
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

        // Follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

        // Do not fail if response code is 4XX or 5XX
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L);

        // Get headers from the response
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseData);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, (+[](char* buffer, size_t size, size_t nitems, void* ptr) {
            auto& headers = static_cast<ResponseData*>(ptr)->response.m_impl->m_headers;
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
                headers.insert_or_assign(key, value);
            }
            return size * nitems;
        }));

        // Track & post progress on the Promise
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &responseData);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, +[](void* ptr, double dtotal, double dnow, double utotal, double unow) -> int {
            auto data = static_cast<ResponseData*>(ptr);

            // Check for cancellation and abort if so
            if (data->cancelled) {
                return 1;
            }

            // Post progress to Promise listener
            auto progress = WebProgress();
            progress.m_impl->m_downloadTotal = dtotal;
            progress.m_impl->m_downloadCurrent = dnow;
            progress.m_impl->m_uploadTotal = utotal;
            progress.m_impl->m_uploadCurrent = unow;
            data->progress(std::move(progress));

            // Continue as normal
            return 0;
        });

        // Make the actual web request
        auto curlResponse = curl_easy_perform(curl);

        // Get the response code; note that this will be invalid if the 
        // curlResponse is not CURLE_OK
        long code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
        responseData.response.m_impl->m_code = static_cast<int>(code);

        // Free up curl memory
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        // Check if the request failed on curl's side or because of cancellation
        if (curlResponse != CURLE_OK) {
            if (cancelled) {
                reject(impl->makeError(-1, "Request cancelled"));
            }
            else {
                reject(impl->makeError(-1, "Curl failed: " + std::string(curl_easy_strerror(curlResponse))));
            }
            return;
        }
        
        // Check if the response was an error code
        if (code >= 400 && code <= 600) {
            reject(std::move(responseData.response));
            return;
        }

        // Otherwise resolve with success :-)
        resolve(std::move(responseData.response));
    });
}
WebPromise WebRequest::post(std::string_view url) {
    return this->send("POST", url);
}
WebPromise WebRequest::get(std::string_view url) {
    return this->send("GET", url);
}
WebPromise WebRequest::put(std::string_view url) {
    return this->send("PUT", url);
}
WebPromise WebRequest::patch(std::string_view url) {
    return this->send("PATCH", url);
}

WebRequest& WebRequest::header(std::string_view name, std::string_view value) {
    m_impl->m_headers.insert_or_assign(std::string(name), std::string(value));
    return *this;
}
WebRequest& WebRequest::param(std::string_view name, std::string_view value) {
    m_impl->m_urlParameters.insert_or_assign(std::string(name), std::string(value));
    return *this;
}
WebRequest& WebRequest::userAgent(std::string_view name) {
    m_impl->m_userAgent = name;
    return *this;
}

WebRequest& WebRequest::timeout(std::chrono::seconds time) {
    m_impl->m_timeout = time;
    return *this;
}

WebRequest& WebRequest::body(ByteVector raw) {
    m_impl->m_body = raw;
    return *this;
}
WebRequest& WebRequest::bodyString(std::string_view str) {
    m_impl->m_body = toByteArray(str);
    return *this;
}
WebRequest& WebRequest::bodyJSON(matjson::Value const& json) {
    this->header("Content-Type", "application/json");
    m_impl->m_body = toByteArray(json);
    return *this;
}
