echo off
echo compiling resource package...
cd ..\unpacked
..\tools\extra_rcc_win_x64.exe -o tmp.qrc --project
..\tools\extra_rcc_win_x64.exe --binary -o ..\resources.dat tmp.qrc
del tmp.qrc
cd ..\tools
echo packed data generated. Move 'resources.dat' to bin\ folder relative to poker_client.exe file
pause