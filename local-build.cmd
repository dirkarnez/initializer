REM run as Administrator
@echo off

cd /d %~dp0
@REM set DOWNLOAD_DIR=%USERPROFILE%\Downloads
@REM set DOWNLOAD_DIR_LINUX=%DOWNLOAD_DIR:\=/%
@REM SET PATH=^
@REM %DOWNLOAD_DIR%\PortableGit\bin;^
@REM %DOWNLOAD_DIR%\x86_64-8.1.0-release-win32-seh-rt_v6-rev0;^
@REM %DOWNLOAD_DIR%\x86_64-8.1.0-release-win32-seh-rt_v6-rev0\bin;^
@REM %DOWNLOAD_DIR%\cmake-3.22.2-windows-x86_64\bin;^

set PATH=^
D:\Softwares\x86_64-8.1.0-release-posix-seh-rt_v6-rev0\mingw64;^
D:\Softwares\x86_64-8.1.0-release-posix-seh-rt_v6-rev0\mingw64\bin;^
D:\Softwares\cmake-3.23.0-rc1-windows-x86_64\bin;

cmake.exe -G"MinGW Makefiles" ^
-DBOOST_ROOT="D:/Code Projects/cpp-libraries/boost-1.78.0-windows-2019-mingw-static/lib/cmake/Boost-1.78.0" ^
-DYAMLCPP_ROOT="D:/Code Projects/cpp-libraries/yaml-cpp-v0.7.0-x86_64-posix-seh-rev0-8.1.0" ^
-DCMAKE_BUILD_TYPE=Debug -B./build &&^
cd build &&^
cmake --build . &&^
echo "Successful build"
pause
