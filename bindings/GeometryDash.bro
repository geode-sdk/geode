class LoadingLayer : cocos2d::CCLayer {
    void setFromRefresh(bool value) {
        m_fromRefresh = value;
    }

    LoadingLayer() {}
    static LoadingLayer* create(bool fromReload) = win 0x271A90;
    bool init(bool fromReload) = win 0x271B30;
    const char* getLoadingString() = win 0x272A20;
    void loadAssets() = win 0x272390;
    void loadingFinished() = win 0x2722A0;

    static cocos2d::CCScene* scene(bool fromReload) {
        auto scene = cocos2d::CCScene::create();
        scene->addChild(LoadingLayer::create(fromReload));

        return scene;
    }

    bool m_fromRefresh ;
}
