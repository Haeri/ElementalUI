#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
root_path=$(pwd)
rm -rf build
mkdir build
cd build

build_type=""

if [ "$1" = "-static" ]; then
	build_type="-DBUILD_SHARED_LIBS=OFF"
fi

cmake .. -DVCPKG_TARGET_TRIPLET=x64-osx -DVCPKG_OVERLAY_PORTS=$root_path"/external/custom-ports"  -DCMAKE_TOOLCHAIN_FILE=$root_path"/external/vcpkg/scripts/buildsystems/vcpkg.cmake" $build_type
err=$?

if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path