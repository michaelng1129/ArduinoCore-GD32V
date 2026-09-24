cmake_minimum_required(VERSION 3.15)

if (CONFIG_BLE_FEATURE STREQUAL "MIN")
    set(ble_config_dir config)
else ()
    set(ble_config_dir config_max)
endif ()

set(GD32VW55X_BLE_INCLUDE_REL_PATHS
    src/export
    src/export/${ble_config_dir}
)
msdk_agg_api(gd32vw55x_ble_api ${GD32VW55X_BLE_INCLUDE_REL_PATHS})
