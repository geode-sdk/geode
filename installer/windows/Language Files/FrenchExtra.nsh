!insertmacro LANGFILE_EXT French

!pragma warning disable 6030
${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "Le programme d'installation vous guidera tout au long de l'installation de $(^NameDA).$\r$\n$\r$\nAvant de démarrer l'installation, assurez-vous que Geometry Dash n'est pas en cours d'exécution.$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "Le programme d'installation vous guidera tout au long de la désinstallation de $(^NameDA).$\r$\n$\r$\nAvant de démarrer la désinstallation, assurez-vous que Geometry Dash n'est pas en cours d'exécution.$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\nGeometry Dash n'est pas installé sur ce chemin !"
${LangFileString} GEODE_TEXT_GD_OLD "$\r$\n$\r$\nVotre version de Geometry Dash est trop ancienne pour cette version de Geode !"
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "Ce chemin a d'autres mods installés !$\r$\nIls seront écrasés par Geode. (the dll trademark)"

; uninstaller

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "Geode n'est pas installé sur ce chemin !"
