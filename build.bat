@echo off

cls

REM Default build type is debug
set BUILD_TYPE=-DCMAKE_BUILD_TYPE=Debug

REM Override the default build type if arg is given
if not "%1"=="" (
	if /i "%1"=="release" set BUILD_TYPE=-DCMAKE_BUILD_TYPE=Release
)

cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static -DOPENSSL_ROOT_DIR=C:/vcpkg/installed/x64-windows-static %BUILD_TYPE% .
if errorlevel 1 (
	echo CMake configuration failed!
	exit /b 1
)

ninja
if errorlevel 1 (
	echo Ninja build failed!
	exit /b 1
)

cls

.\bin\BepInFetch.exe
