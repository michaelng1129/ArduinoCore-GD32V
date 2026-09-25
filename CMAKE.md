# Building an Arduino sketch with the GD32VW55x SDK

The top-level CMake project builds SDK modules as static libraries and links them with a separate Arduino firmware target. The Arduino core owns `main()` and starts its loop as a FreeRTOS task. The vendor SDK app executable and its `MSDK/app/main.c` entry point are not part of the Arduino firmware target.

## Requirements

- The GigaDevice RISC-V Nuclei toolchain must be available on `PATH` as `riscv-nuclei-elf-*`.
- Python and the image packaging tools used by the SDK post-build scripts must be installed and available as expected by the SDK.
- The sketch source must be a C++ `.cpp` file and define `setup()` and `loop()`. Convert an Arduino `.ino` file to C++ before passing it to CMake.
- The current SDK flash partition table in `system/gd32vw55x/sdk/config/config_gdm32.h` is for 4 MB flash. The full SDK CMake target therefore builds the 4 MB layout.

## Configure and build

The top-level `CMakeLists.txt` selects the SDK toolchain by default and adds the vendor SDK as an excluded subdirectory, so the SDK module libraries build only when the Arduino firmware links them. From the repository root, configure the START board:

```powershell
cmake -S . -B .\build\cmake-gd32vw553-start `
  -DBUILD_ARDUINO_CORE=ON `
  -DARDUINO_BOARD=START `
  -DARDUINO_SKETCH=C:/path/to/Sketch.cpp

cmake --build build/cmake-gd32vw553-start --target ArduinoFirmware
```

For the H-EVAL board, use a separate build directory and set `-DARDUINO_BOARD=EVAL`. Keep separate build directories for different boards or SDK configurations because their static libraries are compiled with board, feature, and ABI settings. The vendor CMake files under `system/gd32vw55x/sdk` remain unchanged; the top-level CMake file owns Arduino board selection and source integration.

The root target links the SDK's FreeRTOS, startup assembly, platform, peripheral, and selected wireless modules as static libraries. Those archives are emitted under `build/bin/lib`. `cores/arduino/main.cpp` calls the platform lifecycle interface; `system/gd32vw55x/ArduinoPlatform.cpp` maps it to the GD32VW55x SDK. Each variant's `variant_config.h` declares whether Wi-Fi and BLE initialization are enabled.

The SDK's `platform_def.h` remains unchanged. The root CMake generates a board override header in the build directory so the selected board ID reaches SDK compilation. ELF, Intel HEX, and raw binary outputs are emitted under the build directory's `firmware` folder. The SDK partition layout is currently fixed at 4 MB flash.
