
cmake_minimum_required(VERSION 3.15)

add_library(riscv)



target_sources(riscv
    PRIVATE
        env/entry.S
        env/start.S
        env/env_init.c
        env/handlers.c
        arch/lib/lib_hook.c
        gd32vw55x/gd32vw55x_it.c
        gd32vw55x/system_gd32vw55x.c
)

set(RISCV_INCLUDE_REL_PATHS
        gd32vw55x
        NMSIS/Core/Include
        NMSIS/DSP/Include
        NMSIS/DSP/Include/dsp
        arch
        arch/boot
        arch/lib
        arch/ll
        arch/compiler
)

msdk_agg_api(riscv_api ${RISCV_INCLUDE_REL_PATHS})

target_link_libraries(riscv
    PUBLIC riscv_api
    PRIVATE
        bsp_api
        util_api
        rtos_api
        gd32vw55x_ble_api
)

set_library_output_destination(riscv)

#msdk_agg_module(riscv)


