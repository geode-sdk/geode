!insertmacro LANGFILE_EXT English

!pragma warning disable 6030 ; overwriting default MUI strings on purpose
; to translate these strings:
; 1. copy them from C:\Program Files (x86)\NSIS\Contrib\Language Files\*.nsh
; 2. copy the second line from uninstall text to install text and make it say "installation" instead of "uninstallation"
; 3. replace $(^NameDA) in second line with Geometry Dash
${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nBefore starting the installation, make sure Geometry Dash is not running.$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "Setup will guide you through the uninstallation of $(^NameDA).$\r$\n$\r$\nBefore starting the uninstallation, make sure Geometry Dash is not running.$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\nThis path does not have Geometry Dash installed!"
${LangFileString} GEODE_TEXT_MH_ALREADY_INSTALLED "This path already has Mega Hack v6/v7 installed!$\r$\nGeode doesn't work with MHv6/v7 (MHv8 will be compatible with Geode).$\r$\nPlease, uninstall it before proceeding."
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "This path already has another mod loader installed!$\r$\nGeode doesn't work with any other mod loader.$\r$\nPlease, uninstall it before proceeding."

; uninstaller

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "This path does not have Geode installed!"
