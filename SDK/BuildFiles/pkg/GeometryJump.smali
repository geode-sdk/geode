.class public Lcom/robtopx/geometryjump/GeometryJump;
.super Lorg/cocos2dx/lib/Cocos2dxActivity;
.source "GeometryJump.java"


# direct methods
.method static constructor <clinit>()V
	.locals 1

	.prologue
	.line 69
	const-string v0, "fmod"

	invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

	.line 70
	const-string v0, "cocos2dcpp"

	invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

	# Cacao mods

		.line 71
		# const-string v0, "Cacao"
		# invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

		# .line 72
		const-string v0, "CacTemplate"
		invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

	# End Cacao mods

	.line 85
	return-void
.end method

.method public constructor <init>()V
	.locals 0

	.prologue
	.line 37
	invoke-direct {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;-><init>()V

	return-void
.end method


# virtual methods
.method public getPublicKey()Ljava/lang/String;
	.locals 1

	.prologue
	.line 89
	const-string v0, "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgRyoIFy3izJ9+RBfum3Fvc6iiuk6TFst1yaPUUaThzD/6wlHwCDjVjnoDaqL29HE3j2VQpv5rmdSvDZXayxa1861//Ca3p3aDvlvcxZwyN6Fe51patKofmVNp9DhofDcsMNLLAfJMqihwgRCBHToi2B3xrstd+233UCgzz4oMg1cClfuSHn1YF7clksIrjuTrlBq1fmn3Km4E7S90ghyzL0KxT7gr1dyQdu8IuK++psnvv4wKybsKyuGm6fLk/X20TRJObi2Y8CRv4Zf9gWxSndR8sevQ1tOhQp1VQWWZqhks9r4DwPclEl2Ox0kREs4wR9zFdaWodFb1eKG3oNmPwIDAQAB"

	return-object v0
.end method

.method protected onCreate(Landroid/os/Bundle;)V
	.locals 0
	.param p1, "savedInstanceState"    # Landroid/os/Bundle;

	.prologue
	.line 40
	invoke-static {p0}, Lorg/fmod/FMOD;->init(Landroid/content/Context;)V

	.line 42
	invoke-super {p0, p1}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onCreate(Landroid/os/Bundle;)V

	.line 51
	return-void
.end method

.method public onCreateView()Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;
	.locals 7

	.prologue
	const/4 v1, 0x5

	.line 54
	new-instance v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

	invoke-direct {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;-><init>(Landroid/content/Context;)V

	.line 56
	.local v0, "glSurfaceView":Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;
	const/4 v2, 0x6

	const/4 v4, 0x0

	const/16 v5, 0x10

	const/16 v6, 0x8

	move v3, v1

	invoke-virtual/range {v0 .. v6}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->setEGLConfigChooser(IIIIII)V

	.line 58
	return-object v0
.end method

.method protected onDestroy()V
	.locals 0

	.prologue
	.line 63
	invoke-super {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onDestroy()V

	.line 64
	invoke-static {}, Lorg/fmod/FMOD;->close()V

	.line 66
	return-void
.end method
