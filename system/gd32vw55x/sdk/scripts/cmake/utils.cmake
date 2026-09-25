
# msdk_agg_api
# Dynamically creates and exports INTERFACE include directories for a target.
#
# Usage: msdk_agg_api(<target_name> <DIR1> <DIR2> ...)
function(msdk_agg_api TARGET_NAME)
    set(INTERFACE_INCLUDES "")
    add_library(${TARGET_NAME} INTERFACE)

    setup_header_copy_targets(${TARGET_NAME} ${ARGN})

    # ARGN contains all arguments after TARGET_NAME (the list of paths)
    foreach(REL_PATH IN ITEMS ${ARGN})
        list(APPEND INTERFACE_INCLUDES
             $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${REL_PATH}>
        )
    endforeach()

    # Apply the generated list to the target
    target_include_directories(${TARGET_NAME}
        INTERFACE
            ${INTERFACE_INCLUDES}
    )

    target_link_libraries(MSDK_CORE_API INTERFACE ${TARGET_NAME})
    set_property(TARGET ${TARGET_NAME} APPEND PROPERTY LINK_INTERFACE_MULTIPLICITY 3)

endfunction()

# set_library_output_destination
# Configures the library's output directory to a standard location
# (typically build/bin/lib relative to the project root's build folder).
# This avoids fragile POST_BUILD copy commands and is the standard practice.
#
# Usage: set_library_output_destination(<target_name>)
# Arguments:
#   TARGET_NAME - The name of the library target (e.g., mbedtls).
function(set_library_output_destination TARGET_NAME)
    # Define the final destination path relative to the top-level build directory
    set(FINAL_DESTINATION "${CMAKE_BINARY_DIR}/bin/lib")

    # Ensure the destination directory exists
    file(MAKE_DIRECTORY ${FINAL_DESTINATION})

    # Set the ARCHIVE_OUTPUT_DIRECTORY property to place the static library
    # directly into the desired output folder during the linking step.
    set_target_properties(${TARGET_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${FINAL_DESTINATION}
        # Apply to all configurations (Release, Debug, etc.)
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${FINAL_DESTINATION}
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${FINAL_DESTINATION}
    )

endfunction()

# msdk_agg_module
#
# Usage: msdk_agg_module(<target_name>)
function(msdk_agg_module TARGET_NAME)
    target_link_libraries(MSDK_MODULES INTERFACE ${TARGET_NAME})
    set_library_output_destination(${TARGET_NAME})
endfunction()

# Compatibility wrappers for modules that still use the previous helper names.
function(add_lib_api TARGET_NAME)
    if(NOT TARGET ${TARGET_NAME})
        message(FATAL_ERROR "add_lib_api expected an existing target: ${TARGET_NAME}")
    endif()

    target_link_libraries(MSDK_CORE_API INTERFACE ${TARGET_NAME})
    set_property(TARGET ${TARGET_NAME} APPEND PROPERTY LINK_INTERFACE_MULTIPLICITY 3)
endfunction()

function(add_lib TARGET_NAME)
    if(NOT TARGET ${TARGET_NAME})
        message(FATAL_ERROR "add_lib expected an existing target: ${TARGET_NAME}")
    endif()

    target_link_libraries(MSDK_MODULES INTERFACE ${TARGET_NAME})
    set_library_output_destination(${TARGET_NAME})
endfunction()

# setup_header_copy_targets
# Usage: setup_header_copy_targets(<TARGET_NAME> <DIR1> <DIR2> ...)
function(setup_header_copy_targets TARGET_NAME)
    # Check if header copy is enabled (default: ON)
    if(NOT CONFIG_HEADER_COPY)
        return()
    endif()

    # Define paths
    set(PROJECT_SOURCE_ROOT ${CMAKE_SOURCE_DIR})
    set(BUILD_INCLUDE_DESTINATION "${CMAKE_BINARY_DIR}/bin/include")
    set(INCLUDE_INFO "${CMAKE_BINARY_DIR}/bin/flags/includes")

    # List to store all commands (used to generate the batch script)
    set(COPY_COMMANDS)
    # List to store all source header files (used for target dependency tracking)
    set(ALL_SOURCE_HEADERS)

    # 1. Collect all headers and generate commands
    foreach(REL_HDR_DIR IN ITEMS ${ARGN})
        # Resolve absolute path for globbing
        get_filename_component(SOURCE_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${REL_HDR_DIR}" ABSOLUTE)

        file(GLOB_RECURSE HEADERS_TO_COPY
            "${SOURCE_ROOT_PATH}/*.h"
            "${SOURCE_ROOT_PATH}/*.hpp"
        )
        file(RELATIVE_PATH INCLUDE_PATH ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/${REL_HDR_DIR}")

        file(APPEND ${INCLUDE_INFO} " -iwithprefixbefore ${INCLUDE_PATH} ")

        foreach(HEADER_FILE ${HEADERS_TO_COPY})
            # Calculate destination path relative to the project root
            file(RELATIVE_PATH RELATIVE_PATH ${PROJECT_SOURCE_ROOT} ${HEADER_FILE})
            set(TARGET_FILE_FULL "${BUILD_INCLUDE_DESTINATION}/${RELATIVE_PATH}")
            get_filename_component(TARGET_DIR ${TARGET_FILE_FULL} DIRECTORY)

            # Add source file to the dependency list of the final custom target
            list(APPEND ALL_SOURCE_HEADERS ${HEADER_FILE})

            # Append make_directory and copy commands to the script list

            list(APPEND COPY_COMMANDS "
                file(MAKE_DIRECTORY \"${TARGET_DIR}\")
                file(COPY \"${HEADER_FILE}\" DESTINATION \"${TARGET_DIR}\")
            ")
        endforeach()
    endforeach()

    # 2. Write the batch script file
    set(SCRIPT_FILE_NAME "${TARGET_NAME}_copy_headers.cmake")
    set(SCRIPT_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${SCRIPT_FILE_NAME}")

    # Combine all commands into a single string
    string(JOIN "\n" SCRIPT_CONTENT ${COPY_COMMANDS})

    if(SCRIPT_CONTENT)
        # Write the final script content
        file(WRITE "${SCRIPT_FILE_PATH}" "${SCRIPT_CONTENT}")

        message(STATUS "[HEADER_COPY] Wrote batch script to: ${SCRIPT_FILE_PATH}")

        # 3. Create a single custom target to execute the script
        add_custom_target(${TARGET_NAME}_file_copy ALL
            # Execute the CMake script using -P flag (Process mode)
            COMMAND ${CMAKE_COMMAND} -P "${SCRIPT_FILE_PATH}"
            # The target MUST depend on all source headers. If any source header changes,
            # this single command will be re-run.
            DEPENDS ${ALL_SOURCE_HEADERS}
        )
    else()
        message(STATUS "WARNING: No header files found for target ${TARGET_NAME}_file_copy in directories: ${ARGN}")
    endif()
endfunction()
