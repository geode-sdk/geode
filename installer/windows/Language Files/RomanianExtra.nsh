!insertmacro LANGFILE_EXT Romanian

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

${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "Programul te va ghida prin instalarea $(^NameDA).$\r$\n$\r$\nÎnainte să începem, asigurați-vă că Geometry Dash nu este deschis.$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "Programul te va ghida prin dezinstalarea $(^NameDA).$\r$\n$\r$\nÎnainte să începem, asigurați-vă că Geometry Dash nu este deschis.$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\nAceastă cale nu are Geometry Dash instalat!"
${LangFileString} GEODE_TEXT_GD_OLD "$\r$\n$\r$\nVersiunea ta de Geometry Dash este prea veche!"
${LangFileString} GEODE_TEXT_GD_RUNNING "Vă rugăm să închideți Geometry Dash pentru a efectua instalarea."
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "Această cale are alte modificări instalate!$\r$\nVor fi suprascrise."
${LangFileString} GEODE_TEXT_INSTALLING_VCREDIST "Se instalează VS Runtime. Verificați bara de activități pentru alte ferestre deschise..."

; uninstaller

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "Această cale nu are Geode instalat!"
