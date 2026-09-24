@echo off
REM Unified firmware download script
REM Usage: download.bat [TARGET] [DEBUGGER]
REM TARGET: MSDK, MBL, ALL
REM DEBUGGER: GDLink, JLink

setlocal enabledelayedexpansion

set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"
set "WORK_DIR=%SCRIPT_DIR%\.."
for %%I in ("%WORK_DIR%") do set "WORK_DIR=%%~fI"
set "LOCAL_OPENOCD_BIN=%WORK_DIR%\tools\xpack-openocd-0.11.0-3_windows\bin"

set "TARGET=%~1"
set "DEBUGGER=%~2"

REM set custom OpenOCD path
::set OPENOCD_PATH=path/to/openocd/bin

REM OpenOCD check and setup
:: Prefer the repo-local OpenOCD first
if EXIST "%LOCAL_OPENOCD_BIN%\openocd.exe" (
    echo Using repo OpenOCD path: %LOCAL_OPENOCD_BIN%
    SET "PATH=%LOCAL_OPENOCD_BIN%;%PATH%"
    goto openocd_done
)

if EXIST "%WORK_DIR%\tools\xpack-openocd-0.11.0-3_windows.7z" (
    echo Unzipping gd32vw55x OpenOCD .......
    "%PROGRAMFILES%\7-Zip\7z.exe" x "%WORK_DIR%\tools\xpack-openocd-0.11.0-3_windows.7z" -o"%WORK_DIR%\tools"
    if EXIST "%LOCAL_OPENOCD_BIN%\openocd.exe" (
        echo Using repo OpenOCD path: %LOCAL_OPENOCD_BIN%
        SET "PATH=%LOCAL_OPENOCD_BIN%;%PATH%"
        goto openocd_done
    )
)

:: Fallback to a custom OpenOCD path
if NOT "%OPENOCD_PATH%"=="" (
    if EXIST "%OPENOCD_PATH%" (
        echo Using custom OpenOCD path: %OPENOCD_PATH%
        SET "PATH=%OPENOCD_PATH%;%PATH%"
        goto openocd_done
    )
)

:: Finally, use whatever is already in PATH
where openocd >NUL 2>&1

if ERRORLEVEL 1 (
    echo Please download the gd32vw55x OpenOCD into tools or put it in PATH
    EXIT /B 1
) else (
    for /f "delims=" %%i in ('where openocd') do (
        echo OpenOCD found in PATH: %%i
        goto openocd_done
    )
)

:openocd_done

REM Default to MSDK if target not specified
if "%TARGET%"=="" set TARGET=MSDK
REM Default to GDLink if debugger not specified
if "%DEBUGGER%"=="" set DEBUGGER=GDLink

REM Validate target
if /i not "%TARGET%"=="MSDK" if /i not "%TARGET%"=="MBL" if /i not "%TARGET%"=="ALL" (
    echo Error: Invalid TARGET. Usage: download.bat [MSDK^|MBL^|ALL] [GDLink^|JLink]
    exit /b 1
)

REM Validate debugger
if /i not "%DEBUGGER%"=="GDLink" if /i not "%DEBUGGER%"=="JLink" (
    echo Error: Invalid DEBUGGER. Usage: download.bat [MSDK^|MBL^|ALL] [GDLink^|JLink]
    exit /b 1
)

REM Set binary file and flash address based on target
if /i "%TARGET%"=="MSDK" (
    echo Downloading MSDK via !DEBUGGER!...
    set BIN_FILE=!WORK_DIR!\MSDK\projects\cmake\output\bin\msdk.bin
    set FLASH_ADDR=0x0800A000
) else if /i "%TARGET%"=="MBL" (
    echo Downloading MBL via !DEBUGGER!...
    set BIN_FILE=!WORK_DIR!\MBL\project\cmake\bin\mbl.bin
    set FLASH_ADDR=0x08000000
) else if /i "%TARGET%"=="ALL" (
    echo Downloading ALL via !DEBUGGER!...
    set BIN_FILE=!WORK_DIR!\scripts\images\image-all.bin
    set FLASH_ADDR=0x08000000
)

REM Set OpenOCD config based on debugger
if /i "%DEBUGGER%"=="GDLink" (
    set CONFIG_FILE=!WORK_DIR!\MSDK\projects\eclipse\msdk\openocd_gdlink.cfg
) else if /i "%DEBUGGER%"=="JLink" (
    set CONFIG_FILE=!WORK_DIR!\MSDK\projects\eclipse\msdk\openocd_jlink.cfg
)

REM Convert backslashes to forward slashes for OpenOCD compatibility
set BIN_FILE=!BIN_FILE:\=/!
set CONFIG_FILE=!CONFIG_FILE:\=/!

echo.
echo Target: %TARGET%
echo Debugger: %DEBUGGER%
echo Binary: !BIN_FILE!
echo Flash Address: %FLASH_ADDR%
echo.

REM Run OpenOCD with appropriate config
set "OPENOCD_EXE=openocd.exe"
for /f "delims=" %%i in ('where openocd 2^>NUL') do (
    if /i "%%~nxi"=="openocd.exe" if not defined OPENOCD_RESOLVED (
        set "OPENOCD_RESOLVED=%%i"
    )
)
if defined OPENOCD_RESOLVED set "OPENOCD_EXE=!OPENOCD_RESOLVED!"

"!OPENOCD_EXE!" -f "!CONFIG_FILE!" -c "init" -c "program \"!BIN_FILE!\" %FLASH_ADDR% verify reset" -c "exit"

if errorlevel 1 (
    echo.
    echo Download failed!
    exit /b 1
) else (
    echo.
    echo Download completed successfully!
)
