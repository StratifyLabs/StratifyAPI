@echo off
set DUMMY=%1
set HOST=%2
set ABSTRACTION=%3
set ARCH=%4
set TOOLSPATH=%5
echo "Installing include files in %TOOLSPATH%\%HOST%\include"
mkdir %TOOLSPATH%\%HOST%\include\applib
copy /y ..\include\* %TOOLSPATH%\%HOST%\include\applib
echo "Installing library file: libapp_%ABSTRACTION%_%ARCH%.a in %TOOLSPATH%\%HOST%\lib"
copy /y libapp_%ABSTRACTION%_%ARCH%.a %TOOLSPATH%\%HOST%\lib
exit /b 1
