// (c) Nikita Rogalev. All rights reserved.

#pragma once
#include <iostream>

#include "Engine/Render/Camera.h"
#include "Engine/Core/GlobalConst.h"

//nlohmann-json
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <functional>
#include <mutex>
///////////////

/* 
* TODO: Изначально, я хотел указывать здесь базовые конфигурации движка. Сейчас судьба этого файла не определена.
*/

float g_deltaTime = 0.0f;
float g_lastFrame = 0.0f;

