cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(tinyify VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.4.4
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    curlcpp
    GIT_REPOSITORY https://github.com/JosephP91/curlcpp
    GIT_TAG master
)
FetchContent_MakeAvailable(curlcpp)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.7
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_link_libraries(imgui PRIVATE SDL3::SDL3)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3::SDL3
    curlcpp
    nlohmann_json::nlohmann_json
    imgui
)
target_include_directories(${PROJECT_NAME} PRIVATE include/ thirdparty/)