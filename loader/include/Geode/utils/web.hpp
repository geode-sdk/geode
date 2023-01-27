#pragma once

#include "../DefaultInclude.hpp"
#include <json.hpp>
#include "Result.hpp"
#include "general.hpp"

#include <ghc/filesystem.hpp>
#include <mutex>

namespace geode::utils::web {
    GEODE_DLL void openLinkInBrowser(std::string const& url);

    using FileProgressCallback = std::function<bool(double, double)>;

    /**
     * Synchronously fetch data from the internet
     * @param url URL to fetch
     * @returns Returned data as bytes, or error on error
     */
    GEODE_DLL Result<ByteVector> fetchBytes(std::string const& url);

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
        std::string const& url, ghc::filesystem::path const& into, FileProgressCallback prog = nullptr
    );

    /**
     * Synchronously fetch data from the internet and parse it as JSON
     * @param url URL to fetch
     * @returns Returned data as JSON, or error on error
     */
    Result<json::Value> fetchJSON(std::string const& url);

    class SentAsyncWebRequest;
    template <class T>
    class AsyncWebResult;
    class AsyncWebResponse;
    class AsyncWebRequest;

    using AsyncProgress = std::function<void(SentAsyncWebRequest&, double, double)>;
    using AsyncExpect = std::function<void(std::string const&)>;
    using AsyncExpectCode = std::function<void(std::string const&, int)>;
    using AsyncThen = std::function<void(SentAsyncWebRequest&, ByteVector const&)>;
    using AsyncCancelled = std::function<void(SentAsyncWebRequest&)>;

    /**
     * A handle to an in-progress sent asynchronous web request. Use this to
     * cancel the request / query information about it
     */
    class SentAsyncWebRequest {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

        template <class T>
        friend class AsyncWebResult;
        friend class AsyncWebRequest;

        void pause();
        void resume();
        void error(std::string const& error, int code);
        void doCancel();

    public:
        /**
         * Do not call these manually.
         */
        SentAsyncWebRequest();
        ~SentAsyncWebRequest();
        static std::shared_ptr<SentAsyncWebRequest> create(AsyncWebRequest const&, std::string const& id);

        /**
         * Cancel the request. Cleans up any downloaded files, but if you run
         * extra code in `then`, you will have to clean it up manually in
         * `cancelled`
         */
        void cancel();
        /**
         * Check if the request is finished
         */
        bool finished() const;
    };

    using SentAsyncWebRequestHandle = std::shared_ptr<SentAsyncWebRequest>;

    template <class T>
    using DataConverter = Result<T> (*)(ByteVector const&);

    /**
     * An asynchronous, thread-safe web request. Downloads data from the
     * internet without slowing the main thread. All callbacks are run in the
     * GD thread, so interacting with the Cocos2d UI is perfectly safe
     */
    class GEODE_DLL AsyncWebRequest {
    private:
        std::optional<std::string> m_joinID;
        std::string m_url;
        AsyncThen m_then = nullptr;
        AsyncExpectCode m_expect = nullptr;
        AsyncProgress m_progress = nullptr;
        AsyncCancelled m_cancelled = nullptr;
        bool m_sent = false;
        std::variant<std::monostate, std::ostream*, ghc::filesystem::path> m_target;
        std::vector<std::string> m_httpHeaders;

        template <class T>
        friend class AsyncWebResult;
        friend class SentAsyncWebRequest;
        friend class AsyncWebResponse;

    public:
        /**
         * An asynchronous, thread-safe web request. Downloads data from the
         * internet without slowing the main thread. All callbacks are run in the
         * GD thread, so interacting with the Cocos2d UI is perfectly safe
         */
        AsyncWebRequest() = default;

        /**
         * If you only want one instance of this web request to run (for example,
         * you're downloading some global data for a manager), then use this
         * to specify a Join ID. If another request with the same ID is
         * already running, this request's callbacks will be appended to the
         * existing one instead of creating a new request
         * @param requestID The Join ID of the request. Can be anything,
         * recommended to be something unique
         * @returns Same AsyncWebRequest
         */
        AsyncWebRequest& join(std::string const& requestID);

        /**
         * In order to specify a http header to the request, give it here.
         * Can be called more than once.
         */
        AsyncWebRequest& header(std::string const& header);
        /**
         * URL to fetch from the internet asynchronously
         * @param url URL of the data to download. Redirects will be
         * automatically followed
         * @returns Same AsyncWebRequest
         */
        AsyncWebResponse fetch(std::string const& url);
        /**
         * Specify a callback to run if the download fails. The callback is
         * always ran in the GD thread, so interacting with UI is safe
         * @param handler Callback to run if the download fails
         * @returns Same AsyncWebRequest
         */
        AsyncWebRequest& expect(AsyncExpect handler);
        /**
         * Specify a callback to run if the download fails. The callback is
         * always ran in the GD thread, so interacting with UI is safe
         * @param handler Callback to run if the download fails
         * @returns Same AsyncWebRequest
         */
        AsyncWebRequest& expect(AsyncExpectCode handler);
        /**
         * Specify a callback to run when the download progresses. The callback is
         * always ran in the GD thread, so interacting with UI is safe
         * @param handler Callback to run when the download progresses
         * @returns Same AsyncWebRequest
         */
        AsyncWebRequest& progress(AsyncProgress handler);
        /**
         * Specify a callback to run if the download is cancelled. The callback is
         * always ran in the GD thread, so interacting with UI is safe. Web 
         * requests may be cancelled after they are finished (for example, if 
         * downloading files in bulk and one fails). In that case, handle 
         * freeing up the results of `then` in this handler
         * @param handler Callback to run if the download is cancelled
         * @returns Same AsyncWebRequest
         */
        AsyncWebRequest& cancelled(AsyncCancelled handler);
        /**
         * Begin the web request. It's not always necessary to call this as the
         * destructor calls it automatically, but if you need access to the
         * handle of the sent request, use this
         * @returns Handle to the sent web request
         */
        SentAsyncWebRequestHandle send();
        ~AsyncWebRequest();
    };

    template <class T>
    class AsyncWebResult {
    private:
        AsyncWebRequest& m_request;
        DataConverter<T> m_converter;

        AsyncWebResult(AsyncWebRequest& request, DataConverter<T> converter) :
            m_request(request), m_converter(converter) {}

        friend class AsyncWebResponse;

    public:
        /**
         * Specify a callback to run after a download is finished. Runs in the
         * GD thread, so interacting with UI is safe
         * @param handle Callback to run
         * @returns The original AsyncWebRequest, where you can specify more
         * aspects about the request like failure and progress callbacks
         */
        AsyncWebRequest& then(std::function<void(T)> handle);
        /**
         * Specify a callback to run after a download is finished. Runs in the
         * GD thread, so interacting with UI is safe
         * @param handle Callback to run
         * @returns The original AsyncWebRequest, where you can specify more
         * aspects about the request like failure and progress callbacks
         */
        AsyncWebRequest& then(std::function<void(SentAsyncWebRequest&, T)> handle);
    };

    class GEODE_DLL AsyncWebResponse {
    private:
        AsyncWebRequest& m_request;

        inline AsyncWebResponse(AsyncWebRequest& request) : m_request(request) {}

        friend class AsyncWebRequest;

    public:
        /**
         * Download into a stream. Make sure the stream lives for the entire
         * duration of the request. If you want to download a file, use the
         * `ghc::filesystem::path` overload of `into` instead
         * @param stream Stream to download into. Make sure it lives long
         * enough, otherwise the web request will crash
         * @returns AsyncWebResult, where you can specify the `then` action for
         * after the download is finished. The result has a `std::monostate`
         * template parameter, as it can be assumed you know what you passed
         * into `into`
         */
        AsyncWebResult<std::monostate> into(std::ostream& stream);
        /**
         * Download into a file
         * @param path File to download into. If it already exists, it will
         * be overwritten.
         * @returns AsyncWebResult, where you can specify the `then` action for
         * after the download is finished. The result has a `std::monostate`
         * template parameter, as it can be assumed you know what you passed
         * into `into`
         */
        AsyncWebResult<std::monostate> into(ghc::filesystem::path const& path);
        /**
         * Download into memory as a string
         * @returns AsyncWebResult, where you can specify the `then` action for
         * after the download is finished
         */
        AsyncWebResult<std::string> text();
        /**
         * Download into memory as a byte array
         * @returns AsyncWebResult, where you can specify the `then` action for
         * after the download is finished
         */
        AsyncWebResult<ByteVector> bytes();
        /**
         * Download into memory as JSON
         * @returns AsyncWebResult, where you can specify the `then` action for
         * after the download is finished
         */
        AsyncWebResult<json::Value> json();

        /**
         * Download into memory as a custom type. The data will first be
         * downloaded into memory as a byte array, and then converted using
         * the specified converter function
         * @param converter Function that converts the data from a byte array
         * to the desired type
         * @returns AsyncWebResult, where you can specify the `then` action for
         * after the download is finished
         */
        template <class T>
        AsyncWebResult<T> as(DataConverter<T> converter) {
            return AsyncWebResult(m_request, converter);
        }
    };

    template <class T>
    AsyncWebRequest& AsyncWebResult<T>::then(std::function<void(T)> handle) {
        m_request.m_then = [converter = m_converter,
                            handle](SentAsyncWebRequest& req, ByteVector const& arr) {
            auto conv = converter(arr);
            if (conv) {
                handle(conv.unwrap());
            }
            else {
                req.error("Unable to convert value: " + conv.unwrapErr(), -1);
            }
        };
        return m_request;
    }

    template <class T>
    AsyncWebRequest& AsyncWebResult<T>::then(std::function<void(SentAsyncWebRequest&, T)> handle) {
        m_request.m_then = [converter = m_converter,
                            handle](SentAsyncWebRequest& req, ByteVector const& arr) {
            auto conv = converter(arr);
            if (conv) {
                handle(req, conv.value());
            }
            else {
                req.error("Unable to convert value: " + conv.error(), -1);
            }
        };
        return m_request;
    }
}
