!include LogicLib.nsh
!include MUI2.nsh

Unicode True

!define APPNAME "ConsoleUpdateExample"
!define COMPANYNAME "DesktopSoftwareUpdater"
!define DESCRIPTION "A short description goes here"

# These three must be integers
!define VERSIONMAJOR 0
!define VERSIONMINOR 0
!define VERSIONBUILD 1
!define VERSIONREVISION 0
!define INSTALLSIZE 1234
!define FILE_VERSION "${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}.${VERSIONREVISION}"
!define PRODUCT_VERSION "${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}.${VERSIONREVISION}"

;RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)
 
InstallDir "$LocalAppdata\${COMPANYNAME}\${APPNAME}"

# rtf or txt file - remember if it is txt, it must be in the DOS text format (\r\n)
LicenseData "../LICENSE"
# This will be in the installer/uninstaller's title bar
Name "${COMPANYNAME} - ${APPNAME}"
Icon "logo.ico"
outFile ${APPNAME}-installer.exe

VIAddVersionKey /LANG=0 "ProductName" "${APPNAME}"
VIAddVersionKey /LANG=0 "FileVersion" "${FILE_VERSION}"
VIAddVersionKey /LANG=0 "ProductVersion" "${VERSIONMAJOR}.${VERSIONMINOR}"
VIAddVersionKey /LANG=0 "LegalCopyright" "${COMPANYNAME}"
VIAddVersionKey /LANG=0 "FileDescription" "${APPNAME} installer"
VIProductVersion ${PRODUCT_VERSION}
VIFileVersion ${FILE_VERSION}

!define MUI_ICON "logo.ico"
!define MUI_UNICON "logo.ico"
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_TEXT "Launch GUI application"
!define MUI_FINISHPAGE_RUN_FUNCTION "StartGUIApplication"
!insertmacro MUI_PAGE_FINISH

function killProcessIfExists
	Pop $0
	StrCpy $1 $0
    nsProcess::_FindProcess "$1"
    Pop $R0
    ${If} $R0 = 0
      nsProcess::_KillProcess "$1"
      Pop $R0
      Sleep 500
    ${EndIf}
functionEnd

function .onInit
	Push "updateCUI.exe"
	call killProcessIfExists
	Push "updateCUIAllinOne.exe"
	call killProcessIfExists
	Push "updateWithPauses.exe"
	call killProcessIfExists
	Push "qml_example.exe"
	call killProcessIfExists
functionEnd
 
section "install"
	setOutPath $INSTDIR
	
	# Files added here should be removed by the uninstaller (see section "uninstall")
	file logo.ico
	file ..\LICENSE
	file /r ..\build\Release\*.*
!ifdef USE_64
	file /r ..\3d-party\appupdater\lib_x64
	file /r ..\qml_example\release\*.*
!else
	file /r ..\3d-party\appupdater\lib_x86
!endif

	# Uninstaller - See function un.onInit and section "uninstall" for configuration
	writeUninstaller "$INSTDIR\uninstall.exe"
 
	# Start Menu
	createDirectory "$SMPROGRAMS\${COMPANYNAME}"
	createShortCut "$SMPROGRAMS\${COMPANYNAME}\updateCUI.lnk" "$INSTDIR\updateCUI.exe" "" "$INSTDIR\logo.ico"
	createShortCut "$SMPROGRAMS\${COMPANYNAME}\updateCUIAllinOne.lnk" "$INSTDIR\updateCUIAllinOne.exe" "" "$INSTDIR\logo.ico"
	createShortCut "$SMPROGRAMS\${COMPANYNAME}\updateWithPauses.lnk" "$INSTDIR\updateWithPauses.exe" "" "$INSTDIR\logo.ico"
 	createShortCut "$SMPROGRAMS\${COMPANYNAME}\qml_update.lnk" "$INSTDIR\qml_example.exe" "" "$INSTDIR\logo.ico"

	# Registry information for add/remove programs
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayName" "${COMPANYNAME} - ${APPNAME} - ${DESCRIPTION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayIcon" "$\"$INSTDIR\logo.ico$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "Publisher" "$\"${COMPANYNAME}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayVersion" "$\"${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}$\""
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMinor" ${VERSIONMINOR}
	# There is no option for modifying or repairing the install
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoRepair" 1
	# Set the INSTALLSIZE constant (!defined at the top of this script) so Add/Remove Programs can accurately report the size
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "EstimatedSize" ${INSTALLSIZE}
sectionEnd
 
# Uninstaller
 
function un.onInit
functionEnd
 
section "uninstall"
	delete "$SMPROGRAMS\${COMPANYNAME}\updateCUI.lnk" 
	delete "$SMPROGRAMS\${COMPANYNAME}\updateCUIAllinOne.lnk"
	delete "$SMPROGRAMS\${COMPANYNAME}\updateWithPauses.lnk"	
	delete "$SMPROGRAMS\${COMPANYNAME}\qml_update.lnk"	
	rmDir "$SMPROGRAMS\${COMPANYNAME}"
 
	RMDir /r "$INSTDIR"

	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}"
sectionEnd

Function StartGUIApplication
	Exec "$INSTDIR\qml_example.exe AfterInstall"
FunctionEnd