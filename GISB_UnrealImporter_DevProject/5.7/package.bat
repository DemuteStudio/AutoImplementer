@echo off
echo ========================================
echo GISB_Importer Plugin Packager
echo ========================================
echo.
echo Packaging plugin for Unreal Engine 5.7...
echo Output: D:\AutoImplementing\.releases\Unreal 5.7\GISB_Importer
echo.

powershell -Command "& 'C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\RunUAT.bat' BuildPlugin -Plugin='D:\AutoImplementing\GISB_UnrealImporter_DevProject\5.7\Plugins\GISB_Importer\GISB_Importer.uplugin' -Package='D:\AutoImplementing\.releases\Unreal 5.7\GISB_Importer' -CreateSubFolder"

echo.
echo ========================================
if %ERRORLEVEL% EQU 0 (
    echo PACKAGING COMPLETED SUCCESSFULLY!
    echo.
    echo Plugin packaged to:
    echo D:\AutoImplementing\.releases\Unreal 5.7\GISB_Importer
) else (
    echo PACKAGING FAILED!
    echo Check the output above for errors.
)
echo ========================================
echo.
pause
