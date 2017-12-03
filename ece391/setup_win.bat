@title ECE391 Localized Development Environment by DukeGaGa
@mode con: cols=66 lines=30
@echo off

echo.
echo  ################################################################
echo  #                                                              #
echo  #            ECE391 Localized Development Environment          #
echo  #                                                              #
echo  #                     Created by Fei Deng                      #
echo  #                                                              #
echo  #                          2017.01.14                          #
echo  #                                                              #
echo  ################################################################

echo.
echo.
echo Getting current directory and generating scripts
call setup_scripts_win\get_dir.vbs

echo.
echo.
echo Attempting to create devel and desktop shortcuts
echo.
call temp.bat
call temp.vbs

echo.
echo.
echo Cleaning up
del temp.bat
del temp.vbs

echo.
echo.
pause
