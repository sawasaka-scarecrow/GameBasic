@echo off

:: ------------------------------
:: �ϐ���`
:: ------------------------------
set TOOL=%CD%\..\..\Tool\AutoMaster\AutoMaster.exe
set XLSX_DIR=%CD%\master
set XLSX_EDIT_DIR=%CD%\master_edit
set BINARY_DIR=%CD%\..\..\data\master
set SOURCE_DIR=%CD%\..\..\source\master


:: ------------------------------
:: �G�N�Z���f�[�^�R�s�[
:: ------------------------------
if exist %XLSX_DIR% rmdir /S /Q %XLSX_DIR%
xcopy /I %XLSX_EDIT_DIR% %XLSX_DIR%


:: ------------------------------
:: �o�C�i���f�B���N�g������
:: ------------------------------
if exist %BINARY_DIR% rmdir /S /Q %BINARY_DIR%
mkdir %BINARY_DIR%


:: ------------------------------
:: �\�[�X�f�B���N�g������
:: ------------------------------
if exist %SOURCE_DIR% rmdir /S /Q %SOURCE_DIR%
mkdir %SOURCE_DIR%


:: ------------------------------
:: �}�X�^�[�f�[�^�쐬
:: ------------------------------
%TOOL% -x %XLSX_DIR% -b %BINARY_DIR% -s %SOURCE_DIR%

pause
