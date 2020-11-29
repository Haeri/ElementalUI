@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------



cd ..
if "%1" == "-static" (
	xcopy ".\build\Release\elem.lib" ".\lib\elem-static.lib*"
) else (
	xcopy ".\build\Release\elem.dll" ".\bin\"
	xcopy ".\build\Release\elem.lib" ".\lib\"
)
set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" if not "%2" == "-s" pause
exit /b %err%