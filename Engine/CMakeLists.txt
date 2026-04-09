cmake_minimum_required(VERSION 3.26.0)

set(CMAKE_TOOLCHAIN_FILE "F:/Engine/Engine/external/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiTreeded$<$<CONFIG:Release>:Release>DLL")

project(Engine VERSION 1.0.0 LANGUAGES CXX C)

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

# Поиск пакетов
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Исполняемый файл
file(GLOB_RECURSE MY_SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(GLAD_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c")
set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
set(SHADER_BINARY_DIR "${CMAKE_BINARY_DIR}/../Resources")

#add_executable(Engine 
#    ${MY_SOURCE}
#    ${GLAD_SOURCE}
#)

add_library(Engine STATIC
            ${MY_SOURCE}
            ${GLAD_SOURCE})

if(MSVC)
    set_source_files_properties(external/glad/src/glad.c PROPERTIES 
        COMPILE_FLAGS "/TC"
    )
endif()

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image
)

target_link_libraries(Engine PRIVATE
    OpenGL::GL
    glfw
    glm::glm
)

add_library(glad STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)
if(MSVC)
    set_source_files_properties(${GLAD_SOURCE} PROPERTIES COMPILE_FLAGS "/TC")
endif()


target_link_libraries(Engine PUBLIC
    OpenGL::GL
    glfw
    glm::glm
    spdlog::spdlog_header_only
    nlohmann_json::nlohmann_json
    glad
)

target_compile_definitions(Engine PRIVATE
    ENGINE_WINDOW_GLFW
)
target_compile_definitions(Engine PRIVATE
    ENGINE_RENDER_OPENGL
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