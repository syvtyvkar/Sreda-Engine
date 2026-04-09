// (c) Nikita Rogalev. All rights reserved.

#pragma once // Защита от множественного включения

#include <cmath>        // Для математических функций (std::sqrt)
#include <cstring>
#include <string>

namespace Engine 
{
    using namespace std;
    /*Трёхмерный вектор с компонентами x, y, z.
    Содержит базовые арифметические операции и часто используемые статические векторы*/
    struct Vector3 
    {
        float x, y, z;

        // Конструкторы
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        // Арифметические операторы
        Vector3 operator+(const Vector3& other) const {return Vector3(x + other.x, y + other.y, z + other.z);}
        Vector3 operator-(const Vector3& other) const {return Vector3(x - other.x, y - other.y, z - other.z);}
        Vector3 operator*(float scalar) const {return Vector3(x * scalar, y * scalar, z * scalar);}
        Vector3 operator/(float scalar) const {return Vector3(x / scalar, y / scalar, z / scalar);}
        Vector3& operator+=(const Vector3& other) {x += other.x; y += other.y; z += other.z;return *this;}
        Vector3& operator-=(const Vector3& other) {x -= other.x; y -= other.y; z -= other.z;return *this;}
        bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z;}

        // Полезные методы
        float length() const { return std::sqrt(x*x + y*y + z*z); }                                 // Евклидова длина вектора
        Vector3 normalized() const {float len = length();return len > 0 ? *this / len : Vector3();} // Нормализованная копия (если длина не ноль)
        // Статические методы для получения часто используемых векторов
        static Vector3 zero() { return Vector3(0, 0, 0); }
        static Vector3 one() { return Vector3(1, 1, 1); }
        static Vector3 up() { return Vector3(0, 1, 0); }
        static Vector3 forward() { return Vector3(0, 0, 1); }
        static Vector3 right() { return Vector3(1, 0, 0); }
    };

    /*Двухмерный вектор с компонентами x, y*/
    struct Vector2 
    {
        float x, y;

        Vector2() : x(0), y(0) {}
        Vector2(float x, float y) : x(x), y(y) {}

        Vector2 operator+(const Vector2& other) const {return Vector2(x + other.x, y + other.y);}
        Vector2 operator*(float scalar) const {return Vector2(x * scalar, y * scalar);}
    };

    /*Четырёхмерный вектор (используется для представления цветов в формате RGBA или однородных координат)*/
    struct Vector4 
    {
        float x, y, z, w;

        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    };

    /*Матрица 4x4 с данными в виде плоского массива*/
    struct Matrix4 
    {
        float data[16];

        Matrix4() {for (int i = 0; i < 16; i++) {data[i] = (i % 5 == 0) ? 1.0f : 0.0f; }}   // Единичная: индексы 0,5,10,15 становятся 1

        static Matrix4 identity() 
        {
            Matrix4 m;
            // Единичная матрица (аналогично конструктору)
            for (int i = 0; i < 16; i++) m.data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
            return m;
        }
    };

    /*Матрица 3x3*/
    struct Matrix3 
    {
        float data[9];

        Matrix3() {for (int i = 0; i < 9; i++) data[i] = (i % 5 == 0) ? 1.0f : 0.0f;} // Инициализация единичной матрицей

        static Matrix3 identity() 
        {
            Matrix3 m;
            for (int i = 0; i < 9; i++) m.data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
            return m;
        }
    };

    /*Матрица 2x2*/
    struct Matrix2 
    {
        float data[4];

        Matrix2() {for (int i = 0; i < 4; i++) data[i] = (i % 5 == 0) ? 1.0f : 0.0f;} // Инициализация единичной матрицей

        static Matrix2 identity() 
        {
            Matrix2 m;
            for (int i = 0; i < 4; i++) m.data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
            return m;
        }
    };

    /*Представляет цвет в формате RGBA (float от 0 до 1)*/
    struct Color
    {
        float r, g, b, a;

        Color(float r = 1, float g = 1, float b = 1, float a = 1): r(r), g(g), b(b), a(a) {}
        Vector4 toVec4() const { return Vector4(r, g, b, a); }                                  // Преобразование в Vector4
    };

    /*Хранит позицию, поворот (углы Эйлера в градусах) и масштаб.
    Предоставляет простые методы для изменения и получения направлений*/
    struct Transform 
    {
        Vector3 position;
        Vector3 rotation;  // Углы Эйлера в градусах (вокруг осей X, Y, Z)
        Vector3 scale;

        Transform() : position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1) {}

        void translate(const Vector3& offset) {position += offset;}                                                         // Перемещение на заданное смещение
        void rotate(const Vector3& angles) {rotation += angles;}                                                            // Поворот на заданные углы (суммируются с текущими)
        void scaleBy(const Vector3& factor) {scale = Vector3(scale.x * factor.x, scale.y * factor.y, scale.z * factor.z);}  // Масштабирование покомпонентно
        Vector3 forward() const {return Vector3(0, 0, 1);}                                                                  // Направление "вперёд" в локальных координатах (просто +Z)
        Vector3 up() const {return Vector3(0, 1, 0);}                                                                       // Направление "вверх" (+Y)
        Vector3 right() const {return Vector3(1, 0, 0);}                                                                    // Направление "вправо" (+X)
    };

    /*Описывает вершину для рендеринга: позиция, нормаль, текстурные координаты*/
    struct Vertex 
    {
        Vector3 position;
        Vector3 normal;
        Vector2 texCoord;
    };

    /*Описывает строки, альтернатива std::string*/
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