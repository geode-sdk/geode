!insertmacro LANGFILE_EXT Norwegian

!pragma warning disable 6030 ; Overstyrer standard MUI-strenger med vilje
; for å oversette disse strengene:
; 1. Kopier dem fra C:\Program Files (x86)\NSIS\Contrib\Language Files\*.nsh
; 2. Kopier andre linje fra avinstallasjonstekst til installasjonstekst og gjør den si "installasjon" i stedet for "avinstallasjon"
; 3. Erstatt $(^NameDA) i andre linje med Geometry Dash
; 4. Ikke oversett "(the dll trademark)", siden det erstattes med faktisk dll-tekst
${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "Oppsettet vil veilede deg gjennom installasjonen av $(^NameDA).$\r$\n$\r$\nFør du starter installasjonen, sørg for at Geometry Dash ikke kjører.$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "Oppsettet vil veilede deg gjennom avinstallasjonen av $(^NameDA).$\r$\n$\r$\nFør du starter avinstallasjonen, sørg for at Geometry Dash ikke kjører.$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\nDenne banen har ikke Geometry Dash installert!"
${LangFileString} GEODE_TEXT_MH_ALREADY_INSTALLED "Denne banen har allerede Mega Hack v6/v7 installert!$\r$\nGeode fungerer ikke med MHv6/v7 (MHv8 vil være kompatibel med Geode).$\r$\nVennligst avinstaller det før du fortsetter."
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "Denne banen har allerede en annen modellaster installert!$\r$\nGeode fungerer ikke med noen annen modellaster.$\r$\nVennligst avinstaller det før du fortsetter. (the dll trademark)"

; avinstallerer

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "Denne banen har ikke Geode installert!"
