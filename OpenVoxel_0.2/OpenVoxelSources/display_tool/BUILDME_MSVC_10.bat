@echo on
call build_env_msvc10.bat
qmake
del moc*
del *.o
del *.pdb
nmake distclean
qmake
nmake release
