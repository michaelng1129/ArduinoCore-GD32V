cmake_minimum_required(VERSION 3.15)
set (CMAKE_C_COMPILER_FORCED true)
set (CMAKE_CXX_COMPILER_FORCED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(GD32W55X LANGUAGES C ASM CXX)

set(CONFIG_OS                      "FREERTOS"    CACHE STRING    "Select which OS")
set(CONFIG_BLE_FEATURE             "MAX"         CACHE STRING    "Select which BLE CONFIG")
set(CONFIG_MBEDTLS_VERSION         "3.6.2"       CACHE STRING    "Select which mbedtls version")
# set(CONFIG_OS                      "RTTHREAD"    CACHE STRING    "Select which OS")
set(CONFIG_STATIC_LIB_BUILDED         ON            CACHE BOOL      "Whether static lib builded, like wlan lib")
set(CONFIG_HEADER_COPY                OFF            CACHE BOOL      "Enable header file copy to build/bin/include")


include(${CMAKE_TOOLCHAIN_FILE})
toolchain_reload_compiler()

include(scripts/cmake/utils.cmake)


set(PROJECT_INCLUDE_DIRS
    config
    ROM-EXPORT/bootloader
    ROM-EXPORT/halcomm
    MSDK/plf/riscv/gd32vw55x
    MSDK/plf/riscv/NMSIS/Core/Include
    MSDK/plf/GD32VW55x_standard_peripheral/Include
    MSDK/plf/GD32VW55x_standard_peripheral
    MSDK/plf/src
    MSDK/macsw/export
    MSDK/app
    MSDK/util/include
)

include_directories(${PROJECT_INCLUDE_DIRS})


include_directories(MBL/mainboot)
add_subdirectory(MBL/mainboot)

add_subdirectory(MSDK)



setup_header_copy_targets(MSDK ${PROJECT_INCLUDE_DIRS})