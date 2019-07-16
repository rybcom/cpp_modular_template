rem clear 


cd ..
RMDIR "intermediate" /S /Q
RMDIR "bin" /S /Q
RMDIR "run" /S /Q
RMDIR ".vs" /S /Q
del "solution.sln" /Q

rem generate solution

timeout /t 3 /nobreak
call tools\premake\premake5.exe --file=workspace.lua vs2017

rem build all

timeout /t 3 /nobreak
cd scripts 
call build_all.bat

