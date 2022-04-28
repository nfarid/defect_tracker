

#Preprocess the view files
file(GLOB CSP_LIST "${CMAKE_CURRENT_SOURCE_DIR}/views/*.csp")
set(VIEW_SRC "")
foreach(CSP_FILE ${CSP_LIST})
    exec_program(basename ARGS "-s .csp ${CSP_FILE}" OUTPUT_VARIABLE classname)
    add_custom_command(OUTPUT "${classname}.h" "${classname}.cc"
        COMMAND drogon_ctl
        ARGS create view "${CSP_FILE}"
        DEPENDS "${CSP_FILE}"
        VERBATIM
        COMMENT "abc"
    )
    set(VIEW_SRC "${VIEW_SRC}" "${classname}.cc")
    set_source_files_properties("${classname}.cc" COMPILE_FLAGS "-w")
endforeach()


if(USE_POSTGRESQL)
    set(GENERATED_MODELS
        "models/generated_postgresql/account.cpp"
        "models/generated_postgresql/comment.cpp"
        "models/generated_postgresql/notification.cpp"
        "models/generated_postgresql/project.cpp"
        "models/generated_postgresql/staff.cpp"
        "models/generated_postgresql/ticket.cpp"
    )
else()
    set(GENERATED_MODELS
        "models/generated_sqlite3/account.cpp"
        "models/generated_sqlite3/comment.cpp"
        "models/generated_sqlite3/notification.cpp"
        "models/generated_sqlite3/project.cpp"
        "models/generated_sqlite3/staff.cpp"
        "models/generated_sqlite3/ticket.cpp"
    )
endif()

set(APP_SRC
    "controllers/auxiliary.cpp"
    "controllers/filter.cpp"

    "controllers/comment_controller.cpp"
    "controllers/home_controller.cpp"
    "controllers/project_controller.cpp"
    "controllers/ticket_controller.cpp"
    "controllers/user_controller.cpp"

    "models/account.cpp"
    "models/comment.cpp"
    "models/notification.cpp"
    "models/project.cpp"
    "models/staff.cpp"
    "models/ticket.cpp"

    "${GENERATED_MODELS}"

    "${VIEW_SRC}"

    "util/database.cpp"
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

if(USE_POSTGRESQL)
    target_compile_definitions("${APP_NAME}" PRIVATE "USE_POSTGRESQL")
endif()



#Test
add_executable("${TEST_NAME}"
    "${APP_SRC}"

    "tests/home.test.cpp"
    "tests/project.test.cpp"
    "tests/ticket.test.cpp"
    "tests/user.test.cpp"
    "tests/user_session.test.cpp"

    "main.test.cpp"
)
target_compile_definitions("${TEST_NAME}" PRIVATE "TEST_")
target_link_libraries("${TEST_NAME}" PRIVATE "${LIBRARIES}")
ParseAndAddDrogonTests("${TEST_NAME}")
target_include_directories("${TEST_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
if(USE_POSTGRESQL)
    target_compile_definitions("${TEST_NAME}" PRIVATE "USE_POSTGRESQL")
endif()
