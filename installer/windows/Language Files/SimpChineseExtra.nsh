!insertmacro LANGFILE_EXT SimpChinese

!pragma warning disable 6030
${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "Setup会帮您安装$(^NameDA)。$\r$\n$\r$\n安装Geode之前，请确认Geometry Dash已经关闭。$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "Setup会帮您卸载$(^NameDA)。$\r$\n$\r$\n卸载Geode之前，请确认Geometry Dash已经关闭。$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\nGeometry Dash不在这文件目录，请再试一遍！"
${LangFileString} GEODE_TEXT_MH_ALREADY_INSTALLED "这文件目录已经安装了Mega Hack v6/v7！$\r$\nGeode不能跟MHv6/v7一起用(可是MHv8可以跟Geode一起用)。$\r$\n请先卸载MHv6/v7。"
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "这文件目录已经安装了不同的游戏修改器加载器！$\r$\nGeode不能和不同的游戏修改器加载器一起用。$\r$\n请先卸载那个游戏修改器加载器。 (the dll trademark)"

; uninstaller

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "Geode不在这文件目录，请再试一遍！"