#pragma once

#include <matjson.hpp>
#include "Result.hpp"
#include "Task.hpp"
#include <chrono>

namespace geode::utils::web {
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
        WebRequest& userAgent(std::string_view name);

        WebRequest& timeout(std::chrono::seconds time);

        WebRequest& body(ByteVector raw);
        WebRequest& bodyString(std::string_view str);
        WebRequest& bodyJSON(matjson::Value const& json);
    };
}
