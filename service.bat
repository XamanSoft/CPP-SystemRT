@echo OFF
echo Stopping old service version...
net stop "CppSystemRT"
echo Uninstalling old service version...
sc delete "CppSystemRT"

echo Installing service...
rem DO NOT remove the space after "binpath="!
sc create "CppSystemRT" binpath= "Debug\daemon-test.exe" start= auto
echo Starting server complete
pause
