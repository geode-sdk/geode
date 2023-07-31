!insertmacro LANGFILE_EXT Czech

!pragma warning disable 6030 ; overwriting default MUI strings on purpose
; to translate these strings:
; 1. copy them from C:\Program Files (x86)\NSIS\Contrib\Language Files\*.nsh
; 2. copy the second line from uninstall text to install text and make it say "installation" instead of "uninstallation"
; 3. replace $(^NameDA) in second line with Geometry Dash
${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "Tento průvodce vás provede instalací $(^NameDA).$\r$\n$\r$\nPřed spuštěním instalace se přesvědčte, že hra Geometry Dash není spuštěna.$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "Tento průvodce vás provede odinstalací $(^NameDA).$\r$\n$\r$\nPřed spuštěním odinstalace se přesvědčte, že hra Geometry Dash není spuštěna.$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

${LangFileString} GEODE_TEXT_SYSTEM_XINPUT_MISSING "$0 nebyl nalezen. Geode je kompatibilní pouze s Windows 7 a vyšším."
${LangFileString} GEODE_TEXT_MH_ALREADY_INSTALLED "V této cestě je již nainstalován Mega Hack v6/v7!$\r$\nGeode nefunguje s MHv6/v7 (MHv8 ale již kompatibilní bude).$\r$\nProsím, odinstalujte jej, než budete pokračovat."
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "V této cestě je již nainstalován jiný mod loader!$\r$\nGeode s žádným jiným mod loaderem dohromady nefunguje.$\r$\nProsím, odinstalujte jej, než budete pokračovat."
${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\nV této cestě se nenachází Geometry Dash!"

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "V této cestě se nenachází Geode!"
