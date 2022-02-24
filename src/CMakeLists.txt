

#Preprocess the view files
file(GLOB CSP_LIST "${CMAKE_CURRENT_SOURCE_DIR}/views/*.csp")
set(VIEW_SRC "")
foreach(CSP_FILE ${CSP_LIST})
    message(STATUS "CSP_FILE:" "${CSP_FILE}")
    exec_program(basename ARGS "-s .csp ${CSP_FILE}" OUTPUT_VARIABLE classname)
    message(STATUS "view classname:" "${classname}")
    add_custom_command(OUTPUT "${classname}.h" "${classname}.cc"
        COMMAND drogon_ctl
        ARGS create view "${CSP_FILE}"
        DEPENDS "${CSP_FILE}"
        VERBATIM
    )
    set(VIEW_SRC "${VIEW_SRC}" "${classname}.cc")
    set_source_files_properties("${classname}.cc" COMPILE_FLAGS "-w")
endforeach()


set(APP_SRC
    "app.cpp"

    "controllers/home.cpp"
    "controllers/user.cpp"
    "controllers/user_session.cpp"

    "models/account.cpp"
    "models/comment.hpp"
    "models/project.cpp"
    "models/staff.cpp"
    "models/ticket.cpp"

    "util/all.cpp"

    "${VIEW_SRC}"
)
add_executable("${APP_NAME}"
    "${APP_SRC}"
    "main.cpp"
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
find_package("unofficial-sodium" CONFIG REQUIRED)
set(LIBRARIES
    "Drogon::Drogon"
    "unofficial-sodium::sodium"
    "unofficial-sodium::sodium_config_public"
)
target_link_libraries("${APP_NAME}" PRIVATE "${LIBRARIES}")



#Test
add_executable("${TEST_NAME}"
    "${APP_SRC}"

    "controllers/home.test.cpp"
    "controllers/user_session.test.cpp"

    "main.test.cpp"
)

target_link_libraries("${TEST_NAME}" PRIVATE "${LIBRARIES}")
ParseAndAddDrogonTests("${TEST_NAME}")
