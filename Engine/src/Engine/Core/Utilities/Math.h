// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include "Engine/Core/Utilities/Types.h"      // Include engine core types (e.g., Vector3)
#include <stdint.h>                 // For fixed-width integer types (uint32_t)

#define PI 3.14159                  // Pi constant (approximate)

namespace EngineMath
{
    static float ToDegress(float Radians) {return (Radians * 180.0f) / PI;};    // Convert radians to degrees
    static float ToRadians(float Degress) {return (Degress * PI) / 180.0f;};    // Convert degrees to radians

    /**
     * Fast inverse square root.
     * Uses the famous method from Quake III Arena with magic constant 0x5f3759df.
     * Computes 1 / sqrt(InValue) with high speed and acceptable precision.
     * 
     * @param InValue Input value (must be positive)
     * @return Approximate value of 1 / sqrt(InValue)
     */
    float InvSqrt(float InValue) 
	{
		const float x2 = InValue * 0.5F;
	    const float threehalfs = 1.5F;

	    union {
		    float f;                                    // Floating-point representation
		    uint32_t i;                                 // Integer representation of the same bits
	    } conv = {InValue};                             // Initialize union with InValue (f field)
	    conv.i = 0x5f3759df - ( conv.i >> 1 );          // Magic operation: bit shift and subtract from constant
	    conv.f *= threehalfs - x2 * conv.f * conv.f;    // One Newton iteration for result refinement
	    return conv.f;
	};

    /**
     * Vector normalization (reduce to unit length) using fast InvSqrt.
     * If the vector is already normalized or its length is less than Tolerance, the original vector is returned.
     * 
     * @param Tolerance Tolerance for checking near-zero length (to avoid division by zero)
     * @param InVector Input vector
     * @return Normalized vector
     */
    static Engine::Vector3 Vector3Normal(float Tolerance = 0.f, Engine::Vector3 InVector = Engine::Vector3())
    {
        const float SquareSum = InVector.x*InVector.x + InVector.y*InVector.y + InVector.z*InVector.z;  // Compute sum of squared components (length squared)
        if(SquareSum == 1.f)                                                                            // If already unit length, return unchanged
        {
            return InVector;
        }		
        else if(SquareSum < Tolerance)                                                                  // If vector is too short (less than Tolerance), return original (to avoid huge numbers)
        {
            return InVector;
        }
        const float Scale = EngineMath::InvSqrt(SquareSum);                                                         // Compute scale factor as inverse square root of SquareSum
        return Engine::Vector3(InVector.x*Scale, InVector.y*Scale, InVector.z*Scale);                   // Multiply each component by Scale and return new vector
    };

    /**
     * Cross product of two vectors.
     * 
     * @param q1 First vector
     * @param q2 Second vector
     * @return Cross product q1 × q2
     */
    static Engine::Vector3 Cross(Engine::Vector3 q1, Engine::Vector3 q2)
    {
        return Engine::Vector3(
			q1.y * q2.z - q1.z * q2.y,
			q1.z * q2.x - q1.x * q2.z,
			q1.x * q2.y - q1.y * q2.x);
    };

    /*template<typename T> T Range(T,T);
    template<typename T> T Range(T a, T b)
    {
        return a+b;
    }*/
}
