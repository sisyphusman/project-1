# Project-1
C++과 SFML을 사용해 제작 중인 동양풍 2D 로그라이크 콘솔 게임입니다.

## Requirements
- Visual Studio 2026
- C++20
- CMake
- vcpkg
- SFML
  
## Build

이 프로젝트는 vcpkg를 통해 SFML을 설치합니다.
```
vcpkg install sfml
git clone <repository_url>
CMakePresets.json 파일에서 CMAKE_TOOLCHAIN_FILE에 vcpkg.cmake의 전체 경로를 입력합니다. ("../vcpkg/scripts/buildsystems/vcpkg.cmake")
터미널에서 프로젝트 최상위 경로로 이동 후 cmake --preset vs 입력
build/project-1.slnx 실행
```
