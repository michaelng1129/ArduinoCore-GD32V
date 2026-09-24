
cmake_minimum_required(VERSION 3.15)

add_library(wifi_mgmt)

target_sources(wifi_mgmt
    PRIVATE
        wifi_eloop.c
        wifi_init.c
        wifi_management.c
        wifi_net_ip.c
        wifi_netlink.c
        wifi_vif.c
        wifi_wpa.c
)

set(WIFI_MGMT_INCLUDE_REL_PATHS
    .
    wpas
)

msdk_agg_api(wifi_mgmt_api ${WIFI_MGMT_INCLUDE_REL_PATHS})

target_link_libraries(wifi_mgmt
    PUBLIC
        wifi_mgmt_api
    PRIVATE
        lwIP_api
        rtos_api
        bsp_api
        mbedtls_api
        gd32vw55x_peripheral
        wifi_api
        lwIP
)

msdk_agg_module(wifi_mgmt)
