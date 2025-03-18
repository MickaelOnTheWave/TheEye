#!/bin/bash

source "BuildTools.sh"

buildDir="../build/officialBuild"

createFolderIfNotPresent $buildDir
cmake -S ../  -B $buildDir -DCMAKE_BUILD_TYPE:STRING=Release -DUSE_SYSTEM_DATA=ON
cmake --build $buildDir
