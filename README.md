# Project-1
C++과 SFML을 사용해 제작 중인 고대 한국 배경의 2D 로그라이크 콘솔 게임입니다  
This is a 2D roguelike console game set in an ancient korean world, currently under development using C++ and SFML

## Requirements
- Visual Studio 2026
- C++20
- CMake
- vcpkg
- SFML
- nlohmann-json
  
## Build

이 프로젝트는 vcpkg와 CMake를 사용합니다  
This project uses vcpkg and CMake  

---
1. git clone <repository_url> 후 프로젝트 최상위 경로로 이동
2. vcpkg 폴더 위치를 환경변수로 설정
    - Powershell에서 경로를 수정 후 입력: [Environment]::SetEnvironmentVariable("VCPKG_ROOT", "C:\dev\vcpkg", "User")
    - 또는 CMakePresets.json 파일에서 CMAKE_TOOLCHAIN_FILE에 vcpkg.cmake의 전체 경로를 입력합니다. ("../vcpkg/scripts/buildsystems/vcpkg.cmake")
3. vcpkg install --triplet x64-windows
4. cmake --preset vs
5. Build/Project1.slnx로 빌드
    - 바로 빌드하려면 cmake --build Build --config Debug 이후 Out/Debug/Project1.exe를 실행
---
