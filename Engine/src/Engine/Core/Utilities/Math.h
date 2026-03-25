// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include "Engine/Core/Utilities/Types.h"      // Подключение основных типов движка (например, Vector3)
#include <stdint.h>                 // Для целочисленных типов фиксированной ширины (uint32_t)

#define PI 3.14159                  // Определение числа Пи (приближённое значение)

namespace EngineMath
{
    static float ToDegress(float Radians) {return (Radians * 180.0f) / PI;};    // Преобразование радиан в градусы
    static float ToRadians(float Degress) {return (Degress * PI) / 180.0f;};    // Преобразование градусов в радианы

    /**
     * Быстрый обратный квадратный корень (Fast Inverse Square Root).
     * Использует знаменитый метод из Quake III Arena с магической константой 0x5f3759df.
     * Вычисляет 1 / sqrt(InValue) с высокой скоростью и приемлемой точностью.
     * 
     * @param InValue Входное значение (должно быть положительным)
     * @return Приближённое значение 1 / sqrt(InValue)
     */
    float InvSqrt(float InValue) 
	{
		const float x2 = InValue * 0.5F;
	    const float threehalfs = 1.5F;

	    union {
		    float f;                                    // Представление числа с плавающей точкой
		    uint32_t i;                                 // Целочисленное представление тех же битов
	    } conv = {InValue};                             // Инициализируем union значением InValue (поле f)
	    conv.i = 0x5f3759df - ( conv.i >> 1 );          // Магическая операция: сдвиг битов и вычитание из константы
	    conv.f *= threehalfs - x2 * conv.f * conv.f;    // Одна итерация метода Ньютона для уточнения результата
	    return conv.f;
	};

    /**
     * Нормализация вектора (приведение к единичной длине) с использованием быстрого InvSqrt.
     * Если вектор уже нормализован или его длина меньше допуска Tolerance, возвращается исходный вектор.
     * 
     * @param Tolerance Допуск для проверки близости к нулевой длине (чтобы избежать деления на ноль)
     * @param InVector Входной вектор
     * @return Нормализованный вектор
     */
    static Engine::Vector3 Vector3Normal(float Tolerance = 0.f, Engine::Vector3 InVector)
    {
        const float SquareSum = InVector.x*InVector.x + InVector.y*InVector.y + InVector.z*InVector.z;  // Вычисляем сумму квадратов компонентов (квадрат длины)
        if(SquareSum == 1.f)                                                                            // Если вектор уже единичной длины, возвращаем его без изменений
        {
            return InVector;
        }		
        else if(SquareSum < Tolerance)                                                                  // Если вектор слишком короткий (меньше Tolerance), возвращаем исходный (чтобы избежать огромных чисел)
        {
            return InVector;
        }
        const float Scale = InvSqrt(SquareSum);                                                         // Вычисляем масштабный коэффициент как обратный квадратный корень от SquareSum
        return Engine::Vector3(InVector.x*Scale, InVector.y*Scale, InVector.z*Scale);                   // Умножаем каждую компоненту на Scale и возвращаем новый вектор
    };

    /**
     * Векторное произведение (cross product) двух векторов.
     * 
     * @param q1 Первый вектор
     * @param q2 Второй вектор
     * @return Векторное произведение q1 × q2
     */
    static Engine::Vector3 Cross(Engine::Vector3 q1, Engine::Vector3 q2)
    {
        return Engine::Vector3(
			q1.y * q2.z - q1.z * q2.y,
			q1.z * q2.x - q1.x * q2.z,
			q1.x * q2.y - q1.y * q2.x);
    };
}