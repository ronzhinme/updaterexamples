$APPUPDATERLIB_ARCHIVE=$args[0]
$LIB_URL=$args[1]

echo $APPUPDATERLIB_ARCHIVE $LIB_URL

git submodule update --init --recursive
curl -o "$APPUPDATERLIB_ARCHIVE" "$LIB_URL"
7z x $APPUPDATERLIB_ARCHIVE
mv ./appupdater ./3d-party