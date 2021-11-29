@echo off
pushd %~dp0\..\

RMDIR "intermediate" /S /Q
RMDIR "bin" /S /Q
RMDIR ".vs" /S /Q
del "solution.sln" /Q

popd

if not "%1" == "no_pause" (
PAUSE
)
