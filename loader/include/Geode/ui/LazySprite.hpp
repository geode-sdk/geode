#pragma once

#include "LoadingSpinner.hpp"
#include <Geode/utils/web.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/function.hpp>

#include <cocos2d.h>
#include <filesystem>
#include <string>
#include <functional>
#include <span>
#include <stdint.h>

namespace geode {
    /**
     * A sprite that is loaded asynchronously.
     * It can be initialized to either a blank texture or a loading circle,
     * and then given either a URL, file path, or raw image data that will be processed
     * in the background to avoid freezes.
     */
    class GEODE_DLL LazySprite final : public cocos2d::CCSprite {
    public:
        using Callback = geode::Function<void(Result<>)>;
        using Format = cocos2d::CCImage::EImageFormat;

        static LazySprite* create(cocos2d::CCSize size, bool loadingCircle = true);

        void loadFromUrl(std::string url, Format format = Format::kFmtUnKnown, bool ignoreCache = false);
        void loadFromFile(std::filesystem::path const& path, Format format = Format::kFmtUnKnown, bool ignoreCache = false);
        void loadFromData(std::vector<uint8_t> data, Format format = Format::kFmtUnKnown);
        void loadFromData(std::span<uint8_t const> data, Format format = Format::kFmtUnKnown);
        void loadFromData(uint8_t const* ptr, size_t size, Format format = Format::kFmtUnKnown);

        /**
         * Set the callback to be called once the sprite is fully loaded, or an error occurred.
         * @param callback The callback
         */
        void setLoadCallback(Callback callback);

        /**
         * Set whether the image will be automatically resized to the size given in the `create` / `init` function
         * once it's loaded. By default is `false`.
         */
        void setAutoResize(bool value);

        /**
         * Returns whether the image is now loaded
         */
        bool isLoaded();
        bool isLoading();

        /**
         * Cancel the sprite loading process. Does nothing if `isLoading == false`.
         * Callback will not be called, and sprite loading will be halted as soon as possible.
         */
        void cancelLoad();

        virtual bool initWithTexture(cocos2d::CCTexture2D* pTexture, const cocos2d::CCRect& rect, bool rotated) override;
        using CCSprite::initWithTexture;

        virtual bool initWithSpriteFrame(cocos2d::CCSpriteFrame* pSpriteFrame) override;
        virtual bool initWithSpriteFrameName(const char* pszSpriteFrameName) override;

        virtual bool initWithFile(const char* pszFilename, const cocos2d::CCRect& rect) override;
        using CCSprite::initWithFile;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        LazySprite();
        ~LazySprite();
    };
}
