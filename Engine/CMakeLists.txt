cmake_minimum_required(VERSION 3.26.0)

set(CMAKE_TOOLCHAIN_FILE "F:/Engine/Engine/external/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiTreeded$<$<CONFIG:Release>:Release>DLL")

project(Engine VERSION 1.0.0 LANGUAGES CXX C)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/stb_image)
add_subdirectory(external/glm)
add_subdirectory(external/spdlog)
add_subdirectory(external/nlohmann_json)
#add_subdirectory(external/freetype)
add_subdirectory(external/msdf-atlas-gen)

cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Engine/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Engine/Debug/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Engine/Release/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/Engine/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/Engine/Bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Engine/Bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Engine/lib)

set({VCPKG_ROOT} ${CMAKE_CURRENT_SOURCE_DIR}/Engine/external/vcpkg)

# Поиск пакетов
find_package(Vulkan REQUIRED)
find_package(unofficial-shaderc CONFIG QUIET)
find_package(Freetype REQUIRED)

# Исполняемый файл
file(GLOB_RECURSE MY_SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
set(SHADER_BINARY_DIR "${CMAKE_BINARY_DIR}/../Resources")

add_library(Engine STATIC ${MY_SOURCE})


target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Engine PRIVATE
    glm
    #freetype
    Freetype::Freetype
    Vulkan::Vulkan
    glad
    spdlog
    nlohmann_json
    stb_image
    msdf-atlas-gen
)

if(unofficial-shaderc_FOUND)
    target_link_libraries(Engine PRIVATE unofficial::shaderc::shaderc)
    target_compile_definitions(Engine PRIVATE ENGINE_HAS_SHADERC)
endif()


target_link_libraries(Engine PUBLIC
    glfw
    glm
    spdlog
    nlohmann_json::nlohmann_json
    glad
)

target_compile_definitions(Engine PRIVATE
    ENGINE_WINDOW_GLFW
)
target_compile_definitions(Engine PRIVATE
    ENGINE_RENDER_OPENGL
)
target_compile_definitions(Engine PRIVATE
    ENGINE_RENDER_VULKAN
)


#add_custom_target(CopyShaders ALL
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${SHADER_SOURCE_DIR}"
#        "${SHADER_BINARY_DIR}"
#    COMMENT "Copying shaders to build directory..."
#    BYPRODUCTS "${SHADER_BINARY_DIR}"
#)

#add_dependencies(Engine CopyShaders)

#if(WIN32)
#    add_custom_command(TARGET Engine POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#            "${SHADER_SOURCE_DIR}"
#            "$<TARGET_FILE_DIR:Engine>/../Resources"
#        COMMENT "Copying shaders to executable directory..."
#    )
#endif()