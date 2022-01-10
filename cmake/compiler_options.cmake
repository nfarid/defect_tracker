


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
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(SANITISER "${SANITISER}" "-fsanitize=undefined")
    endif()
endif()

option(ENABLE_ADDRESS_SANTISER "Turn on sanitiser for memory errors, can slow down runtime" OFF)

if(ENABLE_ADDRESS_SANTISER)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(SANTISER "${SANITISER}" "-fsanitize=address")
    endif()
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







