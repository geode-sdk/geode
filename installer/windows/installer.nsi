; includes
    !include MUI2.nsh
    !include nsDialogs.nsh
    !include WinMessages.nsh

; settings
    Name "Geode"
    OutFile "geode-installer-win.exe"
    Unicode true
    InstallDir "$PROGRAMFILES32\Steam\steamapps\common\Geometry Dash\" ; set default path to the most common one
    XPStyle on
    RequestExecutionLevel user
    ManifestSupportedOS Win7
    SetCompressor /SOLID lzma
    SetCompressorDictSize 64
    SetDatablockOptimize ON

; ui settings
    !define MUI_ABORTWARNING
    !define MUI_LANGDLL_ALLLANGUAGES
    !define MUI_FINISHPAGE_NOAUTOCLOSE
    !define MUI_UNFINISHPAGE_NOAUTOCLOSE
    !define MUI_FINISHPAGE_NOREBOOTSUPPORT
    !define MUI_ICON Graphics\logo_inst.ico
    !define MUI_UNICON Graphics\logo_uninst.ico
    !define MUI_WELCOMEFINISHPAGE_BITMAP Graphics\banner.bmp
    !define MUI_UNWELCOMEFINISHPAGE_BITMAP Graphics\banner.bmp

; pages
    !insertmacro MUI_PAGE_WELCOME
    !insertmacro MUI_PAGE_LICENSE "..\..\LICENSE.txt"
    !define MUI_COMPONENTSPAGE_NODESC
    !insertmacro MUI_PAGE_COMPONENTS
    !define MUI_PAGE_CUSTOMFUNCTION_SHOW DirectoryPageShow
    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_INSTFILES
    !insertmacro MUI_PAGE_FINISH

    !insertmacro MUI_UNPAGE_WELCOME
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES
    !insertmacro MUI_UNPAGE_FINISH

; languages
    !macro GEODE_LANGUAGE NSFID
        !insertmacro MUI_LANGUAGE ${NSFID}
        !insertmacro LANGFILE_INCLUDE "Language Files\${NSFID}Extra.nsh"
    !macroend

    ; TODO: add the commented out languages (other available languages are listed here: https://nsis.sourceforge.io/Examples/Modern%20UI/MultiLanguage.nsi)
    !insertmacro GEODE_LANGUAGE "English"
    !insertmacro GEODE_LANGUAGE "French"
    !insertmacro GEODE_LANGUAGE "German"
    !insertmacro GEODE_LANGUAGE "Spanish"
    !insertmacro GEODE_LANGUAGE "SpanishInternational"
    !insertmacro GEODE_LANGUAGE "Italian"
    !insertmacro GEODE_LANGUAGE "Swedish"
    !insertmacro GEODE_LANGUAGE "Greek"
    !insertmacro GEODE_LANGUAGE "Finnish"
    !insertmacro GEODE_LANGUAGE "Polish"
    !insertmacro GEODE_LANGUAGE "Russian"
    !insertmacro GEODE_LANGUAGE "PortugueseBR"
    !insertmacro GEODE_LANGUAGE "Portuguese"
    !insertmacro GEODE_LANGUAGE "Ukrainian"
    !insertmacro GEODE_LANGUAGE "Czech"
    !insertmacro GEODE_LANGUAGE "Turkish"
    !insertmacro GEODE_LANGUAGE "Japanese"
    !insertmacro GEODE_LANGUAGE "SimpChinese"
    !insertmacro GEODE_LANGUAGE "TradChinese"
    !insertmacro GEODE_LANGUAGE "Korean"

    !insertmacro MUI_RESERVEFILE_LANGDLL

; random shit
    ; https://nsis.sourceforge.io/StrReplace
    !define StrRep "!insertmacro StrRep"
    !macro StrRep output string old new
        Push `${string}`
        Push `${old}`
        Push `${new}`
        !ifdef __UNINSTALL__
            Call un.StrRep
        !else
            Call StrRep
        !endif
        Pop ${output}
    !macroend
    !macro Func_StrRep un
        Function ${un}StrRep
            Exch $R2 ;new
            Exch 1
            Exch $R1 ;old
            Exch 2
            Exch $R0 ;string
            Push $R3
            Push $R4
            Push $R5
            Push $R6
            Push $R7
            Push $R8
            Push $R9

            StrCpy $R3 0
            StrLen $R4 $R1
            StrLen $R6 $R0
            StrLen $R9 $R2
            loop:
                StrCpy $R5 $R0 $R4 $R3
                StrCmp $R5 $R1 found
                StrCmp $R3 $R6 done
                IntOp $R3 $R3 + 1 ;move offset by 1 to check the next character
                Goto loop
            found:
                StrCpy $R5 $R0 $R3
                IntOp $R8 $R3 + $R4
                StrCpy $R7 $R0 "" $R8
                StrCpy $R0 $R5$R2$R7
                StrLen $R6 $R0
                IntOp $R3 $R3 + $R9 ;move offset by length of the replacement string
                Goto loop
            done:

            Pop $R9
            Pop $R8
            Pop $R7
            Pop $R6
            Pop $R5
            Pop $R4
            Pop $R3
            Push $R0
            Push $R1
            Pop $R0
            Pop $R1
            Pop $R0
            Pop $R2
            Exch $R1
        FunctionEnd
    !macroend
    !insertmacro Func_StrRep ""
    ;!insertmacro Func_StrRep "un."

    ; https://nsis.sourceforge.io/Remove_leading_and_trailing_whitespaces_from_a_string
    Function Trim
        Exch $R1 ; Original string
        Push $R2
        Loop:
            StrCpy $R2 "$R1" 1
            StrCmp "$R2" " " TrimLeft
            StrCmp "$R2" "$\r" TrimLeft
            StrCmp "$R2" "$\n" TrimLeft
            StrCmp "$R2" "$\t" TrimLeft
            Goto Loop2
        TrimLeft:
            StrCpy $R1 "$R1" "" 1
            Goto Loop
        Loop2:
            StrCpy $R2 "$R1" 1 -1
            StrCmp "$R2" " " TrimRight
            StrCmp "$R2" "$\r" TrimRight
            StrCmp "$R2" "$\n" TrimRight
            StrCmp "$R2" "$\t" TrimRight
            Goto Done
        TrimRight:
            StrCpy $R1 "$R1" -1
            Goto Loop2
        Done:
            Pop $R2
            Exch $R1
    FunctionEnd
    !define Trim "!insertmacro Trim"
    !macro Trim ResultVar String
        Push "${String}"
        Call Trim
        Pop "${ResultVar}"
    !macroend

    ; https://nsis.sourceforge.io/Split_strings
    !macro GET_STRING_TOKEN INPUT PART
        Push $R0
        Push $R1
        Push $R2
        StrCpy $R0 -1
        IntOp $R1 ${PART} * 2
        IntOp $R1 $R1 - 1
        findStart_loop_${PART}:
            IntOp $R0 $R0 + 1
            StrCpy $R2 ${INPUT} 1 $R0
            StrCmp $R2 "" error_${PART}
            StrCmp $R2 '"' 0 findStart_loop_${PART}
            IntOp $R1 $R1 - 1
            IntCmp $R1 0 0 0 findStart_loop_${PART}
            IntOp $R1 $R0 + 1
        findEnd_loop_${PART}:
            IntOp $R0 $R0 + 1
            StrCpy $R2 ${INPUT} 1 $R0
            StrCmp $R2 "" error_${PART}
            StrCmp $R2 '"' 0 findEnd_loop_${PART}
            IntOp $R0 $R0 - $R1
            StrCpy $R0 ${INPUT} $R0 $R1
            Goto done_${PART}
        error_${PART}:
            StrCpy $R0 error
        done_${PART}:
            Pop $R2
            Pop $R1
            Exch $R0
    !macroend

    ; https://nsis.sourceforge.io/StrStr
    Function un.StrStr
        Exch $R0
        Exch
        Exch $R1
        Push $R2
        Push $R3
        Push $R4
        Push $R5

        StrLen $R2 $R0
        StrLen $R3 $R1

        StrCpy $R4 0

        loop:
            StrCpy $R5 $R1 $R2 $R4
            StrCmp $R5 $R0 done
            IntCmp $R4 $R3 done 0 done
            IntOp $R4 $R4 + 1
            Goto loop
        done:
            StrCpy $R0 $R1 `` $R4

        Pop $R5
        Pop $R4
        Pop $R3
        Pop $R2
        Pop $R1
        Exch $R0
    FunctionEnd

    ; https://nsis.sourceforge.io/Get_command_line_parameters
    Function un.GetParameters
        Push $R0
        Push $R1
        Push $R2
        Push $R3

        StrCpy $R2 1
        StrLen $R3 $CMDLINE

        StrCpy $R0 $CMDLINE $R2
        StrCmp $R0 '"' 0 +3
            StrCpy $R1 '"'
            Goto loop
        StrCpy $R1 " "

        loop:
            IntOp $R2 $R2 + 1
            StrCpy $R0 $CMDLINE 1 $R2
            StrCmp $R0 $R1 get
            StrCmp $R2 $R3 get
            Goto loop

        get:
            IntOp $R2 $R2 + 1
            StrCpy $R0 $CMDLINE 1 $R2
            StrCmp $R0 " " get
            StrCpy $R0 $CMDLINE "" $R2

        Pop $R3
        Pop $R2
        Pop $R1
        Exch $R0
    FunctionEnd

    ; https://nsis.sourceforge.io/Get_command_line_parameter_by_name
    Function un.GetParameterValue
        Exch $R0
        Exch
        Exch $R1

        Push $R2
        Push $R3
        Push $R4
        Push $R5

        Strlen $R2 $R1+2

        Call un.GetParameters
        Pop $R3

        StrCpy $R5 '"'
        Push $R3
        Push '"/$R1='
        Call un.StrStr
        Pop $R4
        StrCpy $R4 $R4 "" 1
        StrCmp $R4 "" "" next

        StrCpy $R5 ' '
        Push $R3
        Push '/$R1='
        Call un.StrStr
        Pop $R4

        next:
            StrCmp $R4 "" check_for_switch
            StrCpy $R0 $R4 "" $R2
            Push $R0
            Push $R5
            Call un.StrStr
            Pop $R4
            StrCmp $R4 "" done
            StrLen $R4 $R4
            StrCpy $R0 $R0 -$R4
            goto done

        check_for_switch:
            Push $R3
            Push '/$R1'
            Call un.StrStr
            Pop $R4
            StrCmp $R4 "" done
            StrCpy $R0 ""

        done:
            Pop $R5
            Pop $R4
            Pop $R3
            Pop $R2
            Pop $R1
            Exch $R0
    FunctionEnd

; actual code

!define BINDIR ..\..\bin\nightly

; installer

Var geode.DirectoryPage.ErrorText

Var GamePath
Function FindGamePath
    Push $0
    Push $1
    Push $2
    Push $3
    Push $4

    # get steam path
    ReadRegStr $0 HKCU "Software\Valve\Steam" "SteamPath"
    IfErrors gamePathError
    ${StrRep} $0 $0 "/" "\"

    # read library folders
    FileOpen $1 "$0\steamapps\libraryfolders.vdf" r
    IfErrors gamePathError

    vdfReadLine:
        ClearErrors
        FileRead $1 $2
        IfErrors vdfDone
        ${Trim} $2 $2
        !insertmacro GET_STRING_TOKEN $2 1
        Pop $4
        StrCmp $4 "path" 0 vdfCheckAppId
            !insertmacro GET_STRING_TOKEN $2 2
            Pop $3 # save current library path into $3
    vdfCheckAppId:
        SetErrors
        StrCmp $4 "322170" 0 vdfReadLine
            ClearErrors # gd found in current library
    vdfDone:
        IfErrors gamePathError

    FileClose $1
    IfErrors gamePathError

    ${StrRep} $0 $3 "\\" "\"
    StrCpy $GamePath "$0\steamapps\common\Geometry Dash"

    # make sure game path is correct
    IfFileExists "$GamePath\*.exe" +2 0
    gamePathError:
        SetErrors
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd

Function DirectoryPageShow
    System::Call 'USER32::CreateWindowEx(i${__NSD_Label_EXSTYLE}, t"${__NSD_Label_CLASS}", t"", i${__NSD_Label_STYLE}, i0, i70, i400, i40, p$mui.DirectoryPage, p0, p0, p0)p.s'
    Pop $geode.DirectoryPage.ErrorText
    ShowWindow $geode.DirectoryPage.ErrorText 0
    SendMessage $mui.DirectoryPage ${WM_GETFONT} 0 0 $0
    SendMessage $geode.DirectoryPage.ErrorText ${WM_SETFONT} $0 1
    SetCtlColors $geode.DirectoryPage.ErrorText ff0000 transparent
    ; place the label at the top
    System::Call 'USER32::SetWindowPos(p$geode.DirectoryPage.ErrorText, p0, i0, i0, i0, i0, i3)i'
    Pop $0
FunctionEnd

Function .onVerifyInstDir
    LockWindow on

    ; check if there's any exe and libcocos2d.dll (GeometryDash.exe won't work because of GDPSes)
    IfFileExists $INSTDIR\*.exe 0 noGameNoLife
    IfFileExists $INSTDIR\libcocos2d.dll 0 noGameNoLife

    ; check if we're on 64-bit gd (checks for some of the DLLs introduced in 2.206)
    IfFileExists $INSTDIR\libpng16.dll 0 versionIssueImo
    IfFileExists $INSTDIR\pthreadVC3.dll 0 versionIssueImo
    IfFileExists $INSTDIR\libcrypto-3-x64.dll 0 versionIssueImo

    ; check if geode is already installed
    IfFileExists $INSTDIR\Geode.dll valid

    ; check mod loaders/mod menus
    IfFileExists $INSTDIR\hackpro.dll other_hackpro
    IfFileExists $INSTDIR\XInput1_4.dll other_xinput

    ; all checks passed
    valid:
        ShowWindow $geode.DirectoryPage.ErrorText 0
        LockWindow off
        Return

    noGameNoLife:
        SendMessage $geode.DirectoryPage.ErrorText ${WM_SETTEXT} "" "STR:$(GEODE_TEXT_GD_MISSING)"
        Goto error
    versionIssueImo:
        SendMessage $geode.DirectoryPage.ErrorText ${WM_SETTEXT} "" "STR:$(GEODE_TEXT_GD_OLD)"
        Goto error
    other_hackpro:
        StrCpy $0 "hackpro.dll"
        Goto other
    other_xinput:
        StrCpy $0 "XInput1_4.dll"
        Goto other
    other:
        ${StrRep} $0 $(GEODE_TEXT_MOD_LOADER_ALREADY_INSTALLED) "the dll trademark" $0
        SendMessage $geode.DirectoryPage.ErrorText ${WM_SETTEXT} "" "STR:$0"
        SetCtlColors $geode.DirectoryPage.ErrorText e25402 transparent
        LockWindow off
        Return

    error:
        ShowWindow $geode.DirectoryPage.ErrorText 1
        SetCtlColors $geode.DirectoryPage.ErrorText ff0000 transparent
        LockWindow off
        Abort
        Return
FunctionEnd

SectionGroup "Geode"
    Section "Loader" LOADER_SECTION
        SetOutPath $INSTDIR

        File ${BINDIR}\Geode.dll
        File ${BINDIR}\Geode.pdb
        File ${BINDIR}\GeodeUpdater.exe
        File ${BINDIR}\XInput1_4.dll

        RMdir /r $INSTDIR\geode\update
        RMdir /r $INSTDIR\geode\index
        Delete "$INSTDIR\xinput9_1_0.dll"
        Delete "$INSTDIR\xinput9_1_0.lib"
        Delete "$INSTDIR\xinput9_1_0.pdb"

        Delete "$INSTDIR\hackpro.dll"

        Delete "$INSTDIR\msvcp140.dll"
        Delete "$INSTDIR\msvcp140d.dll"
        Delete "$INSTDIR\vcruntime140.dll"
        Delete "$INSTDIR\vcruntime140d.dll"

        WriteUninstaller "GeodeUninstaller.exe"
    SectionEnd

    Section "Resources"
        CreateDirectory $INSTDIR\geode\resources\geode.loader
        SetOutPath $INSTDIR\geode\resources\geode.loader
        File /r ${BINDIR}\resources\*
    SectionEnd
SectionGroupEnd

; download vc redist in compile-time
!execute "pwsh -nol -noni -nop dl-vcr.ps1"
Section "Visual Studio Runtime"
    SetOutPath $INSTDIR
    File VC_redist.x64.exe
    ExecWait "$INSTDIR\VC_redist.x64.exe /install /quiet /norestart"
    Delete "$INSTDIR\VC_redist.x64.exe"
SectionEnd

Section "steam_appid.txt"
    SetOutPath $INSTDIR
    FileOpen $0 "$INSTDIR\steam_appid.txt" w
    FileWrite $0 "322170"
    FileClose $0
SectionEnd

Function .onInit
    !insertmacro MUI_LANGDLL_DISPLAY

    IntOp $0 ${SF_SELECTED} | ${SF_RO}
    SectionSetFlags ${LOADER_SECTION} $0

    Call FindGamePath
    IfErrors 0 +3
        StrCpy $GamePath ""
        Return
    StrCpy $INSTDIR "$GamePath\"
FunctionEnd

; uninstaller

Function un.onInit
    !insertmacro MUI_UNGETLANGUAGE

    ; verify uninst dir

    ; check if there's any exe and libcocos2d.dll (GeometryDash.exe won't work because of GDPSes)
    IfFileExists $INSTDIR\*.exe 0 invalid
    IfFileExists $INSTDIR\libcocos2d.dll 0 invalid

    ; check if xinput and geode exist
    IfFileExists $INSTDIR\XInput1_4.dll 0 invalid
    IfFileExists $INSTDIR\Geode.dll 0 invalid
        Return

    invalid:
        MessageBox MB_ICONSTOP|MB_OK $(GEODE_UNTEXT_GEODE_MISSING)
        Abort
FunctionEnd
Section "Uninstall"
    DeleteRegKey /ifempty HKCU "Software\Geode"
    Delete $INSTDIR\GeodeUninstaller.exe
    Delete $INSTDIR\Geode.dll
    Delete $INSTDIR\Geode.pdb
    Delete $INSTDIR\Geode.lib
    Delete $INSTDIR\GeodeUpdater.exe
    Delete $INSTDIR\XInput1_4.dll

    # default value of DATA is an empty string
    # if DATA is empty, keep user data
    # otherwise, delete the entire geode and DATA\geode\mods dirs
    # the reason we're deleting DATA\geode\mods instead of just passing
    # that dir directly to DATA is so that in case someone (either accidentally or maliciously)
    # passes the wrong directory, the uninstaller doesn't just blindly clear it
    # it will also check for the presence of CCGameManager.dat and CCLocalLevels.dat in DATA

    Push "DATA"
    Push ""
    Call un.GetParameterValue
    Pop $0
    StrCmp $0 "" keep_data remove_data

    keep_data:
        # keep configs, mods, logs and crash logs
        RMdir /r $INSTDIR\geode\index
        RMdir /r $INSTDIR\geode\resources
        RMdir /r $INSTDIR\geode\temp
        RMdir /r $INSTDIR\geode\unzipped
        RMdir /r $INSTDIR\geode\update
        Return

    remove_data:
        RMdir /r $INSTDIR\geode
        IfFileExists $0\CCGameManager.dat 0 invalid
        IfFileExists $0\CCLocalLevels.dat 0 invalid
        RMdir /r $0\geode\mods ; delete DATA\geode\mods
        RMdir $0\geode ; then delete DATA\geode non-recursively, assuming mods is the only directory in DATA\geode
        Return

    invalid:
        # this message doesnt rly need translatable as
        # its only supposed to be used internally by geode itself
        MessageBox MB_ICONSTOP|MB_OK "The path passed to DATA is not a valid Geometry Dash data folder!"
        Abort

SectionEnd
