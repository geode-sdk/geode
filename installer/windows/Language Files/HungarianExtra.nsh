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

${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "A telepítő végigvezeti Önt a $(^NameDA) telepítésén.$\r$\n$\r$\nA telepítés megkezdése előtt győződj meg arról, hogy a Geometry Dash nem fut.$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "A telepítő végigvezeti Önt a $(^NameDA) eltávolításának folyamatán.$\r$\n$\r$\nAz eltávolítás megkezdése előtt győződj meg arról, hogy a Geometry Dash nem fut.$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\nEzzen az útvonalon nem található Geometry Dash telepítés!"
${LangFileString} GEODE_TEXT_GD_OLD "$\r$\n$\r$\nA Geomtery Dash-ed verziója régi a Geode futattására!"
${LangFileString} GEODE_TEXT_GD_RUNNING "Kérlek zárd be a Geometry Dash-t mielőtt telepíted a Geode-ot."
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "Ezzen az útvonalon más modok vannak telepítve!$\r$\nEzeket a Geode felülírja. (a DLL védjegye)"
${LangFileString} GEODE_TEXT_INSTALLING_VCREDIST "A VS Runtime telepítése. Ellenőrizze a tálcán, hogy nyílt-e új ablak..."

; uninstaller

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "Ezzen az útvonalon nem található Geode telepítés!"
