!insertmacro LANGFILE_EXT French

!pragma warning disable 6030
${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "Le programme d'installation vous guidera tout au long de l'installation de $(^NameDA).$\r$\n$\r$\nAvant de démarrer l'installation, assurez-vous que Geometry Dash n'est pas en cours d'exécution.$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "Le programme d'installation vous guidera tout au long de la désinstallation de $(^NameDA).$\r$\n$\r$\nAvant de démarrer la désinstallation, assurez-vous que Geometry Dash n'est pas en cours d'exécution.$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} GEODE_TEXT_GD_MISSING "$\r$\n$\r$\nGeometry Dash n'est pas installé sur ce chemin !"
${LangFileString} GEODE_TEXT_MH_ALREADY_INSTALLED "Ce chemin a déjà Mega Hack v6/v7 installé !$\r$\nGeode ne fonctionne pas avec MHv6/v7 (MHv8 sera compatible avec Geode).$\r$\nVeuillez le désinstaller avant de continuer."
${LangFileString} GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED "Ce chemin contient déjà un autre chargeur de mod installé !$\r$\nGeode ne fonctionne avec aucun autre chargeur de mod.$\r$\nVeuillez le désinstaller avant de continuer. (the dll trademark)"

; uninstaller

${LangFileString} GEODE_UNTEXT_GEODE_MISSING "Geode n'est pas installé sur ce chemin !"
