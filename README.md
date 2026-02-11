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

이 프로젝트는 vcpkg를 통해 SFML을 설치합니다  
This project installs SFML via vcpkg

---
1. vcpkg install sfml
2. git clone <repository_url>
3. vcpkg 폴더 위치를 환경변수로 설정
- Powershell에서 경로를 수정 후 입력: [Environment]::SetEnvironmentVariable("VCPKG_ROOT", "C:\dev\vcpkg", "User")
- 또는 CMakePresets.json 파일에서 CMAKE_TOOLCHAIN_FILE에 vcpkg.cmake의 전체 경로를 입력합니다. ("../vcpkg/scripts/buildsystems/vcpkg.cmake")
4. 터미널에서 프로젝트 최상위 경로로 이동 후 cmake --preset vs 입력
5. build/project-1.slnx 실행
---
