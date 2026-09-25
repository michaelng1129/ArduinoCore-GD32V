cmake_minimum_required(VERSION 3.15)

if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/system/gd32vw55x/sdk/scripts/cmake/toolchain.cmake"
        CACHE FILEPATH "GD32VW55x RISC-V toolchain file")
endif()

set(CMAKE_C_COMPILER_FORCED true)
set(CMAKE_CXX_COMPILER_FORCED true)

project(ArduinoCoreGD32VW55x LANGUAGES C ASM CXX)

option(BUILD_ARDUINO_CORE "Build the Arduino firmware target" ON)
set(ARDUINO_BOARD "START" CACHE STRING "Arduino board variant: START or EVAL")
set_property(CACHE ARDUINO_BOARD PROPERTY STRINGS START EVAL)
set(ARDUINO_SKETCH "" CACHE FILEPATH "Path to a C++ sketch source that defines setup() and loop()")

if(BUILD_ARDUINO_CORE)
    if(ARDUINO_BOARD STREQUAL "START")
        set(ARDUINO_BOARD_ID 1)
        set(ARDUINO_VARIANT gd32vw553_start)
        set(ARDUINO_BOARD_DEFINE GD32VW553_START)
    elseif(ARDUINO_BOARD STREQUAL "EVAL")
        set(ARDUINO_BOARD_ID 2)
        set(ARDUINO_VARIANT gd32vw553_eval)
        set(ARDUINO_BOARD_DEFINE GD32VW553_EVAL)
    else()
        message(FATAL_ERROR "ARDUINO_BOARD must be START or EVAL (got '${ARDUINO_BOARD}')")
    endif()

    if(NOT ARDUINO_SKETCH)
        message(FATAL_ERROR "Set ARDUINO_SKETCH to a C++ source file that defines setup() and loop().")
    endif()

    get_filename_component(ARDUINO_SKETCH_ABSOLUTE "${ARDUINO_SKETCH}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    if(NOT EXISTS "${ARDUINO_SKETCH_ABSOLUTE}")
        message(FATAL_ERROR "ARDUINO_SKETCH does not exist: ${ARDUINO_SKETCH_ABSOLUTE}")
    endif()

    get_filename_component(ARDUINO_SKETCH_EXTENSION "${ARDUINO_SKETCH_ABSOLUTE}" EXT)
    if(NOT ARDUINO_SKETCH_EXTENSION STREQUAL ".cpp")
        message(FATAL_ERROR "ARDUINO_SKETCH must be a preprocessed .cpp file; Arduino .ino preprocessing is not part of this CMake target.")
    endif()

    set(ARDUINO_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/system/gd32vw55x/sdk")
    set(ARDUINO_SDK_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
    set(ARDUINO_BOARD_OVERRIDE_HEADER "${ARDUINO_SDK_CONFIG_DIR}/gd32vw55x_board_override.h")
    file(MAKE_DIRECTORY "${ARDUINO_SDK_CONFIG_DIR}")
    file(WRITE "${ARDUINO_BOARD_OVERRIDE_HEADER}"
        "#include \"${ARDUINO_SDK_DIR}/config/platform_def.h\"\n"
        "#undef CONFIG_BOARD\n"
        "#define CONFIG_BOARD ${ARDUINO_BOARD_ID}\n")
endif()

if(BUILD_ARDUINO_CORE)
    add_subdirectory("${ARDUINO_SDK_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/gd32vw55x_sdk" EXCLUDE_FROM_ALL)

    function(arduino_apply_board_override directory)
        get_property(directory_targets DIRECTORY "${directory}" PROPERTY BUILDSYSTEM_TARGETS)
        foreach(target_name IN LISTS directory_targets)
            get_target_property(target_type "${target_name}" TYPE)
            get_target_property(target_imported "${target_name}" IMPORTED)
            if(NOT target_imported AND NOT target_type STREQUAL "INTERFACE_LIBRARY")
                target_compile_options("${target_name}" PRIVATE
                    -include "${ARDUINO_BOARD_OVERRIDE_HEADER}")
            endif()
        endforeach()

        get_property(child_directories DIRECTORY "${directory}" PROPERTY SUBDIRECTORIES)
        foreach(child_directory IN LISTS child_directories)
            arduino_apply_board_override("${child_directory}")
        endforeach()
    endfunction()

    arduino_apply_board_override("${CMAKE_CURRENT_BINARY_DIR}/gd32vw55x_sdk")

    if(NOT TARGET MSDK_MODULES OR NOT TARGET MSDK_CORE_API OR NOT TARGET riscv)
        message(FATAL_ERROR "The GD32VW55x SDK did not expose the expected static library targets.")
    endif()

    set(ARDUINO_CORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino")
    set(ARDUINO_VARIANT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/variants/${ARDUINO_VARIANT}")
    set(ARDUINO_SDK_MODULE_DIR "${ARDUINO_SDK_DIR}/MSDK")

    add_executable(ArduinoFirmware)
    target_sources(ArduinoFirmware
        PRIVATE
            "${ARDUINO_CORE_DIR}/HardwareSerial.cpp"
            "${ARDUINO_CORE_DIR}/main.cpp"
            "${ARDUINO_CORE_DIR}/Print.cpp"
            "${ARDUINO_CORE_DIR}/WMath.cpp"
            "${ARDUINO_CORE_DIR}/WString.cpp"
            "${ARDUINO_CORE_DIR}/wiring_analog.cpp"
            "${ARDUINO_CORE_DIR}/wiring_digital.cpp"
            "${ARDUINO_CORE_DIR}/wiring_interrupt.cpp"
            "${ARDUINO_CORE_DIR}/wiring_pulse.cpp"
            "${ARDUINO_CORE_DIR}/wiring_shift.cpp"
            "${ARDUINO_CORE_DIR}/wiring_time.cpp"
            "${ARDUINO_VARIANT_DIR}/variant.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/system/gd32vw55x/ArduinoPlatform.cpp"
            "${ARDUINO_SKETCH_ABSOLUTE}"
    )

    get_filename_component(ARDUINO_SKETCH_DIR "${ARDUINO_SKETCH_ABSOLUTE}" DIRECTORY)
    target_include_directories(ArduinoFirmware
        PRIVATE
            "${ARDUINO_CORE_DIR}"
            "${ARDUINO_VARIANT_DIR}"
            "${ARDUINO_SKETCH_DIR}"
    )

    target_compile_definitions(ArduinoFirmware
        PRIVATE
            ARDUINO=10810
            ARDUINO_ARCH_GD32VW55X
            ARDUINO_${ARDUINO_BOARD_DEFINE}
    )
    target_compile_options(ArduinoFirmware PRIVATE
        -fno-unroll-loops
        -Wunused
        -Wall
        -Wno-format
        -Wno-unused-function
        -Wno-pointer-sign
        -Os
        -u_printf_float
        -include "${ARDUINO_BOARD_OVERRIDE_HEADER}"
    )
    target_link_options(ArduinoFirmware PRIVATE
        -fno-unroll-loops
        -Wunused
        -Wuninitialized
        -Wall
        -Wno-format
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -Wno-pointer-sign
        -u_printf_float
        "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/ArduinoFirmware.map"
    )

    if(CONFIG_BLE_FEATURE STREQUAL "MIN")
        set(ARDUINO_BLE_LIBRARY ble)
    else()
        set(ARDUINO_BLE_LIBRARY ble_max)
    endif()

    target_link_directories(ArduinoFirmware
        PRIVATE
            "${ARDUINO_SDK_MODULE_DIR}/lib"
            "${ARDUINO_SDK_MODULE_DIR}/plf/riscv/NMSIS/Library/DSP/GCC"
    )
    target_link_libraries(ArduinoFirmware
        PRIVATE
            MSDK_CORE_API
            -Wl,--start-group
            wifi
            rf
            rftest
            wpas
            ${ARDUINO_BLE_LIBRARY}
            nmsis_dsp_rv32imafcbp
            m
            iperf3
            MSDK_MODULES
            -Wl,--whole-archive riscv -Wl,--no-whole-archive
            -Wl,--end-group
    )

    target_add_scatter_file(ArduinoFirmware
        "${ARDUINO_SDK_DIR}/MSDK/plf/riscv/env/gd32vw55x.ld")

    set(ARDUINO_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/firmware")
    set_target_properties(ArduinoFirmware PROPERTIES
        SUFFIX ".elf"
        RUNTIME_OUTPUT_DIRECTORY "${ARDUINO_OUTPUT_DIR}"
    )

    add_custom_command(TARGET ArduinoFirmware POST_BUILD
        COMMAND "${CMAKE_OBJCOPY}" -O ihex "$<TARGET_FILE:ArduinoFirmware>" "${ARDUINO_OUTPUT_DIR}/ArduinoFirmware.hex"
        COMMAND "${CMAKE_OBJCOPY}" -O binary "$<TARGET_FILE:ArduinoFirmware>" "${ARDUINO_OUTPUT_DIR}/ArduinoFirmware.bin"
        VERBATIM
    )
endif()
