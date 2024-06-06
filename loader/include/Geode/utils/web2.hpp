#pragma once

#include <Geode/loader/Loader.hpp> // another great circular dependency fix
#include <matjson.hpp>
#include "Result.hpp"
#include "Task.hpp"
#include <chrono>
#include <optional>

namespace geode::utils::web {
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
        
        std::vector<std::string> headers() const;
        std::optional<std::string> header(std::string_view name) const;
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
        WebRequest& param(std::string_view name, std::string_view value);

        template <std::integral T>
        WebRequest& param(std::string_view name, T value) {
            return this->param(name, std::to_string(value));
        }

        WebRequest& userAgent(std::string_view name);

        WebRequest& timeout(std::chrono::seconds time);

        WebRequest& certVerification(bool enabled);
        WebRequest& CABundleContent(std::string_view content);
        WebRequest& proxyOpts(ProxyOpts const& proxyOpts);

        WebRequest& body(ByteVector raw);
        WebRequest& bodyString(std::string_view str);
        WebRequest& bodyJSON(matjson::Value const& json);
    };
}
