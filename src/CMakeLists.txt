

add_executable("${APP_NAME}"
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
set(LIBRARIES "Drogon::Drogon")
target_link_libraries("${APP_NAME}" PRIVATE "${LIBRARIES}")
