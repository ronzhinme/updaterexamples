$QDIR=$args[0]+"\bin"
$QMAKE=$args[0]+"\bin\qmake.exe"
$QTDEPLOY=$args[0]+"\bin\windeployqt.exe"
$BUILD_TOOL=$args[1]
$ARCH=$args[2]
$PLATFORM=$args[3]

echo $QMAKE $QTDEPLOY $BUILD_TOOL $ARCH $PLATFORM

echo "======== updateExample building... ========"
mkdir build
cd build
cmake -G "$BUILD_TOOL" -A $PLATFORM../
cd ..
cmake --build build --config Release

if( $ARCH -ne "x86" )
{
    echo "======== qml_example building... ========"
    cmd /C "$QDIR\qtenv2.bat & vcvarsall $ARCH & cd /d $PWD/qml_example & $QMAKE 'CONFIG+=release' & nmake"
}

echo "======== Copy libs... ========"
Copy-Item -Path "./3d-party/appupdater/lib_$ARCH/*" -Destination "./build/release" -Recurse -Verbose

if( $ARCH -ne "x86" )
{
    Copy-Item -Path "./3d-party/appupdater/lib_$ARCH/*" -Destination "./qml_example/release" -Recurse -Verbose
    echo "======== prepare deploy... ========"
    . $QTDEPLOY --qmldir ./qml_example ./qml_example/release
}
