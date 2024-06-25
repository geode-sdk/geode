!insertmacro LANGFILE_EXT Korean

!pragma warning disable 6030 ; overwriting default MUI strings on purpose
; to translate these strings:
; 1. copy them from C:\Program Files (x86)\NSIS\Contrib\Language Files\*.nsh
; 2. copy the second line from uninstall text to install text and make it say "installation" instead of "uninstallation"
; 3. replace $(^NameDA) in second line with Geometry Dash
; 4. do not translate "(the dll trademark)", since that is replaced with the actual dll text

${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "이 프로그램은 $(^NameDA)의 설치를 진행해줄 것입니다.$\r$\n$\r$\n설치를 시작하기 전에, Geometry Dash가 켜져있지 않은지 확인해주세요.$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "이 프로그램은 $(^NameDA)의 삭제를 진행해줄 것입니다.$\r$\n$\r$\n삭제를 시작하기 전에, Geometry Dash가 켜져있지 않은지 확인해주세요.$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\n이 경로에는 Geometry Dash가 설치되어 있지 않습니다."
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "이 경로에는 다른 모드가 설치되어 있습니다!$\r$\nGeode에 의해 덮어쓰게 될 것입니다. (the dll trademark)"

; uninstaller

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "이 경로에는 Geode가 설치되어 있지 않습니다!"
