!include "EnvVarUpdate.nsh"

Name "Monoprog"
OutFile "$%BUILDDIR%\MonoprogSetup-v$%VERSION%-x86.exe"
InstallDir "$PROGRAMFILES\OpenMono"

;Request application privileges for Windows Vista, 7, 8
RequestExecutionLevel admin

!include LogicLib.nsh

Function .onInit
UserInfo::GetAccountType
pop $0
${If} $0 != "admin" ;Require admin rights on NT4+
    MessageBox mb_iconstop "Administrator rights required!"
    SetErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
    Quit
${EndIf}
FunctionEnd

Section "Install"
	SetOutPath $INSTDIR
	File "$%RELEASEDIR%\monoprog.exe"
	File "$%RELEASEDIR%\*.dll"
	File "C:\windows\system32\MSVCP100.DLL"
	File "C:\windows\system32\MSVCR100.DLL"
	File "USBUART_cdc.inf"
	${EnvVarUpdate} $0 "PATH" "A" "HKLM" "$INSTDIR"

	!include x64.nsh
	${DisableX64FSRedirection}
	nsExec::ExecToLog '"$SYSDIR\PnPutil.exe" /a "$INSTDIR\USBUART_cdc.inf"' $0
	${EnableX64FSRedirection}

	WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Uninstall"
	${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$INSTDIR"
	Delete "$INSTDIR\*.dll"
	Delete "$INSTDIR\*.exe"
	Delete "$INSTDIR\*.inf"
	RMDir "$INSTDIR"
SectionEnd
