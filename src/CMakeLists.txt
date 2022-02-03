

#Preprocess the view files
file(GLOB SCP_LIST "${CMAKE_CURRENT_SOURCE_DIR}/views/*.csp")
foreach(cspFile "${SCP_LIST}")
    message(STATUS "cspFile:" "${cspFile}")
    exec_program(basename ARGS "-s .csp ${cspFile}" OUTPUT_VARIABLE classname)
    message(STATUS "view classname:" "${classname}")
    add_custom_command(OUTPUT "${classname}.h" "${classname}.cc"
        COMMAND drogon_ctl
        ARGS create view "${cspFile}"
        DEPENDS "${cspFile}"
        VERBATIM
    )
    set(VIEWSRC "${VIEWSRC}" "${classname}.cc")
    set_source_files_properties("${classname}.cc" COMPILE_FLAGS "-w")
endforeach()


add_executable("${APP_NAME}"
    "main.cpp"

    "controllers/home.cpp"

    "${VIEWSRC}"
)

set_property(
    SOURCE "main.cpp"
    PROPERTY COMPILE_DEFINITIONS
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_VERSION="${PROJECT_VERSION}"
)

target_compile_options("${APP_NAME}"
    PUBLIC "${COMPILER_WARNINGS}"
    PUBLIC "${SANITISER}"
    PUBLIC "${OTHER_COMPILER_OPTIONS}"
)

target_link_options("${APP_NAME}"
    PUBLIC "${SANITISER}"
    PUBLIC "${OTHER_LINKER_OPTIONS}"
)


find_package("Drogon" CONFIG REQUIRED)
set(LIBRARIES "Drogon::Drogon")
target_link_libraries("${APP_NAME}" PRIVATE "${LIBRARIES}")