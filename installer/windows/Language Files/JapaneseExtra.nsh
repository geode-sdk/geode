!insertmacro LANGFILE_EXT Japanese

!pragma warning disable 6030

${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "このセットアップは$(^NameDA)のインストールを案内します。$\r$\n$\r$\nインストールを開始する前に、Geometry Dashが実行されていないことを確認してください。$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "このセットアップは$(^NameDA)のアンインストールを案内します。$\r$\n$\r$\nアンインストールを開始する前に、Geometry Dashが実行されていないことを確認してください。$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\nこのパスにはGeometry Dashがインストールされていません！"
${LangFileString} GEODE_TEXT_MH_ALREADY_INSTALLED "このパスにはすでにMega Hack v6/v7がインストールされています！$\r$\nGeodeはMHv6/v7と互換性がありません（MHv8はGeodeと互換性があります）。$\r$\n続行する前にアンインストールしてください。"
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "このパスにはすでに他のモッドローダーがインストールされています！$\r$\nGeodeは他のモッドローダーと互換性がありません。$\r$\n続行する前にアンインストールしてください。（the dll trademark）"

; uninstaller

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "このパスにはGeodeがインストールされていません！"
