
#execute_process(
#    COMMAND git describe --tags --always --dirty
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#    OUTPUT_VARIABLE GIT_VERSION
#    OUTPUT_STRIP_TRAILING_WHITESPACE
#    ERROR_QUIET
#)

# Создаём .rc файл из шаблона
#configure_file(
#    ${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc.in
#    ${CMAKE_CURRENT_BINARY_DIR}/app.rc
#    @ONLY
#)

add_executable(Game 
    src/Game.cpp
    $<$<PLATFORM_ID:Windows>:appdata/app.rc>
)

target_include_directories(Game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../Engine/Resources"
        "$<TARGET_FILE_DIR:Game>/Resources"
    COMMENT "Copying Engine Resources to Game output directory..."
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../Game/Config"
        "$<TARGET_FILE_DIR:Game>/Config"
    COMMENT "Copying configs to Game output directory..."
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../Game/Resources"
        "$<TARGET_FILE_DIR:Game>/Resources"
    COMMENT "Copying game contents to Game output directory..."
)

target_link_libraries(Game PRIVATE Engine)
