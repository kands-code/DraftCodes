add_library(${PROJECT_NAME} STATIC
    "option.c"
    "utils.c"
    "game.c"
    "character.c"
    "monster.c"
    "place.c"
    "weapon.c"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)