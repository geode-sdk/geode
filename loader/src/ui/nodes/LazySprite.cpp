#include <Geode/ui/LazySprite.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/file.hpp>

#include <thread>
#include <queue>
#include <condition_variable>

using namespace geode::prelude;

namespace {

// mini threadpool type thing
class Manager {
public:
    using Task = std::function<void()>;

    static Manager& get() {
        static Manager instance;
        return instance;
    }

    Manager() {}

    ~Manager() {
        this->stopThreads();
    }

    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;
    Manager(Manager&&) = delete;
    Manager& operator=(Manager&&) = delete;

    void pushTask(Task task) {
        bool anyFree = false;
        for (size_t i = 0; i < m_threadsInit; i++) {
            if (!m_threadsBusy[i]) {
                anyFree = true;
                break;
            }
        }

        if (!anyFree) {
            this->tryAllocThread();
        }

        std::unique_lock lock(m_mutex);
        m_tasks.emplace(std::move(task));
        m_condvar.notify_one();
    }

private:
    static constexpr size_t MAX_THREADS = 2;
    size_t m_threadsInit = 0;

    std::mutex m_mutex; // guards the queue
    std::queue<Task> m_tasks;
    std::array<std::thread, MAX_THREADS> m_threads;
    std::array<std::atomic_bool, MAX_THREADS> m_threadsBusy;
    std::condition_variable m_condvar;
    std::atomic_bool m_requestedStop;

    std::function<void()> threadPickTask(std::unique_lock<std::mutex>& lock) {
        auto task = std::move(m_tasks.front());
        m_tasks.pop();
        return task;
    }

    bool threadWait(std::unique_lock<std::mutex>& lock) {
        if (!m_tasks.empty()) return true;

        m_condvar.wait_for(lock, std::chrono::milliseconds{50}, [&] {
            return !m_tasks.empty();
        });

        return !m_tasks.empty();
    }

    void threadFunc(size_t idx) {
        while (!m_requestedStop) {
            m_threadsBusy[idx] = false;

            std::unique_lock lock(m_mutex);

            if (!this->threadWait(lock)) {
                continue;
            }

            m_threadsBusy[idx] = true;

            auto task = this->threadPickTask(lock);
            lock.unlock();

            task();
        }
    }

    void tryAllocThread() {
        if (m_threadsInit >= MAX_THREADS) return;

        m_threads[m_threadsInit] = std::thread(&Manager::threadFunc, this, m_threadsInit);
        m_threadsInit++;
    }

    void stopThreads() {
        m_requestedStop.store(true);

        for (size_t i = 0; i < m_threadsInit; i++) {
            if (m_threads[i].joinable()) m_threads[i].join();
        }
    }
};

}

bool LazySprite::init(CCSize size, bool loadingCircle) {
    if (!CCSprite::init()) return false;

    m_isLoading = false;
    m_hasLoaded = false;

    if (loadingCircle) {
        float lcsize = std::min<float>(size.width, size.height);

        m_loadingCircle = LoadingSpinner::create(lcsize);
        m_loadingCircle->setAnchorPoint({0.5f, 0.5f});
        m_loadingCircle->setPosition(size / 2.f);
        this->addChild(m_loadingCircle);
    }

    this->setContentSize(size);

    m_targetSize = size;
    m_autoresize = false;

    return true;
}

void LazySprite::loadFromUrl(const std::string& url, Format format, bool ignoreCache) {
    return this->loadFromUrl(url.c_str(), format, ignoreCache);
}

void LazySprite::loadFromUrl(char const* url, Format format, bool ignoreCache) {
    if (m_isLoading || m_hasLoaded) {
        return;
    }

    if (!ignoreCache && this->initFromCache(url)) {
        return;
    }

    m_expectedFormat = format;
    m_isLoading = true;

    m_listener.bind([this, cacheKey = ignoreCache ? std::string{} : std::string(url)](web::WebTask::Event* event) mutable {
        if (!event || !event->getValue()) return;

        auto resp = event->getValue();
        if (!resp->ok()) {
            std::string errmsg = resp->errorMessage();
            if (errmsg.empty()) {
                errmsg = resp->string().unwrapOrDefault();
            }

            if (errmsg.size() > 127) {
                errmsg.resize(124);
                errmsg += "...";
            }

            this->onError(fmt::format(
                "Request failed (code {}): {}",
                resp->code(),
                errmsg
            ));

            return;
        }

        auto data = resp->data();
        this->doInitFromBytes(std::move(data), std::move(cacheKey));
    });

    web::WebRequest req{};
    m_listener.setFilter(req.get(url));
}

void LazySprite::loadFromFile(const std::filesystem::path& path, Format format, bool ignoreCache) {
    if (m_isLoading || m_hasLoaded) {
        return;
    }

    auto cacheKey = ignoreCache ? std::string{} : this->makeCacheKey(path);
    if (!ignoreCache && this->initFromCache(cacheKey.c_str())) {
        return;
    }

    m_expectedFormat = format;
    m_isLoading = true;

    Manager::get().pushTask([
        selfref = WeakRef(this),
        path = path,
        cacheKey = std::move(cacheKey)
    ]() mutable {
        auto res = utils::file::readBinary(path);

        // oh god
        Loader::get()->queueInMainThread([
            selfref = std::move(selfref),
            path = std::move(path),
            cacheKey = std::move(cacheKey),
            res = std::move(res)
        ]() mutable {
            auto self = selfref.lock();
            if (!self) return;

            if (!res) {
                self->onError(fmt::format("failed to load from file {}: {}", path, res.unwrapErr()));
                return;
            }

            self->doInitFromBytes(std::move(res).unwrap(), std::move(cacheKey));
        });
    });
}

void LazySprite::loadFromData(std::span<uint8_t const> data, Format format) {
    this->loadFromData(data.data(), data.size(), format);
}

void LazySprite::loadFromData(uint8_t const* ptr, size_t size, Format format) {
    this->loadFromData(std::vector(ptr, ptr + size), format);
}

void LazySprite::loadFromData(std::vector<uint8_t> data, Format format) {
    if (m_isLoading || m_hasLoaded) {
        return;
    }

    m_expectedFormat = format;
    m_isLoading = true;

    this->doInitFromBytes(data, "");
}

void LazySprite::doInitFromBytes(std::vector<uint8_t> data, std::string cacheKey) {
    // do initialization in the threadpool
    Manager::get().pushTask([
        selfref = WeakRef(this),
        data = std::move(data),
        cacheKey = std::move(cacheKey),
        format = m_expectedFormat
    ]() mutable {
        auto image = new CCImage();
        bool res = image->initWithImageData(data.data(), data.size(), format);

        if (!res) {
            delete image;

            Loader::get()->queueInMainThread([selfref = std::move(selfref)]() mutable {
                auto self = selfref.lock();
                if (self) {
                    self->onError("invalid image data or format");
                }
            });

            return;
        }

        // image initialization succeeded, all we need to do now is to
        // create the opengl texture (must be on main thread!) and then set this sprite to use that.

        Loader::get()->queueInMainThread([
            selfref = std::move(selfref),
            image,
            cacheKey = std::move(cacheKey)
        ] {
            auto self = selfref.lock();
            if (!self) return;

            auto texture = new CCTexture2D();
            if (!texture->initWithImage(image)) {
                delete texture;
                image->release();
                self->onError("failed to initialize OpenGL texture");
                return;
            }

            image->release(); // deallocate the image, not needed anymore

            // store texture
            if (!cacheKey.empty()) {
                CCTextureCache::get()->m_pTextures->setObject(texture, cacheKey.c_str());
            }

            // this is weird but don't touch it unless you should
            if (!self->CCSprite::initWithTexture(texture)) {
                // this should never happen tbh
                self->onError("failed to initialize the sprite");
            }

            texture->release(); // bring texture's refcount back to 1
        });
    });
}

std::string LazySprite::makeCacheKey(std::filesystem::path const& path) {
#ifdef GEODE_IS_WINDOWS
    std::string p = geode::utils::string::wideToUtf8(path.wstring());
#else
    std::string p = path.string();
#endif

    return p;
}

CCTexture2D* LazySprite::lookupCache(char const* key) {
    return static_cast<CCTexture2D*>(CCTextureCache::get()->m_pTextures->objectForKey(key));
}

bool LazySprite::initFromCache(char const* key) {
    if (auto tex = this->lookupCache(key)) {
        return CCSprite::initWithTexture(tex); // this will end up calling our overriden 2-arg func, which is what we want
    }

    return false;
}

/* It's not impossible to optimize those too, but I did not bother for now, so they are just forwarders */

bool LazySprite::initWithTexture(CCTexture2D* texture, const CCRect& rect) {
    return this->postInit(CCSprite::initWithTexture(texture, rect));
}

bool LazySprite::initWithSpriteFrame(CCSpriteFrame* sf) {
    return this->postInit(CCSprite::initWithSpriteFrame(sf));
}

bool LazySprite::initWithSpriteFrameName(const char* fn) {
    return this->postInit(CCSprite::initWithSpriteFrameName(fn));
}

bool LazySprite::initWithFile(const char* fn, const CCRect& rect) {
    return this->postInit(CCSprite::initWithFile(fn, rect));
}

/* end forwarders */

bool LazySprite::postInit(bool initResult) {
    m_hasLoaded = initResult;
    m_isLoading = false;

    if (!initResult) return false;

    if (m_autoresize) {
        limitNodeSize(this, m_targetSize, std::min<float>(m_targetSize.width, m_targetSize.height), 0.f);
    }

    if (m_callback) {
        m_callback(Ok());
    }

    if (m_loadingCircle) {
        m_loadingCircle->removeFromParent();
        m_loadingCircle = nullptr;
    }

    return true;
}

void LazySprite::onError(std::string err) {
    m_hasLoaded = false;
    m_isLoading = false;

    if (m_callback) {
        m_callback(Err(std::move(err)));
    }

    if (m_loadingCircle) {
        m_loadingCircle->removeFromParent();
        m_loadingCircle = nullptr;
    }
}

void LazySprite::setLoadCallback(Callback callback) {
    m_callback = std::move(callback);
}

void LazySprite::setAutoResize(bool value) {
    m_autoresize = value;
}

bool LazySprite::isLoaded() {
    return m_hasLoaded;
}

bool LazySprite::isLoading() {
    return m_isLoading;
}

LazySprite* LazySprite::create(CCSize size, bool loadingCircle) {
    auto ret = new LazySprite;
    if (ret->init(size, loadingCircle)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
