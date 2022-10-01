SET VCPKG_TOOLCHAIN=vcpkg/scripts/buildsystems/vcpkg.cmake

mkdir BUILD
cd BUILD

cmake .. -DCMAKE_TOOLCHAIN_FILE=../%VCPKG_TOOLCHAIN%
cmake --build . 

pause