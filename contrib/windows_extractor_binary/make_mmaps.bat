@echo off
cls
echo.
echo                    Welcome to the Move Map Gen
echo.
echo You need 2GB of free space in disk, CTRL+C to stop process
echo Hit Enter to start . . .
pause>nul
cls
echo.
echo.
echo.
md mmaps
movements_extractor.exe --silent --offMeshInput offmesh.txt
cls
echo.
echo.
echo.
echo Process done! copy mmaps folder to the Emulator main directory
echo Press any key to exit . . .
pause>nul
