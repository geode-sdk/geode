!insertmacro LANGFILE_EXT Turkish

!pragma warning disable 6030 ; overwriting default MUI strings on purpose
; to translate these strings:
; 1. copy them from C:\Program Files (x86)\NSIS\Contrib\Language Files\*.nsh
; 2. copy the second line from uninstall text to install text and make it say "installation" instead of "uninstallation"
; 3. replace $(^NameDA) in second line with Geometry Dash
${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "Bu sihirbaz size $(^NameDA) kurulumu boyunca rehberlik edecektir.$\r$\n$\r$\nKurulumu başlatmadan önce Geometry Dash'i kapatmanızı öneririz.$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "Bu sihirbaz size $(^NameDA) programının kadırılımı boyunca rehberlik edecektir.$\r$\n$\r$\nKaldırım işlemeni başlatmadan önce Geometry Dash'i kapatmanızı öneririz.$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\Bu dizin yolunda Geometry Dash yüklü değildir!"
${LangFileString} GEODE_TEXT_MH_ALREADY_INSTALLED "Bu dizin yolunda çoktan Mega Hack v6/v7 yüklüdür!$\r$\nGeode MHv6/v7 ile çalışmamaktadır (MHv8 Geode ile uyumlu olacaktır).$\r$\Lütfen devam etmek için programı kaldırın."
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "Bu dizin yolunda çoktan başka bir mod yükleyicisi bulunmaktadır!$\r$\nGeode başka bir mod yükleyicisi ile birlikte çalışmamaktadır.$\r$\nLütfen devam etmek için programı kaldırın. (the dll trademark)"

; uninstaller

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "Bu dizin yolunda Geode yüklü değildir!"
