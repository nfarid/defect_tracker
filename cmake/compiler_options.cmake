


#Options for database type
option(USE_POSTGRESQL "Use the postgresql database (if off use sqlite3 instead)" OFF)
if(USE_POSTGRESQL)
    message(STATUS "Built application will use postgresql.")
else()
    message(STATUS "Built application will use sqlite3.")
endif()


#Options for precompiled headers

option(ENABLE_PRECOMPILE_HEADERS "Use precompile to potentially speed up compilation" ON)



#Options for warnings

set(COMPILER_WARNINGS "")

option(ENABLE_ADDITIONAL_WARNINGS "Turn on addtional warnings" OFF)

if(ENABLE_ADDITIONAL_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        #Add warnings for GCC and Clang
        set(COMPILER_WARNINGS "${COMPILER_WARNINGS}"
            "-Wall"
            "-Wextra"
            "-Wpedantic"
            "-Warray-bounds"
            "-Wcast-align"
            "-Wconversion"
            "-Wdangling-else"
            "-Wimplicit-fallthrough"
            "-Wfloat-equal"
            "-Winit-self"
            "-Wmain"
            "-Wmissing-declarations"
            "-Wnon-virtual-dtor"
            "-Wparentheses"
            "-Wpointer-arith"
            "-Wredundant-decls"
            "-Wswitch"
            "-Wstrict-overflow"
            "-Wuninitialized"
            "-Wundef"
            "-Wunreachable-code"
            "-Wunused"
        )
        #Clang has additional warnings
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(COMPILER_WARNINGS "${COMPILER_WARNINGS}"
                "-Wcovered-switch-default"
                "-Wdeprecated"
                "-Wdeprecated-copy"
                "-Wdeprecated-copy-dtor"
                "-Wmissing-prototypes"
                "-Wmissing-variable-declarations"
                "-Wsuggest-destructor-override"
                "-Wnewline-eof"
                "-Wunused-exception-parameter"
            )
        endif()
    endif()
endif()



#Options for santiser

set(SANITISER "")

option(ENABLE_UNDEFINED_BEHAVIOUR_SANTISER "Turn on sanitiser for undefined behaviour" OFF)

if(ENABLE_UNDEFINED_BEHAVIOUR_SANTISER)
    set(SANITISER "${SANITISER}" "-fsanitize=undefined")
endif()

option(ENABLE_ADDRESS_SANTISER "Turn on sanitiser for memory errors, can slow down runtime (cannot be used with thread sanitiser at the same time)" OFF)

if(ENABLE_ADDRESS_SANTISER)
    set(SANITISER "${SANITISER}" "-fsanitize=address")
endif()

option(ENABLE_THREAD_SANITISER "Turn on sanitiser for data races (cannot be used with address sanitiser at the same time)" OFF)

if(ENABLE_THREAD_SANITISER)
    set(SANITISER "${SANITISER}" "-fsanitize=thread")
endif()

#Misc

set(OTHER_COMPILER_OPTIONS "")
set(OTHER_LINKER_OPTIONS "")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    option(ENABLE_FULL_DEBUG_SYMBOLS "Turn on fstandalone-debug for debug builds for additional symbols" ON)
    set(OTHER_COMPILER_OPTIONS "${OTHER_COMPILER_OPTIONS}" "-ftime-trace")
    set(OTHER_LINKER_OPTIONS "${OTHER_LINKER_OPTIONS}" "-ftime-trace")
    if(ENABLE_FULL_DEBUG_SYMBOLS AND (CMAKE_BUILD_TYPE STREQUAL "Debug") )
        set(OTHER_COMPILER_OPTIONS "${OTHER_COMPILER_OPTIONS}" "-fstandalone-debug")
    endif()
endif()







