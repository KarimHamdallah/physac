#pragma once
#include <cmath>

namespace physac
{
    struct Vec2
    {
        float x, y;

        Vec2(float x = 0.0f, float y = 0.0f)
            : x(x), y(y) {}

        // instance methods
        void add(const Vec2& v)
        {
            x += v.x;
            y += v.y;
        }
        void sub(const Vec2& v)
        {
            x -= v.x;
            y -= v.y;
        }
        void normalize()
        {
            float m = mag();
            if (m > 0)
            {
                x /= m;
                y /= m;
            }
        }
        void translate(const Vec2& v)
        {
            add(v);
        }
        void rotate(float angle)
        {
            float c = cosf(angle);
            float s = sinf(angle);
            float nx = x * c - y * s;
            float ny = x * s + y * c;
            x = nx;
            y = ny;
        }
        void scale(float s)
        {
            x *= s;
            y *= s;
        }
        float dot(const Vec2& v) const
        {
            return x * v.x + y * v.y;
        }
        float mag() const
        {
            return sqrtf(x * x + y * y);
        }
        float mag_sqrd() const
        {
            return (x * x + y * y);
        }

        // operator overloading
        Vec2  operator+ (const Vec2& v) const { return { x + v.x, y + v.y }; }
        Vec2  operator- (const Vec2& v) const { return { x - v.x, y - v.y }; }
        Vec2  operator* (float s)       const { return { x * s,   y * s }; }
        Vec2  operator/ (float s)       const { return { x / s,   y / s }; }
        Vec2& operator+=(const Vec2& v) { add(v); return *this; }
        Vec2& operator-=(const Vec2& v) { sub(v); return *this; }
        Vec2& operator*=(float s) { scale(s); return *this; }
        bool  operator==(const Vec2& v) const { return x == v.x && y == v.y; }
        bool  operator!=(const Vec2& v) const { return !(*this == v); }
    };


    inline Vec2 translate_vec2(const Vec2& v, const Vec2& t)
    {
        return { v.x + t.x, v.y + t.y };
    }
    inline Vec2 rotate_vec2(const Vec2& v, float angle)
    {
        float c = cosf(angle);
        float s = sinf(angle);
        return { v.x * c - v.y * s, v.x * s + v.y * c };
    }
    inline Vec2 scale_vec2(const Vec2& v, float s)
    {
        return { v.x * s, v.y * s };
    }
    inline Vec2 normalize_vec2(const Vec2& v)
    {
        float m = v.mag();
        return m > 0 ? Vec2{ v.x / m, v.y / m } : Vec2{};
    }
}
