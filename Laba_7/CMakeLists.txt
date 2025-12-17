cmake_minimum_required(VERSION 3.15)
project(laba_7 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.0
        TLS_VERIFY false
)

FetchContent_MakeAvailable(googletest)

option(BUILD_TESTS "Build tests" ON)

add_library(game_lib STATIC
        src/core/npc.cpp
        src/core/factory_npc.cpp
        src/core/types_npc.cpp
        src/knight_errant.cpp
        src/slaver.cpp
        src/squirrel.cpp
        src/events/implementations/console_fight_observer.cpp

        include/core/config.h
        include/events/implementations/console_fight_observer.h
)
target_include_directories(game_lib PUBLIC include)

add_executable(game_editor main.cpp
        include/editor/game_map.h
)

target_link_libraries(game_editor game_lib)

enable_testing()
add_executable(tests tests/test_main.cpp)
target_link_libraries(tests game_lib gtest_main)

add_test(NAME MyProjectTests COMMAND tests)