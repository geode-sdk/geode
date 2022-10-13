#pragma once

#include "../DefaultInclude.hpp"
#include <fs/filesystem.hpp>
#include "Result.hpp"
#include "json.hpp"
#include <mutex>

namespace geode::utils::web {
    using FileProgressCallback = std::function<bool(double, double)>;

    /**
     * Synchronously fetch data from the internet
     * @param url URL to fetch
     * @returns Returned data as bytes, or error on error
     */
    GEODE_DLL Result<byte_array> fetchBytes(std::string const& url);

    /**
     * Synchronously fetch data from the internet
     * @param url URL to fetch
     * @returns Returned data as string, or error on error
     */
    GEODE_DLL Result<std::string> fetch(std::string const& url);

    /**
     * Syncronously download a file from the internet
     * @param url URL to fetch
     * @param into Path to download file into
     * @param prog Progress function; first parameter is bytes downloaded so 
     * far, and second is total bytes to download. Return true to continue 
     * downloading, and false to interrupt. Note that interrupting does not 
     * automatically remove the file that was being downloaded
     * @returns Returned data as JSON, or error on error
     */
    GEODE_DLL Result<> fetchFile(
        std::string const& url,
        ghc::filesystem::path const& into,
        FileProgressCallback prog = nullptr
    );

    /**
     * Synchronously fetch data from the internet and parse it as JSON
     * @param url URL to fetch
     * @returns Returned data as JSON, or error on error
     */
    template<class Json = nlohmann::json>
    Result<Json> fetchJSON(std::string const& url) {
        auto res = fetch(url);
        if (!res) return Err(res.error());
        try {
            return Ok(Json::parse(res.value()));
        } catch(std::exception& e) {
            return Err(e.what());
        }
    }

    class SentAsyncWebRequest;
    template<class T>
    class AsyncWebResult;
    class AsyncWebResponse;
    class AsyncWebRequest;

    using AsyncProgress  = std::function<void(SentAsyncWebRequest&, double, double)>;
    using AsyncExpect    = std::function<void(std::string const&)>;
    using AsyncThen      = std::function<void(SentAsyncWebRequest&, byte_array const&)>;
    using AsyncCancelled = std::function<void(SentAsyncWebRequest&)>;

    class SentAsyncWebRequest {
    private:
        std::string m_id;
        std::string m_url;
        std::vector<AsyncThen> m_thens;
        std::vector<AsyncExpect> m_expects;
        std::vector<AsyncProgress> m_progresses;
        std::vector<AsyncCancelled> m_cancelleds;
        std::atomic<bool> m_paused = true;
        std::atomic<bool> m_cancelled = false;
        std::atomic<bool> m_finished = false;
        std::atomic<bool> m_cleanedUp = false;
        mutable std::mutex m_mutex;
        std::variant<
            std::monostate,
            std::ostream*,
            ghc::filesystem::path
        > m_target = std::monostate();

        template<class T>
        friend class AsyncWebResult;
        friend class AsyncWebRequest;

        void pause();
        void resume();
        void error(std::string const& error);
        void doCancel();

    public:
        SentAsyncWebRequest(AsyncWebRequest const&, std::string const& id);

        void cancel();
        bool finished() const;
    };

    using SentAsyncWebRequestHandle = std::shared_ptr<SentAsyncWebRequest>;

    template<class T>
    using DataConverter = Result<T>(*)(byte_array const&);

    class GEODE_DLL AsyncWebRequest {
    private:
        std::optional<std::string> m_joinID;
        std::string m_url;
        AsyncThen m_then = nullptr;
        AsyncExpect m_expect = nullptr;
        AsyncProgress m_progress = nullptr;
        AsyncCancelled m_cancelled = nullptr;
        bool m_sent = false;
        std::variant<
            std::monostate,
            std::ostream*,
            ghc::filesystem::path
        > m_target;

        template<class T>
        friend class AsyncWebResult;
        friend class SentAsyncWebRequest;
        friend class AsyncWebResponse;

    public:
        AsyncWebRequest& join(std::string const& requestID);
        AsyncWebResponse fetch(std::string const& url);
        AsyncWebRequest& expect(AsyncExpect handler);
        AsyncWebRequest& progress(AsyncProgress progressFunc);
        // Web requests may be cancelled after they are finished (for example, 
        // if downloading files in bulk and one fails). In that case, handle 
        // freeing up the results of `then` here
        AsyncWebRequest& cancelled(AsyncCancelled cancelledFunc);
        SentAsyncWebRequestHandle send();
        ~AsyncWebRequest();
    };

    template<class T>
    class AsyncWebResult {
    private:
        AsyncWebRequest& m_request;
        DataConverter<T> m_converter;

        AsyncWebResult(AsyncWebRequest& request, DataConverter<T> converter)
         : m_request(request), m_converter(converter) {}
        
        friend class AsyncWebResponse;

    public:
        AsyncWebRequest& then(std::function<void(T)> handle);
        AsyncWebRequest& then(std::function<void(SentAsyncWebRequest&, T)> handle);
    };

    class GEODE_DLL AsyncWebResponse {
    private:
        AsyncWebRequest& m_request;

        inline AsyncWebResponse(AsyncWebRequest& request) : m_request(request) {}

        friend class AsyncWebRequest;

    public:
        // Make sure the stream lives for the entire duration of the request.
        AsyncWebResult<std::monostate> into(std::ostream& stream);
        AsyncWebResult<std::monostate> into(ghc::filesystem::path const& path);
        AsyncWebResult<std::string> text();
        AsyncWebResult<byte_array> bytes();
        AsyncWebResult<nlohmann::json> json();

        template<class T>
        AsyncWebResult<T> as(DataConverter<T> converter) {
            return AsyncWebResult(m_request, converter);
        }
    };
    
    template<class T>
    AsyncWebRequest& AsyncWebResult<T>::then(std::function<void(T)> handle) {
        m_request.m_then = [
            converter = m_converter,
            handle
        ](SentAsyncWebRequest& req, byte_array const& arr) {
            auto conv = converter(arr);
            if (conv) {
                handle(conv.value());
            } else {
                req.error("Unable to convert value: " + conv.error());
            }
        };
        return m_request;
    }

    template<class T>
    AsyncWebRequest& AsyncWebResult<T>::then(std::function<void(SentAsyncWebRequest&, T)> handle) {
        m_request.m_then = [
            converter = m_converter,
            handle
        ](SentAsyncWebRequest& req, byte_array const& arr) {
            auto conv = converter(arr);
            if (conv) {
                handle(req, conv.value());
            } else {
                req.error("Unable to convert value: " + conv.error());
            }
        };
        return m_request;
    }
}

