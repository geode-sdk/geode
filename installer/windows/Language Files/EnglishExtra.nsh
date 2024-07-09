!insertmacro LANGFILE_EXT English

!pragma warning disable 6030 ; overwriting default MUI strings on purpose
; to translate these strings:
; 1. copy them from C:\Program Files (x86)\NSIS\Contrib\Language Files\*.nsh
; 2. copy the second line from uninstall text to install text and make it say "installation" instead of "uninstallation"
; 3. replace $(^NameDA) in second line with Geometry Dash
; 4. do not translate "(the dll trademark)", since that is replaced with the actual dll text
; Note: please, format translation files the *exact same* way they are formatted in
;       other translations. Remove this entire comment block,
;       do not translate the "installer" and "uninstaller" comments and
;       do not remove the '!pragma ...' and '!insertmacro ...' lines at the top.

${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nBefore starting the installation, make sure Geometry Dash is not running.$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "Setup will guide you through the uninstallation of $(^NameDA).$\r$\n$\r$\nBefore starting the uninstallation, make sure Geometry Dash is not running.$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\nThis path does not have Geometry Dash installed!"
${LangFileString} GEODE_TEXT_GD_OLD "$\r$\n$\r$\nYour version of Geometry Dash is too old for this version of Geode!"
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "This path has other mods installed!$\r$\nThey will be overwritten by Geode. (the dll trademark)"

; uninstaller

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "This path does not have Geode installed!"
