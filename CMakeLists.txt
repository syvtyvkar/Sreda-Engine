cmake_minimum_required(VERSION 3.26.0)

set(CMAKE_TOOLCHAIN_FILE "F:/Engine/Engine/external/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(Engine VERSION 1.0.0 LANGUAGES CXX C)

cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/Bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#-----

add_subdirectory(Engine)
add_subdirectory(Game)


# === Embedded Config ===

set(EMBED_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/embed_config.cmake")
set(EMBEDDED_CONFIG_HEADER "${CMAKE_BINARY_DIR}/generated/config_embed.h")

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")

message(STATUS "=== DEBUG: Embed Config Paths ===")
message(STATUS "  CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "  Config file: ${CMAKE_CURRENT_SOURCE_DIR}/Config/default_engine.json")
message(STATUS "  Script: ${EMBED_SCRIPT}")
message(STATUS "  Output: ${EMBEDDED_CONFIG_HEADER}")
message(STATUS "==============================")

add_custom_target(EmbedConfig
    COMMAND ${CMAKE_COMMAND}
        -DEMBED_CONFIG_INPUT=${CMAKE_CURRENT_SOURCE_DIR}/Config/default_engine.json
        -DEMBED_CONFIG_OUTPUT=${EMBEDDED_CONFIG_HEADER}
        -DEMBED_CONFIG_VAR=DefaultConfigJSON
        -P ${EMBED_SCRIPT}
    BYPRODUCTS ${EMBEDDED_CONFIG_HEADER}
    DEPENDS 
        ${CMAKE_CURRENT_SOURCE_DIR}/Config/default_engine.json
        ${EMBED_SCRIPT}
    COMMENT "Embedding default config into binary..."
    VERBATIM
    USES_TERMINAL
)

target_include_directories(Engine PRIVATE 
    ${CMAKE_BINARY_DIR}/generated
)

add_dependencies(Engine EmbedConfig)

target_compile_definitions(Engine PRIVATE 
    ENGINE_CONFIG_EMBEDDED=1
)