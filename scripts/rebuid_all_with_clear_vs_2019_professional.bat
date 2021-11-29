rem clear 

timeout /t 3 
call clear.bat no_pause

rem generate solution

timeout /t 3 
call generate_solution.bat no_pause

rem build all

timeout /t 3 
call build_all_vs_2019_professional.bat no_pause


if not "%1" == "no_pause" (
pause
)


