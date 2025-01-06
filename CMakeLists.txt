cmake_minimum_required(VERSION 3.28)
project(sfmlTraining)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.0.2
)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)

FetchContent_MakeAvailable(fmt SFML)

add_executable(main src/main.cpp src/Game.cpp src/Player.cpp)
target_link_libraries(main PRIVATE sfml-graphics fmt)
