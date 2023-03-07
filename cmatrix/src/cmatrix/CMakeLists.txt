add_library("${PROJECT_NAME}" STATIC
    "initM.c"
    "attrM.c"
    "manM.c"
    "typeM.c"
    "appM.c"
    "utils.c"
)

# use math library
target_link_libraries("${PROJECT_NAME}" m)

target_include_directories("${PROJECT_NAME}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)