@echo off
pushd %~dp0\..\

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" "solution.sln" /t:Build /p:Configuration=profiler
popd

if not "%1" == "no_pause" (
PAUSE
)

