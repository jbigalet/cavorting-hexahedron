FOR /F "usebackq tokens=2,*" %%A IN (`REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7" /v 15.0`) DO (set root=%%B)
call "%root%\VC\Auxiliary\Build\vcvarsall.bat" x64
cd /D "%~dp0%"
3rdp\make.exe %*
