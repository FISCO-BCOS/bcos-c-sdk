#!/bin/bash
dirpath="$(cd "$(dirname "$0")" && pwd)"
cd ${dirpath}/../
# set -e

# TODO: check java env

TARGET_LIB_NAME="bcos-sdk-jni"
METAINFO_NATIVE_PATH="${dirpath}/../src/main/resources/META-INF/native/"

echo " # Compile JNI dynamic library, Starting ==>> "

cd src/main/c/

[ -d build ] &&  {
    rm -rf build
    echo " == clean build "
}

[ -d bcos-cmake-scripts ] &&  {
    rm -rf bcos-cmake-scripts
    echo " == clean bcos-cmake-scripts "
}

export CFLAGS="${CFLAGS} -fPIC"
export CXXFLAGS="${CXXFLAGS} -fPIC"

CMAKE_COMMAND='cmake'

# check if cmake3 install
type cmake3
if [ $? -eq 0 ]; then
    CMAKE_COMMAND='cmake3' 
fi

mkdir -p build && cd build
${CMAKE_COMMAND} ../
make -j4

ls  | egrep -o ${TARGET_LIB_NAME}
if [ $? -eq 0 ]; then
    echo "Compile JNI dynamic library successfully."
    mkdir -p ${METAINFO_NATIVE_PATH}
    cp *${TARGET_LIB_NAME}* ${METAINFO_NATIVE_PATH}
else
    echo "Compile JNI dynamic library failed."
    exit 1
fi

echo " # Compile JNI dynamic library, Ending   <<== " 