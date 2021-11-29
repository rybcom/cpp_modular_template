
@echo off
pushd %~dp0\

title solution 2019 community DEBUG
call build_solution_debug_vs_2019_community.bat no_pause


title solution 2019 community PROFILER
call build_solution_profile_vs_2019_community.bat no_pause


title solution 2019 community RELEASE
call build_solution_release_vs_2019_community.bat no_pause


echo        ---------- build finished ----------

if not "%1" == "no_pause" (
PAUSE
)

popd
