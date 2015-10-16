!include "EnvVarUpdate.nsh"

Name "Monoprog"
OutFile "..\..\build\MonoprogSetup.exe"
InstallDir "$PROGRAMFILES\OpenMono"

Section "Install"
	SetOutPath $INSTDIR
	File "..\..\build\debug\monoprog.exe"
	${EnvVarUpdate} $0 "PATH" "A" "HKLM" "$PROGRAMFILES\OpenMono"
	WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Uninstall"
	${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$PROGRAMFILES\OpenMono"   
	Delete "$INSTDIR\Uninstall.exe"
	Delete "$INSTDIR\monoprog.exe"
	RMDir "$INSTDIR"
SectionEnd
