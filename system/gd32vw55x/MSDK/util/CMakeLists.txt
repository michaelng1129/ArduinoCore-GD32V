
cmake_minimum_required(VERSION 3.15)

add_library(util)

target_sources(util
    PRIVATE
        src/aes_ecb.c
        src/cJSON.c
        src/crc.c
        src/cyclic_buffer.c
        src/debug_print.c
        src/dlist.c
        src/slist.c
        src/trace_ext.c
        src/user_setting.c
        src/util.c
)

set(UTIL_INCLUDE_REL_PATHS
    include
)

msdk_agg_api(util_api ${UTIL_INCLUDE_REL_PATHS})

target_link_libraries(util
    PUBLIC util_api
    PRIVATE
        rtos_api
        riscv_api
        mbedtls_api
        bsp_api
)

msdk_agg_module(util)