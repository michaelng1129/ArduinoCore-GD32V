
cmake_minimum_required(VERSION 3.15)

add_library(bsp)

target_sources(bsp
    PRIVATE
        bsp.c
        gd32vw55x_platform.c
        init_rom.c
        plf_assert.c
        wakelock.c
        dma/dma.c
        nvds/nvds_flash.c
        raw_flash/raw_flash_api.c
        time/systime.c
        trng/trng.c
        uart/log_uart.c
        uart/uart.c
        uart/trace_uart.c
        uart/ble_uart.c
        dsp.c
        wdt/wdt.c
)

set(BSP_INCLUDE_REL_PATHS
    .
    dma
    nvds
    raw_flash
    reg
    rf
    time
    trng
    uart
    wdt
)
msdk_agg_api(bsp_api ${BSP_INCLUDE_REL_PATHS})

target_link_libraries(bsp
    PUBLIC bsp_api
    PRIVATE
        riscv_api
        util_api
        rtos_api
        wifi_mgmt_api
        lwIP_api
        wifi_api
        gd32vw55x_ble_api
        mbedtls_api
)

msdk_agg_module(bsp)
