
cmake_minimum_required(VERSION 3.15)

add_library(freertos)

target_sources(freertos
    PRIVATE
        Source/croutine.c
        Source/event_groups.c
        Source/list.c
        Source/queue.c
        Source/stream_buffer.c
        Source/tasks.c
        Source/timers.c
)

target_sources(freertos
    PRIVATE
        Source/portable/riscv32/port.c
        Source/portable/riscv32/portasm.S
        Source/portable/riscv32/memcpy.S
        Source/portable/Common/tickless_sleep.c
        Source/portable/MemMang/heap_4.c
)


set(FREERTOS_INCLUDE_REL_PATHS
    Source/include
    Source/portable/Common
    Source/portable/riscv32
    config
)

msdk_agg_api(freertos_api ${FREERTOS_INCLUDE_REL_PATHS})

target_link_libraries(freertos
    freertos_api
    wifi_mgmt_api
    rtos_api
    riscv_api
    util_api
    bsp_api
)

msdk_agg_module(freertos)

