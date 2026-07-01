// (c) Nikita Rogalev. All rights reserved.

#pragma once

/**
 * @brief Basic engine dependency package for simplified development
 */

#include "Engine/Core/Utilities/Log.h"                    // Поддержка логирования
#include "Engine/Core/Utilities/Types.h"        // Базовые типы движка
#include "Engine/Core/Utilities/TypeLib.h"      // Операции с типами
//#include "Engine/Core/Utilities/Math.h"       // Своя математика (пока без нее)
#include "Engine/Core/Utilities/Event.h"        // Поддержка ивентов и делегатов

#include "Engine/Core/FileSystem/ResourceManager.h"
#include "Engine/Core/FileSystem/FileIO.h"      // Работа с файловой системой
#include "Engine/Core/Utilities/Time.h"                   // Система времени
#include "Engine/Core/EngineConfig.h"           // Конфигурация движка
#include "Engine/Input/KeyCodes.h"              // Определения клавиш (InputKey)