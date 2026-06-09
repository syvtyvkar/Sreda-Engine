// (c) Nikita Rogalev. All rights reserved.

#pragma once

/**
 * @brief Базовый пакет зависимостей движка для упрощения разработки
 */

#include "Engine/Core/Log.h"                    // Поддержка логирования
#include "Engine/Core/Utilities/Types.h"        // Базовые типы движка
#include "Engine/Core/Utilities/TypeLib.h"      // Операции с типами
//#include "Engine/Core/Utilities/Math.h"       // Своя математика (пока без нее)
#include "Engine/Core/Utilities/Event.h"        // Поддержка ивентов и делегатов

#include "Engine/Core/FileSystem/FileIO.h"      // Работа с файловой системой
#include "Engine/Core/Time.h"                   // Система времени
#include "Engine/Core/GlobalConst.h"            // Глобальные константы
#include "Engine/Core/EngineConfig.h"           // Конфигурация движка
#include "Engine/Input/KeyCodes.h"              // Определения клавиш (InputKey)