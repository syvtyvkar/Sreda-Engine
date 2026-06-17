// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include <cmath>        // For math functions (std::sqrt)
#include <cstring>
#include <string>
#include <memory>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

namespace Engine 
{
    using namespace std;

    /** 
     * @brief Unique pointer
    */
    template<typename T>
	using TUniquePtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr TUniquePtr<T> CreateUniquePtr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

    /** 
     * @brief Reference
    */
	template<typename T>
	using TRef = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr TRef<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

    /** 
     * @brief Weak
    */
	template<typename T>
	using TWeak = std::weak_ptr<T>;
	template<typename T, typename ... Args>
	constexpr TWeak<T> CreateWeak(Args&& ... args)
	{
		return std::shared_ptr<Args>(args)...;
	}

    /*3D vector with x, y, z components.
    Contains basic arithmetic operations and commonly used static vectors*/
    struct Vector3 
    {
        float x, y, z;

        // Constructors
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        Vector3(float xyz) : x(xyz), y(xyz), z(xyz) {}
        Vector3(glm::vec3 glm) : x(glm.x), y(glm.y), z(glm.z) {}
        Vector3(const glm::vec4& v) : x(v.x), y(v.y), z(v.z) {}

        // Arithmetic operators
        Vector3 operator+(const Vector3& other) const {return Vector3(x + other.x, y + other.y, z + other.z);}
        Vector3 operator-(const Vector3& other) const {return Vector3(x - other.x, y - other.y, z - other.z);}
        Vector3 operator*(float scalar) const {return Vector3(x * scalar, y * scalar, z * scalar);}
        Vector3 operator/(float scalar) const {return Vector3(x / scalar, y / scalar, z / scalar);}
        Vector3& operator+=(const Vector3& other) {x += other.x; y += other.y; z += other.z;return *this;}
        Vector3& operator-=(const Vector3& other) {x -= other.x; y -= other.y; z -= other.z;return *this;}
        bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z;}
        glm::vec3 GetGlmVec3() const {return glm::vec3(x,y,z);}

        // Utility methods
        float length() const { return std::sqrt(x*x + y*y + z*z); }                                 // Euclidean length
        Vector3 normalized() const {float len = length();return len > 0 ? *this / len : Vector3();} // Normalized copy (if length is non-zero)
        // Static methods for commonly used vectors
        static Vector3 zero() { return Vector3(0, 0, 0); }
        static Vector3 one() { return Vector3(1, 1, 1); }
        static Vector3 up() { return Vector3(0, 1, 0); }
        static Vector3 forward() { return Vector3(0, 0, 1); }
        static Vector3 right() { return Vector3(1, 0, 0); }
    };

    /*2D vector with x, y components*/
    struct Vector2 
    {
        float x, y;

        Vector2() : x(0), y(0) {}
        Vector2(float x, float y) : x(x), y(y) {}
        Vector2(float xy) : x(xy), y(xy) {}
        Vector2(glm::vec2 glm) : x(glm.x), y(glm.y) {}

        glm::vec2 GetGlmVec2() const { return glm::vec2(x, y); }

        Vector2 operator+(const Vector2& other) const {return Vector2(x + other.x, y + other.y);}
        Vector2 operator*(float scalar) const {return Vector2(x * scalar, y * scalar);}
    };

    /*4D vector (used for RGBA colors or homogeneous coordinates)*/
    struct Vector4 
    {
        float x, y, z, w;

        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        Vector4(const glm::vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

        glm::vec4 GetGlmVec4() const { return glm::vec4(x, y, z, w); }
    };

    /*4x4 matrix with data stored as a flat array*/
    struct Matrix4 
    {
        float data[16];

        Matrix4() {for (int i = 0; i < 16; i++) {data[i] = (i % 5 == 0) ? 1.0f : 0.0f; }}   // Identity: indices 0,5,10,15 become 1

        Matrix4(const glm::mat4& m) 
        {
            const float* src = glm::value_ptr(m);
            memcpy(data, src, 16 * sizeof(float));
        }
    
        glm::mat4 GetGlmMat4() const 
        {
            glm::mat4 result;
            memcpy(glm::value_ptr(result), data, 16 * sizeof(float));
            return result;
        }

        static Matrix4 identity() 
        {
            Matrix4 m;
            // Identity matrix (same as constructor)
            for (int i = 0; i < 16; i++) m.data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
            return m;
        }
    };

    /*3x3 matrix*/
    struct Matrix3 
    {
        float data[9];

        Matrix3() {for (int i = 0; i < 9; i++) data[i] = (i % 5 == 0) ? 1.0f : 0.0f;} // Initialize with identity matrix

        Matrix3(const glm::mat3& m) 
        {
            const float* src = glm::value_ptr(m);
            memcpy(data, src, 9 * sizeof(float));
        }
    
        glm::mat3 GetGlmMat4() const 
        {
            glm::mat3 result;
            memcpy(glm::value_ptr(result), data, 9 * sizeof(float));
            return result;
        }

        static Matrix3 identity() 
        {
            Matrix3 m;
            for (int i = 0; i < 9; i++) m.data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
            return m;
        }
    };

    /*2x2 matrix*/
    struct Matrix2 
    {
        float data[4];

        Matrix2() {for (int i = 0; i < 4; i++) data[i] = (i % 5 == 0) ? 1.0f : 0.0f;} // Initialize with identity matrix

        Matrix2(const glm::mat2& m) 
        {
            const float* src = glm::value_ptr(m);
            memcpy(data, src, 4 * sizeof(float));
        }
    
        glm::mat2 GetGlmMat2() const 
        {
            glm::mat2 result;
            memcpy(glm::value_ptr(result), data, 4 * sizeof(float));
            return result;
        }

        static Matrix2 identity() 
        {
            Matrix2 m;
            for (int i = 0; i < 4; i++) m.data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
            return m;
        }
    };

    /*Represents a color in RGBA format (float 0 to 255)*/
    struct TColor
    {
        int r, g, b, a = 255;

        TColor(float r = 1.f, float g = 1.f, float b = 1.f) : r((int)r), g ((int)g), b((int)b) {}
        TColor(int r = 1, int g = 1, int b = 1, int a = 1): r(r), g(g), b(b), a(a) {}
        TColor(int r = 1, int g = 1, int b = 1): r(r), g(g), b(b), a(255) {}
        TColor(const TColor& InNewColor): r(InNewColor.r), g(InNewColor.g), b(InNewColor.b), a(InNewColor.a) {}
        TColor(): r(255), g(255), b(255), a(255) {}

        glm::vec4 GetGlmVec4() const { return glm::vec4(r/255.f, g/255.f, b/255.f, a/255.f); }

        static const TColor White;
        static const TColor Black;
        static const TColor Red;
        static const TColor Orange;
        static const TColor Yellow;
        static const TColor Green;
        static const TColor Blue;
        static const TColor Magenta;
        static const TColor Gray;

       /* TColor& operator=(TColor&& InOther) noexcept
        {
            if (this == &InOther) return *this;
            r = InOther.r; g = InOther.g; b = InOther.b; a = InOther.a;
        }*/

        /*glm::vec4& operator[]() 
        { 
            glm::vec4 LV4 = GetGlmVec4();
            return LV4; 
        }
        const glm::vec4& operator[]() const 
        {
            glm::vec4 LV4 = GetGlmVec4();
            return LV4; 
        }
        const TColor& operator[]() const 
        { 
            TColor LC = TColor(r,g,b,a);
            return LC; 
        }*/
    };

    /*Stores position, rotation (Euler angles in degrees) and scale.
    Provides simple methods for changing and getting directions*/
    struct Transform 
    {
        Vector3 position;
        Vector3 rotation;  // Euler angles in degrees (around X, Y, Z axes)
        Vector3 scale;

        Transform() : position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1) {}

        void translate(const Vector3& offset) {position += offset;}                                                         // Move by specified offset
        void rotate(const Vector3& angles) {rotation += angles;}                                                            // Rotate by specified angles (added to current)
        void scaleBy(const Vector3& factor) {scale = Vector3(scale.x * factor.x, scale.y * factor.y, scale.z * factor.z);}  // Component-wise scaling
        Vector3 forward() const {return Vector3(0, 0, 1);}                                                                  // Forward direction in local coordinates (just +Z)
        Vector3 up() const {return Vector3(0, 1, 0);}                                                                       // Up direction (+Y)
        Vector3 right() const {return Vector3(1, 0, 0);}                                                                    // Right direction (+X)
    };

    /*Describes a vertex for rendering: position, normal, texture coordinates*/
    struct Vertex 
    {
        Vector3 position;
        Vector3 normal;
        Vector2 texCoord;
    };

    /*Describes strings, alternative to std::string*/
    struct TString 
    {
    public:
        TString() : m_str() {}
        TString(char* InStr)
        {
            if (InStr == nullptr) 
            {
                m_str = new char[1];
                m_str[0] = '\0';
            }
            else 
            {
                m_str = new char[strlen(InStr) + 1];
                strcpy(m_str, InStr);
                m_str[strlen(InStr)] = '\0';
            }
        }
        TString(const TString& source)
        {
            m_str = new char[strlen(source.m_str) + 1];
            strcpy(m_str, source.m_str);
            m_str[strlen(source.m_str)] = '\0';
        };

        TString(TString&& source)
        {
            m_str = source.m_str;
            source.m_str = nullptr;
        };

        void Empty() {  m_str={}; }
        char* GetChar() {return m_str;}
        static TString ToString(char* InStr){return TString(InStr);}
        bool operator==(const TString& other) const { return m_str == other.m_str;}
        TString TString::operator +(const TString &q) const
        {
            TString s;
            s.m_str = new char[strlen(m_str)+strlen(q.m_str)+1];
            strcpy(s.m_str,m_str);
            strcat(s.m_str,q.m_str);
            return s;
        }
        TString TString::operator =(const TString &q)
        {
            if(this!=&q)
            {
                if(m_str) delete[] m_str;
                m_str = new char[strlen(q.m_str)+1];
                strcpy(m_str,q.m_str);
            }
             return *this;
        }
        std::string GetSTDString() {return GetChar();}
        void FromSTDString(std::string InSTDString) 
        {
            if(m_str) delete[] m_str;
            m_str = new char[strlen(InSTDString.c_str())+1];
            strcpy(m_str, InSTDString.c_str()); 
        }
        
    private:
        char* m_str;
    };
}
