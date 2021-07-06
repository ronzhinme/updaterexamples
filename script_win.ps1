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

echo "======== qml_example building... ========"
cmd /C "$QDIR\qtenv2.bat & vcvarsall $ARCH & cd /d $PWD/qml_example & $QMAKE 'CONFIG+=release' & nmake"

echo "======== prepare deploy... ========"
cp ./3d-party/appupdater/lib_$ARCH/* ./release
cp ./3d-party/appupdater/lib_$ARCH/* ./qml_example/release
. $QTDEPLOY --qmldir ./qml_example ./qml_example/release
