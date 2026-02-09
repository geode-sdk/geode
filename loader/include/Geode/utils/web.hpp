#pragma once

#include <Geode/loader/Loader.hpp> // another great circular dependency fix
#include <Geode/utils/ZStringView.hpp>
#include <Geode/utils/StringMap.hpp>
#include <Geode/utils/async.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/loader/Event.hpp>
#include <arc/sync/oneshot.hpp>
#include <matjson.hpp>
#include <Geode/Result.hpp>
#include <chrono>
#include <optional>
#include <string_view>
#include <span>

namespace geode::utils::web {
    GEODE_DLL void openLinkInBrowser(ZStringView url);

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

    enum class GeodeWebError {
        CURL_INITIALIZATION_ERROR = -999,
        REQUEST_CANCELLED = -998,
        QUEUE_FULL = -997,
        CHANNEL_CLOSED = -996
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

    /// Represents a multipart-form object to be sent in a `WebRequest`.
    ///
    /// @example
    /// web::MultipartForm form;
    /// form.param("key", "value");
    /// form.param("key2", "value2");
    /// form.file("file", { 0xAA, 0xBB, 0xCC }, "raw.bin");
    /// form.file("file2", "path/to/image.png", "screenshot.png", "image/png");
    ///
    /// auto req = web::WebRequest()
    ///     .bodyMultipart(form)
    ///     .get(url);
    class GEODE_DLL MultipartForm final {
    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;

    public:
        MultipartForm();
        ~MultipartForm();

        MultipartForm& param(std::string name, std::string value);
        template <std::integral T>
        MultipartForm& param(std::string name, T value) {
            return this->param(std::move(name), fmt::to_string(value));
        }

        MultipartForm& file(std::string name, std::span<uint8_t const> data, std::string filename, std::string mime = "application/octet-stream");
        Result<MultipartForm&> file(std::string name, std::filesystem::path const& path, std::string mime = "application/octet-stream");

        /**
         * Returns the unique boundary string used in the multipart form.
         * This will also finalize the form, so adding more parameters will not work.
         * @return std::string_view
         */
        std::string_view getBoundary() const;

        /**
         * Returns the value for the Content-Type header with unique boundary string.
         * This will also finalize the form, so adding more parameters will not work.
         * @return std::string
         */
        std::string getHeader() const;

        /**
         * Returns merged body of all parameters and files, with the correct boundary.
         * This will also finalize the form, so adding more parameters will not work.
         * @return ByteVector
         */
        ByteVector getBody() const;
    };

    class WebRequest;

    class GEODE_DLL WebResponse final {
    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;

        friend class WebRequest;
        friend class WebRequestsManager;

    public:
        // Must be default-constructible for use in Promise
        WebResponse();

        bool info() const;
        bool ok() const;
        bool redirected() const;
        bool badClient() const;
        bool badServer() const;
        bool error() const;
        bool cancelled() const;
        int code() const;

        Result<std::string> string() const;
        Result<matjson::Value> json() const;
        ByteVector const& data() const&;
        ByteVector data() &&;
        Result<> into(std::filesystem::path const& path) const;

        std::vector<std::string> headers() const;
        std::optional<ZStringView> header(std::string_view name) const;

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
        std::string_view errorMessage() const;
    };

    class WebProgress final {
    private:
        size_t m_downloadCurrent = 0;
        size_t m_downloadTotal = 0;
        size_t m_uploadCurrent = 0;
        size_t m_uploadTotal = 0;

        friend class WebRequest;

    public:
        // Must be default-constructible for use in Promise
        WebProgress() = default;

        size_t downloaded() const { return m_downloadCurrent; }
        size_t downloadTotal() const { return m_downloadTotal; }
        std::optional<float> downloadProgress() const {
            return downloadTotal() > 0 ? std::optional(downloaded() * 100.f / downloadTotal()) : std::nullopt;
        }

        size_t uploaded() const { return m_uploadCurrent; }
        size_t uploadTotal() const { return m_uploadTotal; }
        std::optional<float> uploadProgress() const {
            return uploadTotal() > 0 ? std::optional(uploaded() * 100.f / uploadTotal()) : std::nullopt;
        }
    };

    struct WebFuture;

    class GEODE_DLL WebRequest final {
    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;

        friend class WebRequestsManager;
        friend struct WebFuture;
    public:
        WebRequest();
        ~WebRequest();

        WebFuture send(std::string method, std::string url, Mod* mod = geode::getMod());
        WebFuture post(std::string url, Mod* mod = geode::getMod());
        WebFuture get(std::string url, Mod* mod = geode::getMod());
        WebFuture put(std::string url, Mod* mod = geode::getMod());
        WebFuture patch(std::string url, Mod* mod = geode::getMod());

        WebResponse sendSync(std::string method, std::string url, Mod* mod = geode::getMod());
        WebResponse postSync(std::string url, Mod* mod = geode::getMod());
        WebResponse getSync(std::string url, Mod* mod = geode::getMod());
        WebResponse putSync(std::string url, Mod* mod = geode::getMod());
        WebResponse patchSync(std::string url, Mod* mod = geode::getMod());

        WebRequest& header(std::string name, std::string value);
        WebRequest& removeHeader(std::string_view name);
        WebRequest& param(std::string name, std::string value);
        template <std::integral T>
        WebRequest& param(std::string name, T value) {
            return this->param(std::move(name), fmt::to_string(value));
        }
        WebRequest& removeParam(std::string_view name);

        /**
         * Sets the request's method.
         * Overwritten unless set in an interceptor.
         * Generally speaking use get/post/etc. instead.
         *
         * @param method
         * @return WebRequest&
         */
        WebRequest& method(std::string method);

        /**
         * Sets the request's URL.
         * Overwritten unless set in an interceptor.
         * Generally speaking use get/post/etc. instead.
         *
         * @param url
         * @return WebRequest&
         */
        WebRequest& url(std::string url);

        /**
         * Sets the request's user agent.
         * Defaults to not sending the User-Agent: header.
         *
         * @param name
         * @return WebRequest&
         */
        WebRequest& userAgent(std::string name);

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
        WebRequest& acceptEncoding(std::string encodingType);

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
         * Defaults to sending the Geode CA bundle, found here: https://github.com/geode-sdk/net_libs/blob/main/ca_bundle.h
         *
         * @param content
         * @return WebRequest&
         */
        WebRequest& CABundleContent(std::string content);

        /**
         * Sets the request's proxy.
         * Defaults to not using a proxy.
         *
         * @param proxyOpts
         * @return WebRequest&
         */
        WebRequest& proxyOpts(ProxyOpts proxyOpts);

        /**
         * Sets the request's HTTP version.
         * The default is `HttpVersion::DEFAULT`.
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
         * Sets the body of the request to a multipart form.
         *
         * @param form The multipart form to set as the body.
         * @return WebRequest&
         */
        WebRequest& bodyMultipart(MultipartForm const& form);

        /**
         * Sets the function that will be called when progress is made on the request.
         * This is an alternative to manually polling it via `getProgress()`.
         */
        WebRequest& onProgress(Function<void(WebProgress const&)> callback);

        /**
         * Gets the unique request ID
         *
         * @return size_t
         */
        size_t getID() const;

        /**
         * Gets the mod which owns the request.
         *
         * @return geode::Mod*
         */
        Mod* getMod() const;

        /**
         * Gets the request method as a string
         *
         * @return std::string
         */
        ZStringView getMethod() const;

        /**
         * Gets the request URL
         *
         * @return std::string
         */
        ZStringView getUrl() const;

        /**
         * Gets the request headers
         *
         * @return std::unordered_map<std::string, std::vector<std::string>>
         */
        utils::StringMap<std::vector<std::string>> const& getHeaders() const;

        /**
         * Gets the parameters inside the URL
         *
         * @return std::unordered_map<std::string, std::string>
         */
        utils::StringMap<std::string> const& getUrlParams() const;

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

        /**
         * Gets the current progress of the request, if it was sent.
         * Otherwise, default values are returned.
         */
        WebProgress getProgress() const;
    };

    struct GEODE_DLL ARC_NODISCARD WebFuture : arc::Pollable<WebFuture, WebResponse> {
        explicit WebFuture(std::shared_ptr<WebRequest::Impl> request);
        ~WebFuture();

        WebFuture(WebFuture&&) noexcept = default;
        WebFuture& operator=(WebFuture&&) noexcept = delete;
        WebFuture(WebFuture const&) = delete;
        WebFuture& operator=(WebFuture const&) = delete;

        std::optional<WebResponse> poll(arc::Context& cx);

    private:
        struct Impl;
        std::shared_ptr<Impl> m_impl;
    };

    /**
     * Allows you to intercept and modify requests before they're sent with either a mod ID filter or globally.
     *
     * @example
     * WebRequestInterceptEvent(Mod::get()->getID()).listen([](auto& req) { return ListenerResult::Propagate; }, Priority::Normal);
     * WebRequestInterceptEvent().listen([](auto id, auto& req) { return ListenerResult::Stop; }, Priority::VeryEarly);
     */
    struct WebRequestInterceptEvent : ThreadSafeGlobalEvent<WebRequestInterceptEvent, bool(std::string_view, WebRequest&), bool(WebRequest&), std::string> {
        using ThreadSafeGlobalEvent::ThreadSafeGlobalEvent;
    };

    /**
     * Allows you to intercept and modify requests before they're sent using the request identifier.
     *
     * @example
     * IDBasedWebRequestInterceptEvent(0).listen([](auto id, auto& req) { return ListenerResult::Propagate; }, Priority::Normal);
     */
    struct IDBasedWebRequestInterceptEvent : ThreadSafeEvent<IDBasedWebRequestInterceptEvent, bool(size_t, WebRequest&), size_t> {
        using ThreadSafeEvent::ThreadSafeEvent;
    };

    /**
     * Allows you to listen for responses after it was received with either a mod ID filter or globally.
     *
     * @example
     * WebResponseEvent(Mod::get()->getID()).listen([](auto const& res) { return ListenerResult::Propagate; }, Priority::Normal);
     * WebResponseEvent().listen([](auto id, auto const& res) { return ListenerResult::Stop; }, Priority::VeryEarly);
     */
    struct WebResponseEvent : ThreadSafeGlobalEvent<WebResponseEvent, bool(std::string_view, WebResponse const&), bool(WebResponse const&), std::string> {
        using ThreadSafeGlobalEvent::ThreadSafeGlobalEvent;
    };

    /**
     * Allows you to listen for responses after it was received using the request identifier.
     *
     * @example
     * IDBasedWebResponseEvent(0).listen([](auto id, auto const& res) { return ListenerResult::Propagate; }, Priority::Normal);
     */
    struct IDBasedWebResponseEvent : ThreadSafeEvent<IDBasedWebResponseEvent, bool(size_t, WebResponse const&), size_t> {
        using ThreadSafeEvent::ThreadSafeEvent;
    };
}
