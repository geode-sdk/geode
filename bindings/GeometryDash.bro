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

    bool m_fromRefresh;
}

class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol, GooglePlayDelegate {
    inline MenuLayer() {}
    //virtual ~MenuLayer() = mac 0x1d1230, win 0x190620;
    virtual bool init() = win 0x276700;
    
    void onMoreGames(cocos2d::CCObject*) = win 0x277A50;
    void onGarage(cocos2d::CCObject*) = win ;
    void onQuit(cocos2d::CCObject*) = win ;
    void onMyProfile(cocos2d::CCObject*) = win ;
    void onPlay(cocos2d::CCObject*) = win 0x277B40;
    void onCreator(cocos2d::CCObject*) = win ;
    void onRobTop(cocos2d::CCObject*) = win ;
    void onNewgrounds(cocos2d::CCObject*) = win ;
    void onDaily(cocos2d::CCObject*) = win ;
    void onOptions(cocos2d::CCObject*) = win 0x277BE0;
    void onAchievements(cocos2d::CCObject*) = win 0x277AE0;
    void onStats(cocos2d::CCObject*) = win ;
    void onFacebook(cocos2d::CCObject*) = win 0x124F50;
    void onTwitter(cocos2d::CCObject*) = win 0x124FF0;
    void onYouTube(cocos2d::CCObject*) = win 0x125080;
    void onDiscord(cocos2d::CCObject*) = win 0x1251A0;
    void onTwitch(cocos2d::CCObject*) = win 0x125110;
    void onReddit(cocos2d::CCObject*) = win 0x125230;
    static cocos2d::CCScene* scene(bool) = win ;

    cocos2d::CCSprite* m_googlePlaySprite;
    cocos2d::CCSprite* m_viewProfileInfoText;
    cocos2d::CCLabelBMFont* m_playerNameLabel;
    CCMenuItemSpriteExtra* m_profileBtn;
}
