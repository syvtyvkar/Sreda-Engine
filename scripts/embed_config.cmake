# scripts/embed_config.cmake

set(INPUT_FILE ${EMBED_CONFIG_INPUT})
set(OUTPUT_FILE ${EMBED_CONFIG_OUTPUT})
set(VARIABLE_NAME ${EMBED_CONFIG_VAR})

# Нормализуем путь
file(TO_NATIVE_PATH "${INPUT_FILE}" INPUT_FILE_NATIVE)

# Проверка файла
if(NOT EXISTS "${INPUT_FILE}")
    message(FATAL_ERROR "Config file not found: ${INPUT_FILE}")
endif()

# Создаём директорию вывода
get_filename_component(OUTPUT_DIR "${OUTPUT_FILE}" DIRECTORY)
file(MAKE_DIRECTORY "${OUTPUT_DIR}")

# Читаем файл
file(READ "${INPUT_FILE}" CONFIG_CONTENT)

# ✅ ПРАВИЛЬНЫЙ ПОРЯДОК экранирования для C++ строки:
# 1. Сначала обратные слеши (иначе сломаются экранированные кавычки)
string(REPLACE "\\" "\\\\" CONFIG_CONTENT "${CONFIG_CONTENT}")

# 2. Потом кавычки
string(REPLACE "\"" "\\\"" CONFIG_CONTENT "${CONFIG_CONTENT}")

# 3. Потом специальные символы
string(REPLACE "\n" "\\n" CONFIG_CONTENT "${CONFIG_CONTENT}")
string(REPLACE "\r" "\\r" CONFIG_CONTENT "${CONFIG_CONTENT}")
string(REPLACE "\t" "\\t" CONFIG_CONTENT "${CONFIG_CONTENT}")

# ✅ Генерируем header с правильным синтаксисом
file(WRITE "${OUTPUT_FILE}"
"#pragma once\n"
"// Auto-generated: DO NOT EDIT\n"
"// Source: ${INPUT_FILE_NATIVE}\n\n"
"namespace Engine { namespace ConfigEmbed {\n"
"    constexpr const char* ${VARIABLE_NAME} =\n"
"        \"${CONFIG_CONTENT}\";\n"
"} // namespace ConfigEmbed\n"
"} // namespace Engine\n"
)

message(STATUS "Embedded config: ${INPUT_FILE_NATIVE} -> ${OUTPUT_FILE}")