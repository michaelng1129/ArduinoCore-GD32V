cmake_minimum_required(VERSION 3.15)

add_library(sntp_api INTERFACE)
add_library(sntp)



target_sources(sntp
    PRIVATE
        sntp_api.c
)

target_include_directories(sntp_api
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


target_link_libraries(sntp
    lwIP_api
    sntp_api
    rtos_api
    riscv_api
    util_api
)

add_lib_api(sntp_api)
add_lib(sntp)
