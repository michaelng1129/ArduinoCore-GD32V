
cmake_minimum_required(VERSION 3.15)

add_library(ble_app_profile)

target_compile_options(ble_app_profile
    PRIVATE
        -w
)

file(GLOB_RECURSE SRC_BLE_APP  ${CMAKE_CURRENT_SOURCE_DIR}/app/*.c)
file(GLOB_RECURSE SRC_BLE_PROFILE  ${CMAKE_CURRENT_SOURCE_DIR}/profile/*.c)

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    SET(abs_dir_list "")

    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path "${file_path}" PATH)
        SET(abs_dir_list ${abs_dir_list} "${dir_path}")
    ENDFOREACH()

    LIST(REMOVE_DUPLICATES abs_dir_list)
    SET(relative_dir_list "")
    FOREACH(abs_dir ${abs_dir_list})
        file(RELATIVE_PATH rel_dir "${CMAKE_CURRENT_SOURCE_DIR}" "${abs_dir}")
        SET(relative_dir_list ${relative_dir_list} "${rel_dir}")
    ENDFOREACH()
    SET(${return_list} ${relative_dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_list)

list(REMOVE_ITEM SRC_BLE_APP "${CMAKE_CURRENT_SOURCE_DIR}/app/atcmd_ble.c")
list(REMOVE_ITEM SRC_BLE_PROFILE "${CMAKE_CURRENT_SOURCE_DIR}/profile/hogp/ble_hogpd.c")

target_sources(ble_app_profile
    PRIVATE
        ${SRC_BLE_APP}
        ${SRC_BLE_PROFILE}
)

SET(BLE_INCLUDE_REL_PATHS
    ${header_list}
    app
    profile
)

target_include_directories(ble_app_profile
    PRIVATE
        ${header_list}
)


msdk_agg_api(ble_app_profile_api ${BLE_INCLUDE_REL_PATHS})

target_link_libraries(ble_app_profile
    PUBLIC ble_app_profile_api
    PRIVATE
        bsp_api
        riscv_api
        util_api
        rtos_api
        wifi_mgmt_api
        lwIP_api
        wifi_api
        mbedtls_api
        gd32vw55x_ble_api
)

msdk_agg_module(ble_app_profile)
