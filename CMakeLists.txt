cmake_minimum_required(VERSION 3.26.0)

set(CMAKE_TOOLCHAIN_FILE "F:/Engine/Engine/external/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(Engine VERSION 1.0.0 LANGUAGES CXX C)

cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)

#-----

add_subdirectory(Engine)
add_subdirectory(Game)
