cmake_minimum_required(VERSION 3.20)

project(project-1 LANGUAGES CXX)

# C++ 버전 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# src 폴더 내의 모든 .cpp와 .h 파일 자동 수집
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# 실행 파일 추가
add_executable(project-1 ${SOURCE_FILES})

# src 폴더를 include 경로에 추가
target_include_directories(project-1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# 실제 폴더 구조를 Visual Studio 필터에 반영
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "" FILES ${SOURCE_FILES})

# vcpkg
find_package(SFML 3 CONFIG REQUIRED COMPONENTS
    System
    Window
    Graphics
    Audio
    Network
)

# SFML3
target_link_libraries(project-1 PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
    SFML::Network
)