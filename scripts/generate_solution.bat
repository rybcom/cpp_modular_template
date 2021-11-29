@echo off
pushd %~dp0\..\

call tools\premake\premake5.exe --file=workspace.lua vs2019

popd

if not "%1" == "no_pause" (
PAUSE
)

