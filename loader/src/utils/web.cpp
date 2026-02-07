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
#include <arc/future/Select.hpp>
#include <arc/time/Sleep.hpp>
#include <arc/task/CancellationToken.hpp>
#include <arc/sync/Mutex.hpp>
#include <arc/sync/mpsc.hpp>
#include <asp/collections/SmallVec.hpp>
#include <ca_bundle.h>
#include <curl/curl.h>
#include <queue>
#include <semaphore>
#include <sstream>

using namespace geode::prelude;
using namespace geode::utils::web;
using namespace geode::utils::string;
using namespace arc;

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

bool WebResponse::info() const {
    return m_impl->m_code >= 100 && m_impl->m_code < 200;
}
bool WebResponse::ok() const {
    return m_impl->m_code >= 200 && m_impl->m_code < 300;
}
bool WebResponse::redirected() const {
    return m_impl->m_code >= 300 && m_impl->m_code < 400;
}
bool WebResponse::badClient() const {
    return m_impl->m_code >= 400 && m_impl->m_code < 500;
}
bool WebResponse::badServer() const {
    return m_impl->m_code >= 500 && m_impl->m_code < 600;
}
bool WebResponse::error() const {
    return m_impl->m_code < 0;
}
bool WebResponse::cancelled() const {
    return m_impl->m_code == static_cast<int>(GeodeWebError::REQUEST_CANCELLED);
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

    struct RequestData : std::enable_shared_from_this<RequestData> {
        std::shared_ptr<WebRequest::Impl> request;
        Mod* mod;
        WebResponse response;
        geode::Function<void(WebResponse)> onComplete;
        CURL* curl = nullptr;

        RequestData(std::shared_ptr<WebRequest::Impl> req, Mod* m, geode::Function<void(WebResponse)> cb)
            : request(std::move(req)), mod(m), onComplete(std::move(cb)) {}

        void complete(WebResponse res) {
            onComplete(res);

            WebResponseEvent(mod->getID()).send(res);
        }

        void onError(int code, std::string_view msg) {
            auto res = WebResponse();
            res.m_impl->m_code = code;
            res.m_impl->m_data = ByteVector(msg.begin(), msg.end());
            complete(std::move(res));
        }

        void onError(GeodeWebError code, std::string_view msg) {
            onError(static_cast<int>(code), msg);
        }
    };

    arc::mpsc::SendResult<std::shared_ptr<RequestData>> tryEnqueue(std::shared_ptr<RequestData> data);
    void cancel(std::shared_ptr<RequestData> data);
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
    std::vector<geode::Function<void(WebProgress const&)>> m_progressCallbacks;
    std::string m_CABundleContent;
    bool m_certVerification = true;
    bool m_transferBody = true;
    bool m_followRedirects = true;
    bool m_ignoreContentLength = false;
    ProxyOpts m_proxyOpts = {};
    HttpVersion m_httpVersion = HttpVersion::DEFAULT;
    size_t m_id;
    Mod* m_mod;
    bool m_inInterceptor = false;
    std::atomic<size_t> m_downloadCurrent = 0;
    std::atomic<size_t> m_downloadTotal = 0;
    std::atomic<size_t> m_uploadCurrent = 0;
    std::atomic<size_t> m_uploadTotal = 0;
    std::atomic<bool> m_progressNotifQueued{false};
    std::atomic<bool> m_cancelled{false};

    // stored to clean up later
    char m_errorBuf[CURL_ERROR_SIZE] = {0};
    curl_slist* m_curlHeaders = nullptr;

    Impl() : m_id(s_idCounter++) {}
    ~Impl() {
        if (m_curlHeaders) {
            curl_slist_free_all(m_curlHeaders);
        }
    }

    WebResponse makeError(GeodeWebError code, std::string_view msg) {
        auto res = WebResponse();
        res.m_impl->m_code = static_cast<int>(code);
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
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, requestData);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, +[](void* ptr, double dtotal, double dnow, double utotal, double unow) -> int {
            auto data = static_cast<ResponseData*>(ptr);

            // TODO v5: external cancellation?

            // Store progress inside the request
            using enum std::memory_order;
            auto& r = *data->request;

            r.m_downloadTotal.store(static_cast<size_t>(dtotal), relaxed);
            r.m_downloadCurrent.store(static_cast<size_t>(dnow), relaxed);
            r.m_uploadTotal.store(static_cast<size_t>(utotal), relaxed);
            r.m_uploadCurrent.store(static_cast<size_t>(unow), relaxed);

            // Queue the callback in the main thread, make sure to do it only once per frame
            if (!r.m_progressCallbacks.empty() && !r.m_progressNotifQueued.exchange(true, acq_rel) && !r.m_cancelled.load(relaxed)) {
                queueInMainThread([req = data->request] {
                    if (req->m_cancelled.load(relaxed)) return;
                    req->m_progressNotifQueued.store(false, release);

                    for (auto& callback : req->m_progressCallbacks) {
                        callback(req->progress());
                    }
                });
            }

            // Continue as normal
            return 0;
        });

        return curl;
    }

    WebProgress progress() const {
        using enum std::memory_order;
        WebProgress p{};
        p.m_downloadCurrent = m_downloadCurrent.load(relaxed);
        p.m_downloadTotal = m_downloadTotal.load(relaxed);
        p.m_uploadCurrent = m_uploadCurrent.load(relaxed);
        p.m_uploadTotal = m_uploadTotal.load(relaxed);
        return p;
    }
};

std::atomic_size_t WebRequest::Impl::s_idCounter = 0;

WebRequest::WebRequest() : m_impl(std::make_shared<Impl>()) {}
WebRequest::~WebRequest() {}

WebFuture WebRequest::send(std::string method, std::string url, Mod* mod) {
    if (m_impl->m_inInterceptor) utils::terminate("Cannot call send again within an interceptor.", mod);

    m_impl->m_mod = mod;

    m_impl->m_method = std::move(method);
    m_impl->m_url = std::move(url);
    m_impl->m_inInterceptor = true;

    WebRequestInterceptEvent(mod->getID()).send(*this);

    m_impl->m_inInterceptor = false;

    return WebFuture{m_impl};
}
WebFuture WebRequest::post(std::string url, Mod* mod) {
    return this->send("POST", std::move(url), mod);
}
WebFuture WebRequest::get(std::string url, Mod* mod) {
    return this->send("GET", std::move(url), mod);
}
WebFuture WebRequest::put(std::string url, Mod* mod) {
    return this->send("PUT", std::move(url), mod);
}
WebFuture WebRequest::patch(std::string url, Mod* mod) {
    return this->send("PATCH", std::move(url), mod);
}

WebResponse WebRequest::sendSync(std::string method, std::string url, Mod* mod) {
    auto fut = this->send(std::move(method), std::move(url), mod);
    return async::runtime().blockOn(std::move(fut));
}
WebResponse WebRequest::postSync(std::string url, Mod* mod) {
    return this->sendSync("POST", std::move(url), mod);
}
WebResponse WebRequest::getSync(std::string url, Mod* mod) {
    return this->sendSync("GET", std::move(url), mod);
}
WebResponse WebRequest::putSync(std::string url, Mod* mod) {
    return this->sendSync("PUT", std::move(url), mod);
}
WebResponse WebRequest::patchSync(std::string url, Mod* mod) {
    return this->sendSync("PATCH", std::move(url), mod);
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

WebRequest& WebRequest::method(std::string method) {
    m_impl->m_method = std::move(method);
    return *this;
}

WebRequest& WebRequest::url(std::string url) {
    m_impl->m_url = std::move(url);
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

WebRequest& WebRequest::onProgress(Function<void(WebProgress const&)> callback) {
    m_impl->m_progressCallbacks.emplace_back(std::move(callback));
    return *this;
}

size_t WebRequest::getID() const {
    return m_impl->m_id;
}

Mod* WebRequest::getMod() const {
    return m_impl->m_mod;
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

WebProgress WebRequest::getProgress() const {
    return m_impl->progress();
}

struct RegisteredSocket {
    arc::Registration rio;
    uint64_t rioId;
    arc::Interest interest;
};

struct PollReadiness {
    curl_socket_t socket;
    Interest readiness;
};

struct ARC_NODISCARD MultiPollFuture : Pollable<MultiPollFuture, PollReadiness> {
    enum class State {
        Init,
        Waiting,
    } m_state = State::Init;
    std::unordered_map<curl_socket_t, RegisteredSocket>* m_sockets;
    asp::SmallVec<std::pair<curl_socket_t, uint64_t>, 16> registered;

    explicit MultiPollFuture(decltype(m_sockets) sockets) : m_sockets(sockets) {}

    MultiPollFuture(MultiPollFuture&&) = default;
    MultiPollFuture& operator=(MultiPollFuture&&) = default;

    std::optional<PollReadiness> poll(arc::Context& cx) {
        switch (m_state) {
            case State::Init: {
                // initial state: poll all sockets, return immediately if there's activity on one of them
                for (auto& [fd, rs] : *m_sockets) {
                    auto ready = rs.rio.pollReady(rs.interest | Interest::Error, cx, rs.rioId);
                    if (ready != 0) {
                        return PollReadiness{fd, ready};
                    }

                    registered.emplace_back(fd, rs.rioId);
                }

                m_state = State::Waiting;
                return std::nullopt;
            } break;

            case State::Waiting: {
                // nothing to do here, just wait to get woken up
                return std::nullopt;
            } break;

            default: std::unreachable();
        }
    }

    ~MultiPollFuture() {
        // unregister all ios
        for (auto const& [fd, rioId] : registered) {
            auto it = m_sockets->find(fd);
            if (it != m_sockets->end()) {
                it->second.rio.unregister(rioId);
                it->second.rioId = 0;
            }
        }
    }
};

class WebRequestsManager::Impl {
public:
    CURLM* m_multiHandle;

    std::optional<arc::TaskHandle<void>> m_worker;
    std::optional<arc::mpsc::Sender<std::shared_ptr<RequestData>>> m_reqtx;
    std::optional<arc::mpsc::Sender<std::shared_ptr<RequestData>>> m_canceltx;
    arc::CancellationToken m_cancel;
    arc::Notify m_wakeNotify;
    asp::Instant m_nextWakeup = asp::Instant::farFuture();

    std::unordered_set<std::shared_ptr<RequestData>> m_activeRequests;
    std::unordered_map<curl_socket_t, RegisteredSocket> m_sockets;

    Impl() {
        auto [tx, rx] = arc::mpsc::channel<std::shared_ptr<RequestData>>(1024);
        auto [ctx, crx] = arc::mpsc::channel<std::shared_ptr<RequestData>>();

        m_reqtx = std::move(tx);
        m_canceltx = std::move(ctx);

        m_multiHandle = curl_multi_init();
        curl_multi_setopt(m_multiHandle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 32L);
        curl_multi_setopt(m_multiHandle, CURLMOPT_MAXCONNECTS, 16L);
        curl_multi_setopt(m_multiHandle, CURLMOPT_SOCKETFUNCTION, +[](CURL* easy, curl_socket_t s, int what, void* userp, void* socketp) -> int {
            auto self = static_cast<Impl*>(userp);
            self->socketCallback(easy, s, what, socketp);
            return 0;
        });
        curl_multi_setopt(m_multiHandle, CURLMOPT_SOCKETDATA, this);
        curl_multi_setopt(m_multiHandle, CURLMOPT_TIMERFUNCTION, +[](CURLM* multi, long timeout_ms, void* userp) -> int {
            auto self = static_cast<Impl*>(userp);
            self->timerCallback(multi, timeout_ms);
            return 0;
        });
        curl_multi_setopt(m_multiHandle, CURLMOPT_TIMERDATA, this);

        m_worker = async::runtime().spawn([this, rx = std::move(rx), crx = std::move(crx)] mutable -> arc::Future<> {
            bool running = true;
            while (running) {
                co_await arc::select(
                    arc::selectee(
                        m_cancel.waitCancelled(),
                        [&] { running = false; }
                    ),

                    arc::selectee(rx.recv(), [&](auto req) {
                        if (req) this->workerAddRequest(std::move(req).unwrap());
                    }),

                    arc::selectee(crx.recv(), [&](auto req) {
                        if (req) this->workerCancelRequest(std::move(req).unwrap());
                    }),

                    arc::selectee(
                        this->workerPoll()
                    )
                );
            }
        });
        m_worker->setName("Geode Web Worker");
    }

    // Note for future people: this is currently leaked because cleanup is unsafe in statics
    // if this becomes not leaked in the future pls remember to store arc runtime as weakptr
    // or m_worker->abort will likely invoke ub
    ~Impl() {
        m_cancel.cancel();

        if (m_worker) {
            m_worker->abort();
        }

        // clean up remaining requests
        for (auto& req : m_activeRequests) {
            curl_multi_remove_handle(m_multiHandle, req->curl);
            curl_easy_cleanup(req->curl);
        }

        curl_multi_cleanup(m_multiHandle);
    }

    void workerAddRequest(std::shared_ptr<RequestData> req) {
        CURL* handle = req->request->makeCurlHandle(req.get());

        if (!handle) {
            req->onError(GeodeWebError::CURL_INITIALIZATION_ERROR, "Failed to initialize cURL");
            return;
        }

        // associate them with each other
        req->curl = handle;
        curl_easy_setopt(handle, CURLOPT_PRIVATE, req.get());

        log::debug("Added request ({})", req->request->m_url);
        curl_multi_add_handle(m_multiHandle, handle);
        m_activeRequests.insert(std::move(req));
        this->workerKickCurl();
    }

    void workerCancelRequest(std::shared_ptr<RequestData> req) {
        log::debug("Cancelled request ({})", req->request->m_url);
        req->onError(GeodeWebError::REQUEST_CANCELLED, "Request cancelled");

        this->cleanupRequest(std::move(req));
    }

    void cleanupRequest(std::shared_ptr<RequestData> req) {
        log::debug("Removing request ({})", req->request->m_url);
        m_activeRequests.erase(req);

        auto curl = std::exchange(req->curl, nullptr);
        if (curl) {
            curl_multi_remove_handle(m_multiHandle, curl);
            curl_easy_cleanup(curl);
        }

        this->workerKickCurl();
    }

    Future<> workerPoll() {
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
                RequestData* rdptr = nullptr;
                curl_easy_getinfo(handle, CURLINFO_PRIVATE, &rdptr);
                if (!rdptr) utils::terminate("queued request has no associated data");

                auto& requestData = *rdptr;

                // Get the response code; note that this will be invalid if the
                // curlResponse is not CURLE_OK
                long code = 0;
                curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &code);

                requestData.response.m_impl->m_code = code;

                char* errorBuf = requestData.request->m_errorBuf;
                requestData.response.m_impl->m_errMessage = std::string(errorBuf);

                // Check if the request failed on curl's side or because of cancellation
                if (msg->data.result != CURLE_OK) {
                    std::string_view err = curl_easy_strerror(msg->data.result);
                    log::error("cURL failure, error: {}", err);
                    log::warn("Error buffer: {}", errorBuf);
                    requestData.onError(
                        // Make all CURL error codes negatives to not conflict with HTTP codes
                        msg->data.result * -1,
                        !errorBuf ?
                                fmt::format("Curl failed: {}", err)
                            : fmt::format("Curl failed: {} ({})", err, errorBuf)
                    );
                } else {
                    // resolve with success :-)
                    requestData.complete(std::move(requestData.response));
                }

                // clean up
                this->cleanupRequest(requestData.shared_from_this());
            }
        }

        // poll for either 250ms or until curl needs us, whatever happens earlier
        auto now = asp::Instant::now();
        auto deadline = std::min(m_nextWakeup, now + asp::Duration::fromMillis(250));

        bool activeTransfers = stillRunning > 0;

        // poll until there is socket activity or the timer expires

        co_await arc::select(
            arc::selectee(m_wakeNotify.notified()),

            arc::selectee(arc::sleepUntil(deadline), [&] {
                this->workerKickCurl();
            }),

            arc::selectee(this->workerPollSockets(), [&](PollReadiness readiness) {
                auto [fd, ready] = readiness;
                int ev = 0;
                if (ready & Interest::Readable) ev |= CURL_CSELECT_IN;
                if (ready & Interest::Writable) ev |= CURL_CSELECT_OUT;
                if (ready & Interest::Error) ev |= CURL_CSELECT_ERR;

                int running = 0;
                curl_multi_socket_action(m_multiHandle, fd, ev, &running);
            }, activeTransfers)
        );
    }

    void workerKickCurl() {
        // it's kind of silly, but any time we do anything (add/remove easy handles, etc.)
        // we should call this function to let curl call our socket callbacks and kickstart everything
        int running = 0;
        curl_multi_socket_action(m_multiHandle, CURL_SOCKET_TIMEOUT, 0, &running);
    }

    MultiPollFuture workerPollSockets() {
        return MultiPollFuture{ &m_sockets };
    }

    void socketCallback(CURL* easy, curl_socket_t s, int what, void* socketp) {
        auto& driver = Runtime::current()->ioDriver();
        auto it = m_sockets.find(s);

        if (what == CURL_POLL_REMOVE) {
            // remove the socket, which unregisters it from the io driver as well
            if (it != m_sockets.end()) {
                m_sockets.erase(it);
            } else {
                log::warn("[WebRequestsManager] Tried to remove unknown socket {}", (int)s);
            }
            return;
        }

        // register the socket with the io driver, or just update interest if registered
        Interest interest{};
        if (what & CURL_POLL_IN) {
            interest |= arc::Interest::Readable;
        }
        if (what & CURL_POLL_OUT) {
            interest |= arc::Interest::Writable;
        }

        if (it != m_sockets.end()) {
            it->second.interest = interest;
        } else {
            auto rio = driver.registerIo(s, arc::Interest::ReadWrite);
            m_sockets.emplace(s, RegisteredSocket{ std::move(rio), 0, interest });
        }
    }

    void timerCallback(CURLM* multi, long timeout_ms) {
        if (timeout_ms == -1) {
            m_nextWakeup = asp::Instant::farFuture();
        } else {
            m_nextWakeup = asp::Instant::now() + asp::Duration::fromMillis(timeout_ms);
        }
    }
};

WebRequestsManager::WebRequestsManager() : m_impl(new Impl()) {}
WebRequestsManager::~WebRequestsManager() = default; // leaking m_impl on purpose to avoid destruction order issues

WebRequestsManager* WebRequestsManager::get() {
    static WebRequestsManager instance;
    return &instance;
}

struct WebFuture::Impl {
    std::shared_ptr<WebRequestsManager::RequestData> m_request;
    arc::oneshot::Receiver<WebResponse> m_rx;
    arc::oneshot::RecvAwaiter<WebResponse> m_awaiter;
    bool m_sent = false;
    bool m_finished = false;

    Impl(std::shared_ptr<WebRequestsManager::RequestData> request, arc::oneshot::Receiver<WebResponse> awaiter)
        : m_request(std::move(request)), m_rx(std::move(awaiter)), m_awaiter(m_rx.recv()) {}
};

WebFuture::WebFuture(std::shared_ptr<WebRequest::Impl> request) {
    auto [tx, rx] = arc::oneshot::channel<WebResponse>();

    auto rdata = std::make_shared<WebRequestsManager::RequestData>(
        std::move(request), request->m_mod, [tx = std::move(tx)](auto res) mutable {
            (void) tx.send(std::move(res));
        }
    );

    m_impl = std::make_shared<Impl>(std::move(rdata), std::move(rx));
}

WebFuture::~WebFuture() {
    if (!m_impl) return;

    if (!m_impl->m_sent) {
        m_impl->m_request->onError(GeodeWebError::REQUEST_CANCELLED, "Request cancelled");
    } else if (!m_impl->m_finished) {
        // future got cancelled, tell request manager to stop this request
        WebRequestsManager::get()->cancel(m_impl->m_request);
    }
}

std::optional<WebResponse> WebFuture::poll(arc::Context& cx) {
    using RequestData = WebRequestsManager::RequestData;

    if (!m_impl->m_sent) {
        // send the actual request
        auto res = WebRequestsManager::get()->tryEnqueue(m_impl->m_request);
        if (!res) {
            return m_impl->m_request->request->makeError(GeodeWebError::QUEUE_FULL, "Failed to enqueue web request: queue is full");
        }
        m_impl->m_sent = true;
    }

    auto rpoll = m_impl->m_awaiter.poll(cx);
    if (!rpoll) {
        return std::nullopt;
    }

    auto result = std::move(rpoll).value();
    if (!result) {
        m_impl->m_finished = true;
        return m_impl->m_request->request->makeError(GeodeWebError::CHANNEL_CLOSED, "Failed to receive web response: channel closed");
    }

    m_impl->m_finished = true;
    return std::move(result).unwrap();
}

void WebRequestsManager::cancel(std::shared_ptr<RequestData> data) {
    // set the cancel flag immediately so our progress callbacks never get called again
    data->request->m_cancelled.store(true, std::memory_order::relaxed);

    // cancel channel is unbounded, so this never fails
    (void) m_impl->m_canceltx->trySend(std::move(data));
}

mpsc::SendResult<std::shared_ptr<WebRequestsManager::RequestData>> WebRequestsManager::tryEnqueue(std::shared_ptr<RequestData> data) {
    return m_impl->m_reqtx->trySend(std::move(data));
}