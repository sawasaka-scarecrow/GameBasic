@echo off

:: ------------------------------
:: �ϐ���`
:: ------------------------------
set TOOL=%CD%\..\..\Tool\FilePacker\FilePacker.exe
set SRC=%CD%\src
set DST_DIR=%CD%\..\..\data\image
set DST=%DST_DIR%\image.dat

:: ------------------------------
:: �}�X�^�[�f�[�^�쐬
:: ------------------------------
rmdir /S /Q %DST_DIR%
mkdir %DST_DIR%
%TOOL% -i %SRC% -o %DST%

pause
