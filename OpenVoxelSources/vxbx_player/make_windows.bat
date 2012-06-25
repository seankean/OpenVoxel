@echo on
call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
set PATH=%PATH%;c:\Qt\4.8.2\bin
qmake
del moc*
del *.o
del *.pdb
nmake distclean
qmake
nmake release
