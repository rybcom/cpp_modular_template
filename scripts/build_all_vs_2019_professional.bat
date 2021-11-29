
@echo off
pushd %~dp0\

title solution 2019 professional DEBUG
call build_solution_debug_vs_2019_professional.bat no_pause

title solution 2019 professional PROFILER
call build_solution_profile_vs_2019_professional.bat no_pause

title solution 2019 professional RELEASE
call build_solution_release_vs_2019_professional.bat no_pause


echo        ---------- build finished ----------

if not "%1" == "no_pause" (
PAUSE
)

popd
