@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"
set "BUILD_DIR=%SCRIPT_DIR%\cmake_build"
set "TOOLCHAIN_FILE=%SCRIPT_DIR%\scripts\cmake\toolchain.cmake"
set "CMAKE_GENERATOR=Unix Makefiles"
set "DEFAULT_MAKEFLAGS=--output-sync=target"
set "LOCAL_TOOLCHAIN_BIN=%SCRIPT_DIR%\tools\gd32vw55x_toolchain_windows\bin"
set "LOCAL_OPENOCD_BIN=%SCRIPT_DIR%\tools\xpack-openocd-0.11.0-3_windows\bin"

REM set custom OpenOCD path
::set OPENOCD_PATH=path/to/openocd/bin
REM set custom Toolchain path
::set TOOLCHAIN_PATH=path/to/toolchain/bin


IF NOT "%1"=="" (
    SET APP=%1
    IF NOT "%2"=="" (
        SET USER_CMD=%2
    ) ELSE (
        SET USER_CMD=-j
    )
) ELSE (
    SET APP=app
    SET USER_CMD=-j
)


IF NOT EXIST "%SCRIPT_DIR%\MSDK\%APP%" (
    echo build app error: %SCRIPT_DIR%\MSDK\%APP% does not exist !!
    EXIT /B 1
)

REM toolchain check and setup
:: Prefer the repo-local toolchain first
if EXIST "%LOCAL_TOOLCHAIN_BIN%\riscv-nuclei-elf-gcc.exe" (
    echo Using repo toolchain path: %LOCAL_TOOLCHAIN_BIN%
    SET "PATH=%LOCAL_TOOLCHAIN_BIN%;%PATH%"
    goto toolchain_done
)

if EXIST "%SCRIPT_DIR%\tools\gd32vw55x_toolchain_windows.7z.001" (
    echo Unzipping gd32vw55x toolchain .......
    "%PROGRAMFILES%\7-Zip\7z.exe" x "%SCRIPT_DIR%\tools\gd32vw55x_toolchain_windows.7z.001" -o"%SCRIPT_DIR%\tools"
    if EXIST "%LOCAL_TOOLCHAIN_BIN%\riscv-nuclei-elf-gcc.exe" (
        echo Using repo toolchain path: %LOCAL_TOOLCHAIN_BIN%
        SET "PATH=%LOCAL_TOOLCHAIN_BIN%;%PATH%"
        goto toolchain_done
    )
)

:: Fallback to a custom toolchain path
if NOT "%TOOLCHAIN_PATH%"=="" (
    if EXIST "%TOOLCHAIN_PATH%" (
        echo Using custom toolchain path: %TOOLCHAIN_PATH%
        SET "PATH=%TOOLCHAIN_PATH%;%PATH%"
        goto toolchain_done
    )
)

:: Finally, use whatever is already in PATH
where riscv-nuclei-elf-gcc >NUL 2>&1

if ERRORLEVEL 1 (
    echo Please download the gd32vw55x toolchain into tools or put it in PATH
    EXIT /B 1
) else (
    for /f "delims=" %%i in ('where riscv-nuclei-elf-gcc') do (
        echo Toolchain found in PATH: %%i
        goto toolchain_done
    )
)

:toolchain_done

REM OpenOCD check and setup
:: Prefer the repo-local OpenOCD first
if EXIST "%LOCAL_OPENOCD_BIN%\openocd.exe" (
    echo Using repo OpenOCD path: %LOCAL_OPENOCD_BIN%
    SET "PATH=%LOCAL_OPENOCD_BIN%;%PATH%"
    goto openocd_done
)

if EXIST "%SCRIPT_DIR%\tools\xpack-openocd-0.11.0-3_windows.7z" (
    echo Unzipping gd32vw55x OpenOCD .......
    "%PROGRAMFILES%\7-Zip\7z.exe" x "%SCRIPT_DIR%\tools\xpack-openocd-0.11.0-3_windows.7z" -o"%SCRIPT_DIR%\tools"
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

set "MAKE_EXE="
for /f "delims=" %%i in ('where make 2^>NUL') do (
    if not defined MAKE_EXE (
        set "MAKE_EXE=%%i"
    )
)

if not defined MAKE_EXE (
    echo Could not find make.exe in PATH.
    echo Ensure the GD32VW55x toolchain bin directory is available in PATH.
    EXIT /B 1
)

if NOT EXIST "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

if "%USER_CMD%"=="clean" (
    pushd "%BUILD_DIR%"
    DEL /S /Q *.* 2>NUL
    FOR /D %%D IN (*.*) DO (
        RD /S /Q "%%D" 2>NUL
    )
    popd
) else (
    :: Configure
    cmake -S "%SCRIPT_DIR%" -B "%BUILD_DIR%" -G "%CMAKE_GENERATOR%" "-DCMAKE_MAKE_PROGRAM:FILEPATH=%MAKE_EXE%" -DAPP=%APP% -DCONFIG_BLE_FEATURE=MAX -DCONFIG_MBEDTLS_VERSION=3.6.2 --toolchain "%TOOLCHAIN_FILE%"

    :: Make
    if defined MAKEFLAGS (
        set "MAKEFLAGS=%DEFAULT_MAKEFLAGS% %MAKEFLAGS%"
    ) else (
        set "MAKEFLAGS=%DEFAULT_MAKEFLAGS%"
    )
    cmake --build "%BUILD_DIR%" -- %USER_CMD%
)
if ERRORLEVEL 2 pause
:end
endlocal
