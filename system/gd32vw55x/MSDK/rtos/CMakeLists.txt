cmake_minimum_required(VERSION 3.15)

add_library(rtos)

target_compile_options(rtos
    PRIVATE
        -w
)

set(RTOS_WRAPPER_INCLUDE_REL_PATHS
    rtos_wrapper
)

if (PLATFORM_OS_FREERTOS)
    add_subdirectory(FreeRTOS)
    list(APPEND RTOS_WRAPPER_INCLUDE_REL_PATHS
        FreeRTOS/Source/include
        FreeRTOS/Source/portable/riscv32
        FreeRTOS/config
    )
endif()


msdk_agg_api(rtos_api ${RTOS_WRAPPER_INCLUDE_REL_PATHS})

target_sources(rtos
    PRIVATE
        rtos_wrapper/wrapper_os.c
)

target_link_libraries(rtos
    $<$<BOOL:${PLATFORM_OS_FREERTOS}>:freertos>
    rtos_api
)

msdk_agg_module(rtos)
