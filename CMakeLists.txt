cmake_minimum_required(VERSION 3.20)

project(Project1 LANGUAGES CXX)

# C++ 버전 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# src 폴더 내의 모든 .cpp와 .h 파일 자동 수집
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.h"
)

# 실행 파일 추가
add_executable(Project1 ${SOURCE_FILES})

# src 폴더를 include 경로에 추가
target_include_directories(Project1 PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Src"
)

# 실제 폴더 구조를 Visual Studio 탐색기에 반영
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Src" PREFIX "" FILES ${SOURCE_FILES})

# 빌드 결과물 경로 지정
set_target_properties(Project1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Out"
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Project1>"
)

# Assets 폴더를 실행 파일 위치로 복사
add_custom_command(TARGET Project1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets"
        "$<TARGET_FILE_DIR:Project1>/Assets"
)

find_package(SFML 3 CONFIG REQUIRED COMPONENTS
    System Window Graphics Audio Network
)

find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(Project1 PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
    SFML::Network

    nlohmann_json::nlohmann_json
)