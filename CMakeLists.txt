
#Pre-setup

cmake_minimum_required(VERSION 3.16)


if(DEFINED "ENV{VCPKG_ROOT}" AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Use vcpkg if toolchain is not specified")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE STRING "Create a compile_commands.json file")



#Basic setup

project("defect_tracker"
    LANGUAGES "CXX"
    VERSION "0.1.0"
    DESCRIPTION "A defect tracker"
)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR
        "In-source builds are not supported, please remove the `CMakeFiles'"
        "folder and `CMakeCache.txt', and create a folder for the build:"
        "mkdir build; cd build; cmake .."
    )
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fcoroutines")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(WARNING
        "Clang (clang++) does not have full coroutine support yet, which this project needs."
        "Please use GCC (g++) instead."
    )
endif()

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
else()
    message(WARNING "This project is intended to be used in a linux environment.")
endif()

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(${IS_MULTI_CONFIG})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/MinSizeRel")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/RelWithDebInfo")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()


#Actual builds

include("cmake/compiler_options.cmake")

set(APP_NAME "${PROJECT_NAME}")
set(TEST_NAME "test-${PROJECT_NAME}")
add_subdirectory("src/")



#Other directories

add_custom_target("data_directory" ALL
    COMMAND "${CMAKE_COMMAND}" "-E" "create_symlink" "${CMAKE_SOURCE_DIR}/data" "${CMAKE_BINARY_DIR}/app/data"
)

add_custom_target("sql_directory" ALL
    COMMAND "${CMAKE_COMMAND}" "-E" "create_symlink" "${CMAKE_SOURCE_DIR}/sql" "${CMAKE_BINARY_DIR}/sql"
)

