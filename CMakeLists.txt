
#Pre-setup

cmake_minimum_required(VERSION 3.21)


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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



#Setting up for different platforms

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()
if(WIN32)
    set(WINDOWS TRUE)
endif()

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(${IS_MULTI_CONFIG})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/MinSizeRel")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/RelWithDebInfo")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    file(MAKE_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
endif()


#Actual builds

include("cmake/compiler_options.cmake")

set(APP_NAME "${PROJECT_NAME}")
add_subdirectory("src/")



#Data directory

set(RES_SRC "${CMAKE_SOURCE_DIR}/data")
if(EXISTS "${RES_SRC}")
    set(RES_DEST "${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}")
    file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/share/")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/share/")
    add_custom_target("resource_directory" ALL
        COMMAND "${CMAKE_COMMAND}" "-E" "create_symlink" "${RES_SRC}" "${RES_DEST}"
    )
endif()

