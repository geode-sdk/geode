#include <Geode/ui/LazySprite.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/async.hpp>

using namespace geode::prelude;

class LazySprite::Impl {
public:
    Ref<LoadingSpinner> m_loadingCircle;
    Callback m_callback;
    Format m_expectedFormat;
    async::TaskHolder<web::WebResponse> m_listener;
    bool m_isLoading = false;
    std::atomic_bool m_hasLoaded = false;
    bool m_autoresize;
    cocos2d::CCSize m_targetSize;
    LazySprite* m_self;

    Impl(LazySprite* self) : m_self(self) {}

    bool init(cocos2d::CCSize size, bool loadingCircle = true);
    void doInitFromBytes(std::vector<uint8_t> data, std::string cacheKey);
    std::string makeCacheKey(std::filesystem::path const& path);
    // std::string makeCacheKey(std::string_view url);

    cocos2d::CCTexture2D* lookupCache(char const* key);
    bool initFromCache(char const* key);
    bool postInit(bool initResult);

    void onError(std::string err);
    void handleRequest();
};

LazySprite::LazySprite()
    : m_impl(std::make_unique<Impl>(this)) {}

LazySprite::~LazySprite() = default;

bool LazySprite::Impl::init(CCSize size, bool loadingCircle) {
    if (!m_self->CCSprite::init()) return false;

    m_isLoading = false;
    m_hasLoaded = false;

    if (loadingCircle) {
        float lcsize = std::min<float>(size.width, size.height);

        m_loadingCircle = LoadingSpinner::create(lcsize);
        m_loadingCircle->setAnchorPoint({0.5f, 0.5f});
        m_loadingCircle->setPosition(size / 2.f);
        m_self->addChild(m_loadingCircle);
    }

    m_self->setContentSize(size);

    m_targetSize = size;
    m_autoresize = false;

    return true;
}

void LazySprite::loadFromUrl(std::string url, Format format, bool ignoreCache) {
    if (m_impl->m_isLoading || m_impl->m_hasLoaded) {
        return;
    }

    if (!ignoreCache && m_impl->initFromCache(url.c_str())) {
        return;
    }

    m_impl->m_expectedFormat = format;
    m_impl->m_isLoading = true;

    m_impl->m_listener.spawn(
        "LazySprite Web Listener",
        web::WebRequest{}.get(url),
        [this, cacheKey = ignoreCache ? std::string{} : std::string(url)](web::WebResponse resp) mutable {
            if (!resp.ok()) {
                std::string errmsg(resp.errorMessage());
                if (errmsg.empty()) {
                    errmsg = resp.string().unwrapOrDefault();
                }
    
                if (errmsg.size() > 127) {
                    errmsg.resize(124);
                    errmsg += "...";
                }
    
                this->m_impl->onError(fmt::format(
                    "Request failed (code {}): {}",
                    resp.code(),
                    errmsg
                ));
    
                return;
            }
    
            this->m_impl->doInitFromBytes(std::move(resp).data(), std::move(cacheKey));
        }
    );
}

void LazySprite::loadFromFile(const std::filesystem::path& path, Format format, bool ignoreCache) {
    if (m_impl->m_isLoading || m_impl->m_hasLoaded) {
        return;
    }

    auto cacheKey = ignoreCache ? std::string{} : m_impl->makeCacheKey(path);
    if (!ignoreCache && m_impl->initFromCache(cacheKey.c_str())) {
        return;
    }

    m_impl->m_expectedFormat = format;
    m_impl->m_isLoading = true;

    async::runtime().spawnBlocking<void>([
        selfref = WeakRef(this),
        path = path,
        cacheKey = std::move(cacheKey)
    ] mutable {
        auto res = utils::file::readBinary(path);

        // oh god
        Loader::get()->queueInMainThread([
            selfref = std::move(selfref),
            path = std::move(path),
            cacheKey = std::move(cacheKey),
            res = std::move(res)
        ]() mutable {
            auto self = selfref.lock();

            // if sprite was destructed or loading has been cancelled, do nothing
            if (!self || !self->m_impl->m_isLoading) return;

            if (!res) {
                self->m_impl->onError(fmt::format("failed to load from file {}: {}", path, res.unwrapErr()));
                return;
            }

            self->m_impl->doInitFromBytes(std::move(res).unwrap(), std::move(cacheKey));
        });
    });
}

void LazySprite::loadFromData(std::vector<uint8_t> data, Format format) {
    if (m_impl->m_isLoading || m_impl->m_hasLoaded) {
        return;
    }

    m_impl->m_expectedFormat = format;
    m_impl->m_isLoading = true;

    m_impl->doInitFromBytes(std::move(data), "");
}

void LazySprite::loadFromData(std::span<uint8_t const> data, Format format) {
    this->loadFromData(std::vector<uint8_t>{data.begin(), data.end()}, format);
}

void LazySprite::loadFromData(uint8_t const* ptr, size_t size, Format format) {
    this->loadFromData(std::span{ptr, size}, format);
}

// ! This function must be invoked on main thread !
void LazySprite::Impl::doInitFromBytes(std::vector<uint8_t> data, std::string cacheKey) {
    // do initialization in the threadpool
    async::runtime().spawnBlocking<void>([
        selfref = WeakRef(m_self),
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
                if (self && self->m_impl->m_isLoading) {
                    self->m_impl->onError("invalid image data or format");
                }
            });

            return;
        }

        // image initialization succeeded, all we need to do now is to
        // create the OpenGL texture (must be on main thread!) and then set this sprite to use that.

        Loader::get()->queueInMainThread([
            selfref = std::move(selfref),
            image,
            cacheKey = std::move(cacheKey)
        ] {
            auto self = selfref.lock();
            if (!self || !self->m_impl->m_isLoading) return;

            auto texture = new CCTexture2D();
            if (!texture->initWithImage(image)) {
                delete texture;
                image->release();
                self->m_impl->onError("failed to initialize OpenGL texture");
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
                self->m_impl->onError("failed to initialize the sprite");
            }

            texture->release(); // bring texture's refcount back to 1
        });
    });
}

std::string LazySprite::Impl::makeCacheKey(std::filesystem::path const& path) {
    return utils::string::pathToString(path);
}

CCTexture2D* LazySprite::Impl::lookupCache(char const* key) {
    return static_cast<CCTexture2D*>(CCTextureCache::get()->m_pTextures->objectForKey(key));
}

bool LazySprite::Impl::initFromCache(char const* key) {
    if (auto tex = this->lookupCache(key)) {
        return m_self->CCSprite::initWithTexture(tex); // this will end up calling our overridden 2-arg func, which is what we want
    }

    return false;
}

/* It's not impossible to optimize those too, but I did not bother for now, so they are just forwarders */

bool LazySprite::initWithTexture(CCTexture2D* texture, const CCRect& rect, bool rotated) {
    return m_impl->postInit(CCSprite::initWithTexture(texture, rect, rotated));
}

bool LazySprite::initWithSpriteFrame(CCSpriteFrame* sf) {
    return m_impl->postInit(CCSprite::initWithSpriteFrame(sf));
}

bool LazySprite::initWithSpriteFrameName(const char* fn) {
    return m_impl->postInit(CCSprite::initWithSpriteFrameName(fn));
}

bool LazySprite::initWithFile(const char* fn, const CCRect& rect) {
    return m_impl->postInit(CCSprite::initWithFile(fn, rect));
}

/* end forwarders */

bool LazySprite::Impl::postInit(bool initResult) {
    m_hasLoaded = initResult;
    m_isLoading = false;

    if (!initResult) return false;

    if (m_autoresize) {
        limitNodeSize(m_self, m_targetSize, std::min<float>(m_targetSize.width, m_targetSize.height), 0.f);
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

void LazySprite::Impl::onError(std::string err) {
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
    m_impl->m_callback = std::move(callback);
}

void LazySprite::setAutoResize(bool value) {
    m_impl->m_autoresize = value;
}

bool LazySprite::isLoaded() {
    return m_impl->m_hasLoaded;
}

bool LazySprite::isLoading() {
    return m_impl->m_isLoading;
}

void LazySprite::cancelLoad() {
    m_impl->m_isLoading = false;

    if (m_impl->m_loadingCircle) {
        m_impl->m_loadingCircle->removeFromParent();
        m_impl->m_loadingCircle = nullptr;
    }
}

LazySprite* LazySprite::create(CCSize size, bool loadingCircle) {
    auto ret = new LazySprite;
    if (ret->m_impl->init(size, loadingCircle)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
