#pragma once

#include <Geode/loader/Loader.hpp> // another great circular dependency fix
#include <matjson.hpp>
#include <Geode/Result.hpp>
#include "Task.hpp"
#include <chrono>
#include <optional>

namespace geode::utils::web {
    GEODE_DLL void openLinkInBrowser(std::string const& url);
    
    // https://curl.se/libcurl/c/CURLOPT_HTTPAUTH.html
    namespace http_auth {
        constexpr static long BASIC = 0x0001;
        constexpr static long DIGEST = 0x0002;
        constexpr static long DIGEST_IE = 0x0004;
        constexpr static long BEARER = 0x0008;
        constexpr static long NEGOTIATE = 0x0010;
        constexpr static long NTLM = 0x0020;
        constexpr static long NTLM_WB = 0x0040;
        constexpr static long ANY = 0x0080;
        constexpr static long ANYSAFE = 0x0100;
        constexpr static long ONLY = 0x0200;
        constexpr static long AWS_SIGV4 = 0x0400;
    }

    // https://curl.se/libcurl/c/CURLOPT_HTTP_VERSION.html
    enum class HttpVersion {
        DEFAULT,
        VERSION_1_0,
        VERSION_1_1,
        VERSION_2_0,
        VERSION_2TLS,
        VERSION_2_PRIOR_KNOWLEDGE,
        VERSION_3 = 30,
        VERSION_3ONLY = 31
    };

    // https://curl.se/libcurl/c/CURLOPT_PROXYTYPE.html
    enum class ProxyType {
        HTTP, // HTTP
        HTTPS, // HTTPS
        HTTPS2, // HTTPS (attempt to use HTTP/2)
        SOCKS4, // Socks4
        SOCKS4A, // Socks4 with hostname resolution
        SOCKS5, // Socks5
        SOCKS5H, // Socks5 with hostname resolution
    };

    struct ProxyOpts {
        std::string address; // Proxy address/FQDN
        std::optional<std::uint16_t> port; // Proxy port
        ProxyType type = ProxyType::HTTP; // Proxy type
        long auth = http_auth::BASIC; // HTTP proxy auth method
        std::string username; // Proxy username
        std::string password; // Proxy password
        bool tunneling = false; // Enable HTTP tunneling
        bool certVerification = true; // Enable HTTPS certificate verification
    };

    class WebRequest;

    class GEODE_DLL WebResponse final {
    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;

        friend class WebRequest;

    public:
        // Must be default-constructible for use in Promise
        WebResponse();

        bool ok() const;
        int code() const;

        Result<std::string> string() const;
        Result<matjson::Value> json() const;
        ByteVector data() const;
        Result<> into(std::filesystem::path const& path) const;

        std::vector<std::string> headers() const;
        std::optional<std::string> header(std::string_view name) const;

        /**
         * Retrieves a list of all headers from the response with a given name - there can be
         * multiple headers with the same name, such as Set-Cookie, with each cookie in a separate
         * header
         * @param name name of the header
         * @return std::optional<std::vector<std::string>>
         */
        std::optional<std::vector<std::string>> getAllHeadersNamed(std::string_view name) const;

        /**
         * Returns additional error information, in case the request failed.
         * In case the request did not fail, or no more information is available beyond what `string` returns,
         * an empty string is returned.
         */
        std::string const& errorMessage() const;
    };

    class GEODE_DLL WebProgress final {
    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;

        friend class WebRequest;
    
    public:
        // Must be default-constructible for use in Promise
        WebProgress();

        size_t downloaded() const;
        size_t downloadTotal() const;
        std::optional<float> downloadProgress() const;

        size_t uploaded() const;
        size_t uploadTotal() const;
        std::optional<float> uploadProgress() const;
    };

    using WebTask = Task<WebResponse, WebProgress>;

    class GEODE_DLL WebRequest final {
    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;
    public:
        WebRequest();
        ~WebRequest();

        WebTask send(std::string_view method, std::string_view url);
        WebTask post(std::string_view url);
        WebTask get(std::string_view url);
        WebTask put(std::string_view url);
        WebTask patch(std::string_view url);

        WebRequest& header(std::string_view name, std::string_view value);
        WebRequest& removeHeader(std::string_view name);
        WebRequest& param(std::string_view name, std::string_view value);
        template <std::integral T>
        WebRequest& param(std::string_view name, T value) {
            return this->param(name, std::to_string(value));
        }
        WebRequest& removeParam(std::string_view name);

        /**
         * Sets the request's user agent.
         * Defaults to not sending the User-Agent: header.
         *
         * @param name
         * @return WebRequest&
         */
        WebRequest& userAgent(std::string_view name);

        /**
         * Sets the response's encoding. Valid values include: br, gzip, deflate, ...
         * You can set multiple encoding types by calling this method with a comma separated list
         * of the encodings of your choosing.
         * Defaults to not sending an Accept-Encoding: header, and in turn, does not decompress received contents automatically.
         *
         * @example
         * auto req = web::WebRequest()
         *  .acceptEncoding("gzip, deflate")
         *  .get(url);
         *
         * @param encodingType Target response encoding type. An empty string ("") will use all built-in supported encodings.
         * @return WebRequest&
         */
        WebRequest& acceptEncoding(std::string_view encodingType);

        /**
         * Sets the maximum amount of seconds to allow the entire transfer operation to take.
         * The default timeout is 0, which means the request never times out during transfer.
         *
         * @param time
         * @return WebRequest&
         */
        WebRequest& timeout(std::chrono::seconds time);

        /**
         * Sets the target byte range to request.
         * Defaults to receiving the full request.
         *
         * @param byteRange a pair of ints, first value is what byte to start from, second value is the last byte to get (both inclusive)
         * @return WebRequest&
         */
        WebRequest& downloadRange(std::pair<std::uint64_t, std::uint64_t> byteRange);

        /**
         * Enable or disables peer verification in SSL handshake.
         * The default is true.
         *
         * @param enabled
         * @return WebRequest&
         */
        WebRequest& certVerification(bool enabled);

        /**
         * Enables or disabled getting the body of a request. For HTTP(S), this does a HEAD request.
         * For most other protocols it means just not asking to transfer the body data. 
         * The default is true.
         *
         * @param enabled
         * @return WebRequest&
         */
        WebRequest& transferBody(bool enabled);

        /**
         * Follow HTTP 3xx redirects.
         * The default is true.
         *
         * @param enabled
         * @return WebRequest&
         */
        WebRequest& followRedirects(bool enabled);

        /**
         * Enables or disables ignoring the content length header.
         * The default is false.
         *
         * @param enabled
         * @return WebRequest&
         */
        WebRequest& ignoreContentLength(bool enabled);

        /**
         * Sets the Certificate Authority (CA) bundle content.
         * Defaults to not sending a CA bundle.
         *
         * @param content
         * @return WebRequest&
         */
        WebRequest& CABundleContent(std::string_view content);

        /**
         * Sets the request's proxy.
         * Defaults to not using a proxy.
         *
         * @param proxyOpts
         * @return WebRequest&
         */
        WebRequest& proxyOpts(ProxyOpts const& proxyOpts);

        /**
         * Sets the request's HTTP version.
         * The default is HttpVersion::VERSION_2TLS.
         *
         * @param httpVersion
         * @return WebRequest&
         */
        WebRequest& version(HttpVersion httpVersion);

        /**
         * Sets the body of the request to a byte vector.
         *
         * @param raw The raw bytes to set as the body.
         * @return WebRequest&
         */
        WebRequest& body(ByteVector raw);
        /**
         * Sets the body of the request to a string.
         *
         * @param str The string to set as the body.
         * @return WebRequest&
         */
        WebRequest& bodyString(std::string_view str);
        /**
         * Sets the body of the request to a json object.
         *
         * @param json
         * @return WebRequest&
         */
        WebRequest& bodyJSON(matjson::Value const& json);

        /**
         * Gets the unique request ID
         *
         * @return size_t
         */
        size_t getID() const;


        /**
         * Gets the request method as a string
         *
         * @return std::string
         */
        std::string getMethod() const;

        /**
         * Gets the request URL
         *
         * @return std::string
         */
        std::string getUrl() const;

        /**
         * Gets the request headers
         *
         * @return std::unordered_map<std::string, std::vector<std::string>>
         */
        std::unordered_map<std::string, std::vector<std::string>> getHeaders() const;

        /**
         * Gets the parameters inside the URL
         *
         * @return std::unordered_map<std::string, std::string>
         */
        std::unordered_map<std::string, std::string> getUrlParams() const;

        /**
         * Gets the post body stream
         *
         * @return std::optional<ByteVector>
         */
        std::optional<ByteVector> getBody() const;

        /**
         * Gets the request timeout in seconds
         *
         * @return std::optional<std::chrono::seconds>
         */
        std::optional<std::chrono::seconds> getTimeout() const;

        /**
         * Gets HTTP versions applied to the request
         *
         * @return HttpVersion
         */
        HttpVersion getHttpVersion() const;
    };
}
