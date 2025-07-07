#include <Geode/loader/Log.hpp>
#include <Geode/Result.hpp>
#include <Geode/utils/general.hpp>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <matjson.hpp>
#include <system_error>
#define CURL_STATICLIB
#include <curl/curl.h>
#include <ca_bundle.h>

#include <Geode/loader/Mod.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/terminate.hpp>
#include <sstream>

using namespace geode::prelude;
using namespace geode::utils::web;

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
    std::unordered_map<std::string, std::vector<std::string>> m_headers;

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

MultipartForm& MultipartForm::param(std::string_view name, std::string_view value) {
    if (!m_impl->isBuilt()) {
        m_impl->m_params.insert_or_assign(std::string(name), std::string(value));
    }
    return *this;
}

Result<MultipartForm&> MultipartForm::file(std::string_view name, std::filesystem::path const& path, std::string_view mime) {
    if (!m_impl->isBuilt()) {
        GEODE_UNWRAP_INTO(auto data, utils::file::readBinary(path));

        std::string filename = utils::string::pathToString(path.filename());

        // according to mdn, filenames should be ascii
        for (unsigned char c : filename) {
            if (c < 0x20 || c > 0x7E) {
                return Err("Invalid character in filename (0x{:X}): '{}'", c, filename);
            }
        }

        m_impl->m_files.insert_or_assign(std::string(name), MultipartFile{
            .data = std::move(data),
            .filename = std::move(filename),
            .mime = std::string(mime),
        });
    }
    return Ok(*this);
}

MultipartForm& MultipartForm::file(std::string_view name, std::span<uint8_t const> data, std::string_view filename, std::string_view mime) {
    if (!m_impl->isBuilt()) {
        m_impl->m_files.insert_or_assign(std::string(name), MultipartFile{
            .data = ByteVector(data.begin(), data.end()),
            .filename = std::string(filename),
            .mime = std::string(mime),
        });
    }
    return *this;
}

std::string const& MultipartForm::getBoundary() const {
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
ByteVector WebResponse::data() const {
    return m_impl->m_data;
}
Result<> WebResponse::into(std::filesystem::path const& path) const {
    return m_impl->into(path);
}

std::vector<std::string> WebResponse::headers() const {
    return map::keys(m_impl->m_headers);
}

std::optional<std::string> WebResponse::header(std::string_view name) const {
    if (auto str = std::string(name); m_impl->m_headers.contains(str)) {
        return m_impl->m_headers.at(str).at(0);
    }
    return std::nullopt;
}

std::optional<std::vector<std::string>> WebResponse::getAllHeadersNamed(std::string_view name) const {
    if (auto str = std::string(name); m_impl->m_headers.contains(str)) {
        return m_impl->m_headers.at(str);
    }
    return std::nullopt;
}

std::string const& WebResponse::errorMessage() const {
    return m_impl->m_errMessage;
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
    static std::atomic_size_t s_idCounter;

    std::string m_method;
    std::string m_url;
    std::unordered_map<std::string, std::vector<std::string>> m_headers;
    std::unordered_map<std::string, std::string> m_urlParameters;
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

    Impl() : m_id(s_idCounter++) {}

    WebResponse makeError(int code, std::string const& msg) {
        auto res = WebResponse();
        res.m_impl->m_code = code;
        res.m_impl->m_data = ByteVector(msg.begin(), msg.end());
        return res;
    }
};

std::atomic_size_t WebRequest::Impl::s_idCounter = 0;

WebRequest::WebRequest() : m_impl(std::make_shared<Impl>()) {}
WebRequest::~WebRequest() {}

// Encodes a url param
static std::string urlParamEncode(std::string_view input) {
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

WebTask WebRequest::send(std::string_view method, std::string_view url) {
    m_impl->m_method = method;
    m_impl->m_url = url;
    return WebTask::run([impl = m_impl](auto progress, auto hasBeenCancelled) -> WebTask::Result {
        // Init Curl
        auto curl = curl_easy_init();
        if (!curl) {
            log::error("Failed to initialize cURL");
            return impl->makeError(-1, "Failed to initialize curl");
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
        for (auto& [name, values] : impl->m_headers) {
            // Sanitize header name
            auto header = name;
            header.erase(std::remove_if(header.begin(), header.end(), [](char c) {
                return c == '\r' || c == '\n';
            }), header.end());
            // Append value
            for (const auto& value: values) {
                header += ": " + value;
                headers = curl_slist_append(headers, header.c_str());
            }
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Add parameters to the URL and pass it to curl
        auto url = impl->m_url;
        bool first = url.find('?') == std::string::npos;
        for (auto& [key, value] : impl->m_urlParameters) {
            url += (first ? "?" : "&") + urlParamEncode(key) + "=" + urlParamEncode(value);
            first = false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set HTTP version
        auto useHttp1 = Loader::get()->getLaunchFlag("use-http1");
        if (impl->m_httpVersion == HttpVersion::DEFAULT && useHttp1) {
            impl->m_httpVersion = HttpVersion::VERSION_1_1; // Force HTTP/1.1 if the flag is set
        }

        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, unwrapHttpVersion(impl->m_httpVersion));

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
        } else if (impl->m_method == "POST") {
            // curl_easy_perform would freeze on a POST request with no fields, so set it to an empty string
            // why? god knows
            // SMJS: because the stream isn't complete without a body according to the spec
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        }

        // Cert verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, impl->m_certVerification ? 1L : 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        int sslOptions = 0;

        if (impl->m_certVerification) {
            if (impl->m_CABundleContent.empty()) {
                impl->m_CABundleContent = CA_BUNDLE_CONTENT;
            }

            if (!impl->m_CABundleContent.empty()) {
                curl_blob caBundleBlob = {};
                caBundleBlob.data = reinterpret_cast<void*>(impl->m_CABundleContent.data());
                caBundleBlob.len = impl->m_CABundleContent.size();
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
        curl_easy_setopt(curl, CURLOPT_NOBODY, impl->m_transferBody ? 0L : 1L);

        // Set user agent if provided
        if (impl->m_userAgent) {
            curl_easy_setopt(curl, CURLOPT_USERAGENT, impl->m_userAgent->c_str());
        }

        // Set encoding
        if (impl->m_acceptEncodingType) {
            curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, impl->m_acceptEncodingType->c_str());
        }

        // Set timeout
        if (impl->m_timeout) {
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, impl->m_timeout->count());
        }

        // Set range
        if (impl->m_range) {
            curl_easy_setopt(curl, CURLOPT_RANGE, fmt::format("{}-{}", impl->m_range->first, impl->m_range->second).c_str());
        }

        // Set proxy options
        auto const& proxyOpts = impl->m_proxyOpts;
        if (!proxyOpts.address.empty()) {
            curl_easy_setopt(curl, CURLOPT_PROXY, proxyOpts.address.c_str());

            if (proxyOpts.port.has_value()) {
                curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxyOpts.port.value());
            }

            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, unwrapProxyType(proxyOpts.type));

            if (!proxyOpts.username.empty() || !proxyOpts.username.empty()) {
                curl_easy_setopt(curl, CURLOPT_PROXYAUTH, unwrapHttpAuth(proxyOpts.auth));
                curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD,
                    fmt::format("{}:{}", proxyOpts.username, proxyOpts.password).c_str());
            }

            curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, proxyOpts.tunneling ? 1 : 0);
            curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYPEER, proxyOpts.certVerification ? 1 : 0);
            curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYHOST, 2);
        }

        // Follow request through 3xx responses
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, impl->m_followRedirects ? 1L : 0L);

        // Ignore content length
        curl_easy_setopt(curl, CURLOPT_IGNORE_CONTENT_LENGTH, impl->m_ignoreContentLength ? 1L : 0L);

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
        char errorBuf[CURL_ERROR_SIZE];
        errorBuf[0] = '\0';
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuf);

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
                // Create a new vector and add to it or add to an already existing one
                if (headers.contains(key)) {
                    headers.at(key).push_back(value);
                } else {
                    headers.insert_or_assign(key, std::vector{value});
                }
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

        responseData.response.m_impl->m_errMessage = std::string(errorBuf);

        // Free up curl memory
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        // Check if the request failed on curl's side or because of cancellation
        if (curlResponse != CURLE_OK) {
            if (hasBeenCancelled()) {
                log::debug("Request cancelled");
                return WebTask::Cancel();
            }
            else {
                std::string const err = curl_easy_strerror(curlResponse);
                log::error("cURL failure, error: {}", err);
                log::warn("Error buffer: {}", errorBuf);
                return impl->makeError(
                    -1,
                    !*errorBuf ?
                          fmt::format("Curl failed: {}", err)
                        : fmt::format("Curl failed: {} ({})", err, errorBuf)
                );
            }
        }

        // resolve with success :-)
        return std::move(responseData.response);
    }, fmt::format("{} {}", method, url));
}
WebTask WebRequest::post(std::string_view url) {
    return this->send("POST", url);
}
WebTask WebRequest::get(std::string_view url) {
    return this->send("GET", url);
}
WebTask WebRequest::put(std::string_view url) {
    return this->send("PUT", url);
}
WebTask WebRequest::patch(std::string_view url) {
    return this->send("PATCH", url);
}

WebRequest& WebRequest::header(std::string_view name, std::string_view value) {
    if (name == "User-Agent") {
        userAgent(value);

        return *this;
    } if (name == "Accept-Encoding") {
        acceptEncoding(value);

        return *this;
    } if (name == "Keep-Alive") {
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
    std::string strName = std::string(name);
    std::string strValue = std::string(value);
    if (m_impl->m_headers.contains(strName)) {
        m_impl->m_headers.at(strName).push_back(strValue);
    } else {
        m_impl->m_headers.insert_or_assign(strName, std::vector{strValue});
    }

    return *this;
}

WebRequest& WebRequest::removeHeader(std::string_view name) {
    m_impl->m_headers.erase(std::string(name));
    return *this;
}

WebRequest& WebRequest::param(std::string_view name, std::string_view value) {
    m_impl->m_urlParameters.insert_or_assign(std::string(name), std::string(value));
    return *this;
}

WebRequest& WebRequest::removeParam(std::string_view name) {
    m_impl->m_urlParameters.erase(std::string(name));
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

WebRequest& WebRequest::CABundleContent(std::string_view content) {
    m_impl->m_CABundleContent = content;
    return *this;
}

WebRequest& WebRequest::proxyOpts(ProxyOpts const& proxyOpts) {
    m_impl->m_proxyOpts = proxyOpts;
    return *this;
}

WebRequest& WebRequest::version(HttpVersion httpVersion) {
    m_impl->m_httpVersion = httpVersion;
    return *this;
}

WebRequest& WebRequest::acceptEncoding(std::string_view str) {
    m_impl->m_acceptEncodingType = str;
    return *this;
}

WebRequest& WebRequest::body(ByteVector raw) {
    m_impl->m_body = raw;
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

std::string WebRequest::getMethod() const {
    return m_impl->m_method;
}

std::string WebRequest::getUrl() const {
    return m_impl->m_url;
}

std::unordered_map<std::string, std::vector<std::string>> WebRequest::getHeaders() const {
    return m_impl->m_headers;
}

std::unordered_map<std::string, std::string> WebRequest::getUrlParams() const {
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