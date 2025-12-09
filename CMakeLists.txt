cmake_minimum_required(VERSION 3.20)

project(project-1 LANGUAGES CXX)

# C++ 버전 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 실행 파일 추가
add_executable(project-1
	src/main.cpp
)