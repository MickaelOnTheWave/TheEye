#!/bin/bash

source "BuildTools.sh"

PROJECT_BINARY=eye
PACKAGE_VERSION=1
AUTHOR_NAME="Mickaël C. Guimarães"
AUTHOR_EMAIL="mickael.dacruz@gmail.com"

PROJECT_ROOT=$PWD"/.."
PROJECT_VERSION=`cat "$PROJECT_ROOT/VERSION"`
PACKAGE_ROOT="build/debPackage"
PACKAGE_NAME="$PROJECT_BINARY"_"$PROJECT_VERSION"-"$PACKAGE_VERSION"

BIN_FOLDER=$PACKAGE_NAME/usr/local/bin
CONTROL_FOLDER=$PACKAGE_NAME/DEBIAN
DATA_FOLDER=$PACKAGE_NAME/usr/share/$PROJECT_BINARY/data/

createPackageStructure()
{
    cdAndCreateIfNotPresent "../$PACKAGE_ROOT"
	removeFolderIfPresent $PACKAGE_NAME
	mkdir $PACKAGE_NAME
	mkdir $PACKAGE_NAME/usr
	mkdir $PACKAGE_NAME/usr/local
        mkdir $PACKAGE_NAME/usr/share
        mkdir $PACKAGE_NAME/usr/share/$PROJECT_BINARY
	mkdir $BIN_FOLDER
	mkdir $CONTROL_FOLDER
        mkdir $DATA_FOLDER
}

copyFilesToStructure()
{
    cd $PROJECT_ROOT
    cp build/officialBuild/TheEye $PACKAGE_ROOT/$BIN_FOLDER/eyeScreensaver
    cp build/officialBuild/data/eye-blue.jpg $PACKAGE_ROOT/$DATA_FOLDER/eye-blue.jpg
    cp build/officialBuild/data/basic.vert $PACKAGE_ROOT/$DATA_FOLDER/basic.vert
    cp build/officialBuild/data/singleTexture.frag $PACKAGE_ROOT/$DATA_FOLDER/singleTexture.frag
}

createDebianControlFile()
{
    CONTROL_FILE=$PACKAGE_ROOT/$CONTROL_FOLDER/control

    echo "Package: $PROJECT_BINARY" >> $CONTROL_FILE
    echo "Version: $PROJECT_VERSION-$PACKAGE_VERSION" >> $CONTROL_FILE
    echo "Architecture: amd64" >> $CONTROL_FILE
    #echo "Depends: libsomethingorrather (>= 1.2.13), anotherDependency (>= 1.2.6)" >> $CONTROL_FILE
    echo "Maintainer: $AUTHOR_NAME <$AUTHOR_EMAIL>" >> $CONTROL_FILE
    echo "Description: Eye Screensaver" >> $CONTROL_FILE
    echo " A screensaver that watches you when you are idle." >> $CONTROL_FILE
}

createPackage()
{
    cd $PACKAGE_ROOT
    dpkg-deb --build $PACKAGE_NAME
}

createPackageStructure
copyFilesToStructure
createDebianControlFile
createPackage
