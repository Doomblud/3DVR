cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(ShrineScene)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

# Reuse the course third-party dependencies.
# set(INFO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty}")
set(THIRD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty")

add_library(glad STATIC ${THIRD_PARTY_ROOT}/glad/src/glad.c)
target_include_directories(glad PUBLIC ${THIRD_PARTY_ROOT}/glad/include)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)

find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    # add_subdirectory(${INFO_ROOT}/3rdParty/glfw ${CMAKE_BINARY_DIR}/glfw)
    add_subdirectory(${THIRD_PARTY_ROOT}/glfw ${CMAKE_BINARY_DIR}/glfw)
endif()

set(GAME_SOURCES
    main.cpp
    src/application.cpp
    src/scene.cpp
)

add_executable(${PROJECT_NAME} ${GAME_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # ${INFO_ROOT}/3rdParty/glm
    # ${INFO_ROOT}/3rdParty/stb
    ${THIRD_PARTY_ROOT}/glm
    ${THIRD_PARTY_ROOT}/stb
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw glad)

# Use absolute shader path to avoid runtime cwd issues.
target_compile_definitions(${PROJECT_NAME} PRIVATE
    GAME_SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    GAME_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
