#include <Geode/loader/Log.hpp>
#include <Geode/Result.hpp>
#include <Geode/utils/general.hpp>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <matjson.hpp>
#include <system_error>
#define CURL_STATICLIB
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/StringBuffer.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/terminate.hpp>
#include <Geode/utils/web.hpp>
#include <ca_bundle.h>
#include <curl/curl.h>
#include <queue>
#include <semaphore>
#include <sstream>

using namespace geode::prelude;
using namespace geode::utils::web;
using namespace geode::utils::string;

static long unwrapProxyType(ProxyType type) {
    switch (type) {
        using enum ProxyType;

        case HTTP:
            return CURLPROXY_HTTP;
        case HTTPS:
            return CURLPROXY_HTTPS;
        case HTTPS2:
            return CURLPROXY_HTTPS2;
        case SOCKS4:
            return CURLPROXY_SOCKS4;
        case SOCKS4A:
            return CURLPROXY_SOCKS4A;
        case SOCKS5:
            return CURLPROXY_SOCKS5;
        case SOCKS5H:
            return CURLPROXY_SOCKS5_HOSTNAME;
    }

    // Shouldn't happen.
    unreachable("Unexpected proxy type!");
}

static long unwrapHttpAuth(long auth) {
    long unwrapped = 0;

#define SET_AUTH(name) \
    if (auth & http_auth::name) \
        unwrapped |= CURLAUTH_##name;

    SET_AUTH(BASIC);
    SET_AUTH(DIGEST);
    SET_AUTH(DIGEST_IE);
    SET_AUTH(BEARER);
    SET_AUTH(NEGOTIATE);
    SET_AUTH(NTLM);
    SET_AUTH(NTLM_WB);
    SET_AUTH(ANY);
    SET_AUTH(ANYSAFE);
    SET_AUTH(ONLY);
    SET_AUTH(AWS_SIGV4);

#undef SET_AUTH

    return unwrapped;
}

static long unwrapHttpVersion(HttpVersion version)
{
    switch (version) {
        using enum HttpVersion;

        case DEFAULT:
            return CURL_HTTP_VERSION_NONE;
        case VERSION_1_0:
            return CURL_HTTP_VERSION_1_0;
        case VERSION_1_1:
            return CURL_HTTP_VERSION_1_1;
        case VERSION_2_0:
            return CURL_HTTP_VERSION_2_0;
        case VERSION_2TLS:
            return CURL_HTTP_VERSION_2TLS;
        case VERSION_2_PRIOR_KNOWLEDGE:
            return CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE;
        case VERSION_3:
            return CURL_HTTP_VERSION_3;
        case VERSION_3ONLY:
            return CURL_HTTP_VERSION_3ONLY;
    }

    // Shouldn't happen.
    unreachable("Unexpected HTTP Version!");
}

class WebResponse::Impl {
public:
    int m_code;
    ByteVector m_data;
    std::string m_errMessage;
    utils::StringMap<std::vector<std::string>> m_headers;

    Result<> into(std::filesystem::path const& path) const;
};

Result<> WebResponse::Impl::into(std::filesystem::path const& path) const {
    // Test if there are no permission issues
    std::error_code ec;
    auto _ = std::filesystem::exists(path, ec);
    if (ec) {
        return Err(fmt::format("Couldn't write to file: {}", ec.category().message(ec.value())));
    }

    auto stream = std::ofstream(path, std::ios::out | std::ios::binary);
    stream.write(reinterpret_cast<const char*>(m_data.data()), m_data.size());
    stream.close();

    return Ok();
}

struct MultipartFile {
    ByteVector data;
    std::string filename;
    std::string mime;
};

class MultipartForm::Impl {
public:
    std::unordered_map<std::string, std::string> m_params;
    std::unordered_map<std::string, MultipartFile> m_files;
    mutable std::string m_boundary;

    bool isBuilt() const {
        return !m_boundary.empty();
    }

    bool isBoundaryUnique(std::string_view boundary) const {
        // make sure params and files don't contain the boundary
        for (auto const& [name, value] : m_params) {
            if (name.find(boundary) != std::string::npos || value.find(boundary) != std::string::npos) {
                return false;
            }
        }

        // check files
        for (auto const& [name, file] : m_files) {
            if (name.find(boundary) != std::string::npos
                || file.filename.find(boundary) != std::string::npos
                || file.mime.find(boundary) != std::string::npos) {
                return false;
            }

            if (std::ranges::search(file.data, boundary).begin() != file.data.end()) {
                return false;
            }
        }

        return true;
    }

    void pickUniqueBoundary() const {
        if (isBuilt()) {
            return;
        }

        auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
        do {
            m_boundary = fmt::format("----GeodeWebBoundary{}", timestamp++);
        } while (!isBoundaryUnique(m_boundary));
    }

    ByteVector getBody() const {
        pickUniqueBoundary();

        ByteVector data;
        const auto addText = [&](std::string_view value) {
            data.insert(data.end(), value.begin(), value.end());
        };

        // add params
        for (auto const& [name, value] : m_params) {
            addText(fmt::format(
                "--{}\r\nContent-Disposition: form-data; name=\"{}\"\r\n\r\n{}\r\n",
                m_boundary, name, value
            ));
        }

        // add files
        for (auto const& [name, file] : m_files) {
            addText(fmt::format(
                "--{}\r\nContent-Disposition: form-data; name=\"{}\"; filename=\"{}\"\r\nContent-Type: {}\r\n\r\n",
                m_boundary, name, file.filename, file.mime
            ));
            data.insert(data.end(), file.data.begin(), file.data.end());
            addText("\r\n");
        }

        addText(fmt::format("--{}--\r\n", m_boundary));
        return data;
    }
};

MultipartForm::MultipartForm() : m_impl(std::make_shared<Impl>()) {}
MultipartForm::~MultipartForm() = default;

MultipartForm& MultipartForm::param(std::string name, std::string value) {
    if (!m_impl->isBuilt()) {
        m_impl->m_params.insert_or_assign(std::move(name), std::move(value));
    }
    return *this;
}

Result<MultipartForm&> MultipartForm::file(std::string name, std::filesystem::path const& path, std::string mime) {
    if (!m_impl->isBuilt()) {
        std::string filename = utils::string::pathToString(path.filename());

        // according to mdn, filenames should be ascii
        for (unsigned char c : filename) {
            if (c < 0x20 || c > 0x7E) {
                return Err("Invalid character in filename (0x{:X}): '{}'", c, filename);
            }
        }

        GEODE_UNWRAP_INTO(auto data, utils::file::readBinary(path));

        m_impl->m_files.insert_or_assign(std::move(name), MultipartFile{
            .data = std::move(data),
            .filename = std::move(filename),
            .mime = std::move(mime),
        });
    }
    return Ok(*this);
}

MultipartForm& MultipartForm::file(std::string name, std::span<uint8_t const> data, std::string filename, std::string mime) {
    if (!m_impl->isBuilt()) {
        m_impl->m_files.insert_or_assign(std::move(name), MultipartFile{
            .data = ByteVector(data.begin(), data.end()),
            .filename = std::move(filename),
            .mime = std::move(mime),
        });
    }
    return *this;
}

std::string_view MultipartForm::getBoundary() const {
    m_impl->pickUniqueBoundary();
    return m_impl->m_boundary;
}

std::string MultipartForm::getHeader() const {
    return fmt::format("multipart/form-data; boundary={}", getBoundary());
}

ByteVector MultipartForm::getBody() const {
    return m_impl->getBody();
}

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
    return matjson::parse(value).mapErr([&](auto const& err) {
        return fmt::format("Error parsing JSON: {}", err);
    });
}
ByteVector const& WebResponse::data() const& {
    return m_impl->m_data;
}

ByteVector WebResponse::data() && {
    return std::move(m_impl->m_data);
}

Result<> WebResponse::into(std::filesystem::path const& path) const {
    return m_impl->into(path);
}

std::vector<std::string> WebResponse::headers() const {
    return map::keys(m_impl->m_headers);
}

std::optional<ZStringView> WebResponse::header(std::string_view name) const {
    auto it = m_impl->m_headers.find(name);
    if (it != m_impl->m_headers.end()) {
        return it->second.front();
    }
    return std::nullopt;
}

std::optional<std::vector<std::string>> WebResponse::getAllHeadersNamed(std::string_view name) const {
    auto it = m_impl->m_headers.find(name);
    if (it != m_impl->m_headers.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::string_view WebResponse::errorMessage() const {
    return m_impl->m_errMessage;
}

class web::WebRequestsManager {
private:
    class Impl;
    Impl* m_impl;

    WebRequestsManager();
    ~WebRequestsManager();

public:
    static WebRequestsManager* get();

    struct RequestData {
        std::shared_ptr<WebRequest::Impl> request;
        WebResponse response;
        WebTask::PostResult onComplete;
        WebTask::PostProgress onProgress;
        WebTask::HasBeenCancelled hasBeenCancelled;
    };

    void enqueue(std::shared_ptr<RequestData> data);
    WebResponse enqueueAndWait(std::shared_ptr<WebRequest::Impl> data, WebTask::PostProgress progress = nullptr);
};

static void hexAppend(auto& buf, unsigned char c) {
    auto hexDigits = "0123456789ABCDEF";
    buf.append(hexDigits[(c >> 4) & 0xf]);
    buf.append(hexDigits[c & 0xf]);
}

// Encodes a url param
static void urlEncodeAppend(auto& buf, std::string_view input) {
    for (char c : input) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            buf.append(c);
        } else {
            buf.append('%');
            hexAppend(buf, static_cast<unsigned char>(c));
        }
    }
}

class WebRequest::Impl {
public:
    static std::atomic_size_t s_idCounter;

    std::string m_method;
    std::string m_url;
    utils::StringMap<std::vector<std::string>> m_headers;
    utils::StringMap<std::string> m_urlParameters;
    std::optional<std::string> m_userAgent;
    std::optional<std::string> m_acceptEncodingType;
    std::optional<ByteVector> m_body;
    std::optional<std::chrono::seconds> m_timeout;
    std::optional<std::pair<std::uint64_t, std::uint64_t>> m_range;
    bool m_certVerification = true;
    bool m_transferBody = true;
    bool m_followRedirects = true;
    bool m_ignoreContentLength = false;
    std::string m_CABundleContent;
    ProxyOpts m_proxyOpts = {};
    HttpVersion m_httpVersion = HttpVersion::DEFAULT;
    size_t m_id;

    // stored to clean up later
    char m_errorBuf[CURL_ERROR_SIZE] = {0};
    curl_slist* m_curlHeaders = nullptr;

    Impl() : m_id(s_idCounter++) {}
    ~Impl() {
        if (m_curlHeaders) {
            curl_slist_free_all(m_curlHeaders);
        }
    }

    WebResponse makeError(int code, std::string_view msg) {
        auto res = WebResponse();
        res.m_impl->m_code = code;
        res.m_impl->m_data = ByteVector(msg.begin(), msg.end());
        return res;
    }

    CURL* makeCurlHandle(WebRequestsManager::RequestData* requestData) {
        auto curl = curl_easy_init();
        if (!curl) {
            log::error("Failed to initialize cURL");
            return nullptr;
        }

        // todo: in the future, we might want to support downloading directly into
        // files / in-memory streams like the old AsyncWebRequest class

        // Store downloaded response data into a byte vector
        using ResponseData = WebRequestsManager::RequestData;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, requestData);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char* data, size_t size, size_t nmemb, void* ptr) {
            auto& target = static_cast<ResponseData*>(ptr)->response.m_impl->m_data;
            target.insert(target.end(), data, data + size * nmemb);
            return size * nmemb;
        });

        // Set headers
        m_curlHeaders = nullptr;
        for (auto& [name, values] : m_headers) {
            // Sanitize header name
            auto header = name;
            header.erase(std::remove_if(header.begin(), header.end(), [](char c) {
                return c == '\r' || c == '\n';
            }), header.end());
            header.append(": ");
            size_t origSize = header.size();

            // Append values
            for (const auto& value: values) {
                header.append(value);
                m_curlHeaders = curl_slist_append(m_curlHeaders, header.c_str());
                header.resize(origSize);
            }
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, m_curlHeaders);

        // Add parameters to the URL and pass it to curl
        StringBuffer<> urlBuffer{m_url};
        bool first = m_url.find('?') == std::string::npos;

        for (auto& [key, value] : m_urlParameters) {
            urlBuffer.append(first ? '?' : '&');
            urlEncodeAppend(urlBuffer, key);
            urlBuffer.append('=');
            urlEncodeAppend(urlBuffer, value);
            first = false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, urlBuffer.c_str());

        // Set HTTP version
        auto useHttp1 = Loader::get()->getLaunchFlag("use-http1");
        if (m_httpVersion == HttpVersion::DEFAULT && useHttp1) {
            m_httpVersion = HttpVersion::VERSION_1_1; // Force HTTP/1.1 if the flag is set
        }

        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, unwrapHttpVersion(m_httpVersion));

        // Set request method
        if (m_method != "GET") {
            if (m_method == "POST") {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
            }
            else {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, m_method.c_str());
            }
        }

        // Set body if provided
        if (m_body) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_body->data());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, m_body->size());
        } else if (m_method == "POST") {
            // curl_easy_perform would freeze on a POST request with no fields, so set it to an empty string
            // why? god knows
            // SMJS: because the stream isn't complete without a body according to the spec
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        }

        // Cert verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, m_certVerification ? 1L : 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        int sslOptions = 0;

        std::string_view caBundle;

        if (m_certVerification) {
            if (m_CABundleContent.empty()) {
                caBundle = CA_BUNDLE_CONTENT;
            } else {
                caBundle = m_CABundleContent;
            }

            if (!caBundle.empty()) {
                curl_blob caBundleBlob = {};
                caBundleBlob.data = const_cast<void*>(static_cast<const void*>(caBundle.data()));
                caBundleBlob.len = caBundle.size();
                caBundleBlob.flags = CURL_BLOB_NOCOPY;
                curl_easy_setopt(curl, CURLOPT_CAINFO_BLOB, &caBundleBlob);
                // Also add the native CA, for good measure
                sslOptions |= CURLSSLOPT_NATIVE_CA;
            }
        }

        // weird windows stuff, don't remove if we still use schannel!
        GEODE_WINDOWS(sslOptions |= CURLSSLOPT_REVOKE_BEST_EFFORT);
        curl_easy_setopt(curl, CURLOPT_SSL_OPTIONS, sslOptions);

        // Transfer body
        curl_easy_setopt(curl, CURLOPT_NOBODY, m_transferBody ? 0L : 1L);

        // Set user agent if provided
        if (m_userAgent) {
            curl_easy_setopt(curl, CURLOPT_USERAGENT, m_userAgent->c_str());
        }

        // Set encoding
        if (m_acceptEncodingType) {
            curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, m_acceptEncodingType->c_str());
        }

        // Set timeout
        if (m_timeout) {
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_timeout->count());
        }

        // Set range
        if (m_range) {
            curl_easy_setopt(curl, CURLOPT_RANGE, fmt::format("{}-{}", m_range->first, m_range->second).c_str());
        }

        // Set proxy options
        auto const& proxyOpts = m_proxyOpts;
        if (!proxyOpts.address.empty()) {
            curl_easy_setopt(curl, CURLOPT_PROXY, proxyOpts.address.c_str());

            if (proxyOpts.port.has_value()) {
                curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxyOpts.port.value());
            }

            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, unwrapProxyType(proxyOpts.type));

            if (!proxyOpts.username.empty()) {
                curl_easy_setopt(curl, CURLOPT_PROXYAUTH, unwrapHttpAuth(proxyOpts.auth));
                curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD,
                    fmt::format("{}:{}", proxyOpts.username, proxyOpts.password).c_str());
            }

            curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, proxyOpts.tunneling ? 1 : 0);
            curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYPEER, proxyOpts.certVerification ? 1 : 0);
            curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYHOST, 2);
        }

        // Follow request through 3xx responses
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, m_followRedirects ? 1L : 0L);

        // Ignore content length
        curl_easy_setopt(curl, CURLOPT_IGNORE_CONTENT_LENGTH, m_ignoreContentLength ? 1L : 0L);

        // Track progress
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

        // don't change the method from POST to GET when following a redirect
        curl_easy_setopt(curl, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);

        // Do not fail if response code is 4XX or 5XX
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L);

        // IPv4
        if (Mod::get()->getLaunchFlag("curl-force-ipv4")) {
            curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        }

        // Verbose logging
        if (Mod::get()->getSettingValue<bool>("verbose-curl-logs")) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, +[](CURL* handle, curl_infotype type, char* data, size_t size, void* clientp) {
                while (size > 0 && (data[size - 1] == '\n' || data[size - 1] == '\r')) {
                    size--; // remove trailing newline
                }

                switch (type) {
                    case CURLINFO_TEXT:
                        log::debug("[Curl] {}", std::string_view(data, size));
                        break;
                    case CURLINFO_HEADER_IN:
                        log::debug("[Curl] Header in: {}", std::string_view(data, size));
                        break;
                    case CURLINFO_HEADER_OUT:
                        log::debug("[Curl] Header out: {}", std::string_view(data, size));
                        break;
                    case CURLINFO_DATA_IN:
                        log::debug("[Curl] Data in ({} bytes)", size);
                        break;
                    case CURLINFO_DATA_OUT:
                        log::debug("[Curl] Data out ({} bytes)", size);
                        break;
                    case CURLINFO_SSL_DATA_IN:
                        log::debug("[Curl] SSL data in ({} bytes)", size);
                        break;
                    case CURLINFO_SSL_DATA_OUT:
                        log::debug("[Curl] SSL data out ({} bytes)", size);
                        break;
                    case CURLINFO_END:
                        log::debug("[Curl] End of info");
                        break;
                    default:
                        log::debug("[Curl] Unknown info type: {}", static_cast<int>(type));
                        break;
                }
            });
        }

        // If an error happens, we want to get a more specific description of the issue
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, m_errorBuf);

        // Get headers from the response
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, requestData);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, (+[](char* buffer, size_t size, size_t nitems, void* ptr) {
            auto& headers = static_cast<ResponseData*>(ptr)->response.m_impl->m_headers;
            std::string line;
            std::stringstream ss(std::string(buffer, size * nitems));
            while (std::getline(ss, line)) {
                auto colon = line.find(':');
                if (colon == std::string::npos) continue;

                auto lineview = std::string_view(line);
                auto key = lineview.substr(0, colon);
                auto value = lineview.substr(colon + 2);
                if (value.ends_with('\r')) {
                    value.remove_suffix(1);
                }
                // Create a new vector and add to it or add to an already existing one
                auto it = headers.find(key);
                if (it != headers.end()) {
                    it->second.push_back(std::string{value});
                } else {
                    headers.insert_or_assign(std::string{key}, std::vector{std::string{value}});
                }
            }
            return size * nitems;
        }));

        // Track & post progress on the Promise
        // onProgress can only be not set if using sendSync without one, and hasBeenCancelled is always null in that case
        if (requestData->onProgress) {
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, requestData);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, +[](void* ptr, double dtotal, double dnow, double utotal, double unow) -> int {
                auto data = static_cast<ResponseData*>(ptr);

                // Check for cancellation and abort if so
                if (data->hasBeenCancelled && data->hasBeenCancelled()) {
                    return 1;
                }

                // Post progress to Promise listener
                WebProgress progress{};
                progress.m_downloadTotal = dtotal;
                progress.m_downloadCurrent = dnow;
                progress.m_uploadTotal = utotal;
                progress.m_uploadCurrent = unow;
                data->onProgress(progress);

                // Continue as normal
                return 0;
            });
        }

        return curl;
    }
};

std::atomic_size_t WebRequest::Impl::s_idCounter = 0;

WebRequest::WebRequest() : m_impl(std::make_shared<Impl>()) {}
WebRequest::~WebRequest() {}

WebTask WebRequest::send(std::string method, std::string url) {
    auto [task, result, progress, cancelled] = WebTask::spawn(fmt::format("{} {}", method, url));
    m_impl->m_method = std::move(method);
    m_impl->m_url = std::move(url);

    WebRequestsManager::get()->enqueue(std::make_shared<WebRequestsManager::RequestData>(
        m_impl,
        WebResponse(),
        std::move(result),
        std::move(progress),
        std::move(cancelled)
    ));

    return task;
}
WebTask WebRequest::post(std::string url) {
    return this->send("POST", std::move(url));
}
WebTask WebRequest::get(std::string url) {
    return this->send("GET", std::move(url));
}
WebTask WebRequest::put(std::string url) {
    return this->send("PUT", std::move(url));
}
WebTask WebRequest::patch(std::string url) {
    return this->send("PATCH", std::move(url));
}

WebResponse WebRequest::sendSync(std::string method, std::string url, WebTask::PostProgress onProgress) {
    m_impl->m_method = std::move(method);
    m_impl->m_url = std::move(url);
    return WebRequestsManager::get()->enqueueAndWait(m_impl, std::move(onProgress));
}
WebResponse WebRequest::postSync(std::string url, WebTask::PostProgress onProgress) {
    return this->sendSync("POST", std::move(url), std::move(onProgress));
}
WebResponse WebRequest::getSync(std::string url, WebTask::PostProgress onProgress) {
    return this->sendSync("GET", std::move(url), std::move(onProgress));
}
WebResponse WebRequest::putSync(std::string url, WebTask::PostProgress onProgress) {
    return this->sendSync("PUT", std::move(url), std::move(onProgress));
}
WebResponse WebRequest::patchSync(std::string url, WebTask::PostProgress onProgress) {
    return this->sendSync("PATCH", std::move(url), std::move(onProgress));
}

WebRequest& WebRequest::header(std::string name, std::string value) {
    if (equalsIgnoreCase(name, "User-Agent")) {
        this->userAgent(std::move(value));
        return *this;
    } else if (equalsIgnoreCase(name, "Accept-Encoding")) {
        this->acceptEncoding(std::move(value));
        return *this;
    } else if (equalsIgnoreCase(name, "Keep-Alive")) {
        const size_t timeoutPos = value.find("timeout");

        if (timeoutPos != std::string::npos) {
            // At this point idc what happens if I get NPOS or string ends, you shouldn't custom format a spec header
            const size_t numStart = value.find('=', timeoutPos) + 1;
            const size_t comma = value.find(',', numStart);
            const size_t numLength = (comma == std::string::npos ? value.size() : comma) - numStart;

            int timeoutValue = 5;
            auto res = numFromString<int>(value.substr(numStart, numLength));
            if (res) {
                timeoutValue = res.unwrap();
            }

            timeout(std::chrono::seconds(timeoutValue));

            return *this;
        }
    }

    // Create a new vector and add to it or add to an already existing one
    auto it = m_impl->m_headers.find(name);
    if (it != m_impl->m_headers.end()) {
        it->second.push_back(std::move(value));
    } else {
        m_impl->m_headers.insert_or_assign(std::move(name), std::vector{std::move(value)});
    }

    return *this;
}

WebRequest& WebRequest::removeHeader(std::string_view name) {
    auto it = m_impl->m_headers.find(name);
    if (it != m_impl->m_headers.end()) {
        m_impl->m_headers.erase(it);
    }
    return *this;
}

WebRequest& WebRequest::param(std::string name, std::string value) {
    m_impl->m_urlParameters.insert_or_assign(std::move(name), std::move(value));
    return *this;
}

WebRequest& WebRequest::removeParam(std::string_view name) {
    auto it = m_impl->m_urlParameters.find(name);
    if (it != m_impl->m_urlParameters.end()) {
        m_impl->m_urlParameters.erase(it);
    }
    return *this;
}

WebRequest& WebRequest::userAgent(std::string name) {
    m_impl->m_userAgent = std::move(name);
    return *this;
}

WebRequest& WebRequest::timeout(std::chrono::seconds time) {
    m_impl->m_timeout = time;
    return *this;
}

WebRequest& WebRequest::downloadRange(std::pair<std::uint64_t, std::uint64_t> byteRange) {
    m_impl->m_range = byteRange;
    return *this;
}

WebRequest& WebRequest::certVerification(bool enabled) {
    m_impl->m_certVerification = enabled;
    return *this;
}
WebRequest& WebRequest::transferBody(bool enabled) {
    m_impl->m_transferBody = enabled;
    return *this;
}

WebRequest& WebRequest::followRedirects(bool enabled) {
    m_impl->m_followRedirects = enabled;
    return *this;
}

WebRequest& WebRequest::ignoreContentLength(bool enabled) {
    m_impl->m_ignoreContentLength = enabled;
    return *this;
}

WebRequest& WebRequest::CABundleContent(std::string content) {
    m_impl->m_CABundleContent = std::move(content);
    return *this;
}

WebRequest& WebRequest::proxyOpts(ProxyOpts proxyOpts) {
    m_impl->m_proxyOpts = std::move(proxyOpts);
    return *this;
}

WebRequest& WebRequest::version(HttpVersion httpVersion) {
    m_impl->m_httpVersion = httpVersion;
    return *this;
}

WebRequest& WebRequest::acceptEncoding(std::string str) {
    m_impl->m_acceptEncodingType = std::move(str);
    return *this;
}

WebRequest& WebRequest::body(ByteVector raw) {
    m_impl->m_body = std::move(raw);
    return *this;
}
WebRequest& WebRequest::bodyString(std::string_view str) {
    m_impl->m_body = ByteVector { str.begin(), str.end() };
    return *this;
}
WebRequest& WebRequest::bodyJSON(matjson::Value const& json) {
    this->header("Content-Type", "application/json");
    std::string str = json.dump(matjson::NO_INDENTATION);
    m_impl->m_body = ByteVector { str.begin(), str.end() };
    return *this;
}
WebRequest& WebRequest::bodyMultipart(MultipartForm const& form) {
    this->header("Content-Type", form.getHeader());
    m_impl->m_body = form.getBody();
    return *this;
}

size_t WebRequest::getID() const {
    return m_impl->m_id;
}

ZStringView WebRequest::getMethod() const {
    return m_impl->m_method;
}

ZStringView WebRequest::getUrl() const {
    return m_impl->m_url;
}

utils::StringMap<std::vector<std::string>> const& WebRequest::getHeaders() const {
    return m_impl->m_headers;
}

utils::StringMap<std::string> const& WebRequest::getUrlParams() const {
    return m_impl->m_urlParameters;
}

std::optional<ByteVector> WebRequest::getBody() const {
    return m_impl->m_body;
}

std::optional<std::chrono::seconds> WebRequest::getTimeout() const {
    return m_impl->m_timeout;
}

HttpVersion WebRequest::getHttpVersion() const {
    return m_impl->m_httpVersion;
}

class WebRequestsManager::Impl {
public:
    CURLM* m_multiHandle;

    std::thread m_worker;
    std::atomic<bool> m_running;
    std::mutex m_mutex;

    std::queue<std::shared_ptr<RequestData>> m_pendingRequests;
    std::unordered_map<CURL*, std::shared_ptr<RequestData>> m_activeRequests;

    std::binary_semaphore m_semaphore{0};

    Impl() {
        m_multiHandle = curl_multi_init();
        curl_multi_setopt(m_multiHandle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 32L);
        curl_multi_setopt(m_multiHandle, CURLMOPT_MAXCONNECTS, 16L);

        m_running = true;
        m_worker = std::thread(&Impl::workerThread, this);
    }

    ~Impl() {
        m_running = false;
        m_semaphore.release();
        curl_multi_wakeup(m_multiHandle);
        if (m_worker.joinable()) {
            m_worker.join();
        }

        curl_multi_cleanup(m_multiHandle);
    }

    void workerThread() {
        thread::setName("Geode Web Worker");

        while (m_running) {
            {
                std::unique_lock lock(m_mutex);
                while (!m_pendingRequests.empty()) {
                    auto& reqData = m_pendingRequests.front();
                    CURL* handle = reqData->request->makeCurlHandle(reqData.get());

                    if (!handle) {
                        reqData->onComplete(reqData->request->makeError(-1, "Failed to initialize cURL"));
                        m_pendingRequests.pop();
                        continue;
                    }

                    curl_multi_add_handle(m_multiHandle, handle);
                    m_activeRequests.insert({ handle, std::move(reqData) });
                    m_pendingRequests.pop();
                }
            }

            int stillRunning = 0;
            CURLMcode mc = curl_multi_perform(m_multiHandle, &stillRunning);
            if (mc != CURLM_OK) {
                log::error("curl_multi_perform() failed: {}", curl_multi_strerror(mc));
            }

            // check for completed requests
            int msgsInQueue = 0;
            while (auto* msg = curl_multi_info_read(m_multiHandle, &msgsInQueue)) {
                if (msg->msg == CURLMSG_DONE) {
                    CURL* handle = msg->easy_handle;

                    // handle the completed request
                    auto& requestData = *m_activeRequests.at(handle);

                    // Get the response code; note that this will be invalid if the
                    // curlResponse is not CURLE_OK
                    long code = 0;
                    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &code);

                    requestData.response.m_impl->m_code = static_cast<int>(code);

                    char* errorBuf = requestData.request->m_errorBuf;
                    requestData.response.m_impl->m_errMessage = std::string(errorBuf);

                    // Check if the request failed on curl's side or because of cancellation
                    if (msg->data.result != CURLE_OK) {
                        if (requestData.hasBeenCancelled && requestData.hasBeenCancelled()) {
                            log::debug("Request cancelled");
                            requestData.onComplete(WebTask::Cancel());
                        }
                        else {
                            std::string_view err = curl_easy_strerror(msg->data.result);
                            log::error("cURL failure, error: {}", err);
                            log::warn("Error buffer: {}", errorBuf);
                            requestData.onComplete(requestData.request->makeError(
                                -1,
                                !errorBuf ?
                                      fmt::format("Curl failed: {}", err)
                                    : fmt::format("Curl failed: {} ({})", err, errorBuf)
                            ));
                        }
                    } else {
                        // resolve with success :-)
                        requestData.onComplete(std::move(requestData.response));
                    }

                    // clean up
                    curl_multi_remove_handle(m_multiHandle, handle);
                    curl_easy_cleanup(handle);
                    {
                        std::lock_guard lock(m_mutex);
                        m_activeRequests.erase(handle);
                    }
                }
            }

            bool idle;
            {
                std::lock_guard lock(m_mutex);
                idle = m_pendingRequests.empty() && m_activeRequests.empty();
            }

            if (idle) {
                m_semaphore.acquire();
                continue;
            }

            if (!m_activeRequests.empty()) {
                int numfds = 0;
                curl_multi_poll(m_multiHandle, nullptr, 0, -1, &numfds);
            }
        }

        // clean up remaining requests
        for (auto& [handle, _] : m_activeRequests) {
            curl_multi_remove_handle(m_multiHandle, handle);
            curl_easy_cleanup(handle);
        }
        m_activeRequests.clear();
    }
};

WebRequestsManager::WebRequestsManager() : m_impl(new Impl()) {}
WebRequestsManager::~WebRequestsManager() = default; // leaking m_impl on purpose to avoid destruction order issues

WebRequestsManager* WebRequestsManager::get() {
    static WebRequestsManager instance;
    return &instance;
}

void WebRequestsManager::enqueue(std::shared_ptr<RequestData> data) {
    {
        std::lock_guard lock(m_impl->m_mutex);
        m_impl->m_pendingRequests.push(std::move(data));
    }
    m_impl->m_semaphore.release();
    curl_multi_wakeup(m_impl->m_multiHandle);
}

WebResponse WebRequestsManager::enqueueAndWait(std::shared_ptr<WebRequest::Impl> data, WebTask::PostProgress progress) {
    std::binary_semaphore sem{0};
    WebResponse response;

    this->enqueue(std::make_shared<RequestData>(
        std::move(data), response,
        [&](WebTask::Result&&) {
            sem.release();
        },
        std::move(progress),
        nullptr
    ));

    sem.acquire();
    return response;
}