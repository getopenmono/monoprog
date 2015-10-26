!include "EnvVarUpdate.nsh"

Name "Monoprog"
# TODO: Find out how to use $%BUILDDIR%, $%RELEASEDIR% and %VERSION%.
OutFile "..\..\bin\MonoprogSetup-v0.6.0-x86.exe"
InstallDir "$PROGRAMFILES\OpenMono"

Section "Install"
	SetOutPath $INSTDIR
	File "..\..\bin\release\monoprog.exe"
	# TODO: Find out how to extract the paths from environment.
	File "C:\Qt\Qt5.5.0\5.5\msvc2010\bin\Qt5Core.dll"
	File "C:\windows\system32\MSVCP100.DLL"
	File "C:\windows\system32\MSVCR100.DLL"
	${EnvVarUpdate} $0 "PATH" "A" "HKLM" "$INSTDIR"
	WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Uninstall"
	${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$INSTDIR"
	Delete "$INSTDIR\Qt5Core.dll"
	Delete "$INSTDIR\MSVCP100.DLL"
	Delete "$INSTDIR\MSVCR100.DLL"
	Delete "$INSTDIR\monoprog.exe"
	Delete "$INSTDIR\Uninstall.exe"
	RMDir "$INSTDIR"
SectionEnd
