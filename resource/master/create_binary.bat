@echo off

:: ------------------------------
:: �ϐ���`
:: ------------------------------
set TOOL=%CD%\..\..\Tool\AutoMaster\AutoMaster.exe
set XLSX_DIR=%CD%\master
set BINARY_DIR=%CD%\..\..\data\master


:: ------------------------------
:: �o�C�i���f�B���N�g������
:: ------------------------------
if exist %BINARY_DIR% rmdir /S /Q %BINARY_DIR%
mkdir %BINARY_DIR%


:: ------------------------------
:: �}�X�^�[�f�[�^�쐬
:: ------------------------------
%TOOL% -x %XLSX_DIR% -b %BINARY_DIR%
