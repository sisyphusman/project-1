cmake_minimum_required(VERSION 3.20)

project(project-1 LANGUAGES CXX)

# C++ 버전 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 실행 파일 추가
add_executable(project-1
    # sources
	src/main.cpp
    src/Game.cpp
    src/TimeManager.cpp

    # headers
    src/Game.h
    src/TimeManager.h
)

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