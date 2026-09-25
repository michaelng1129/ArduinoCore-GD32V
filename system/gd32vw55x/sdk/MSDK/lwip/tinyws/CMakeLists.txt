cmake_minimum_required(VERSION 3.15)

add_library(tinyws_api INTERFACE)
add_library(tinyws)



target_sources(tinyws
    PRIVATE
        tinyws.c
        ws_ssl.c
)

target_include_directories(tinyws_api
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


target_link_libraries(tinyws
    lwIP_api
    tinyws_api
    rtos_api
    riscv_api
    util_api
    bsp_api
    mbedtls_api
)

add_lib_api(tinyws_api)
add_lib(tinyws)
