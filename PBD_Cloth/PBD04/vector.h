#pragma once
#ifndef Vector_h
#define Vector_h
#include<cmath>

struct Vec3
{
    float x;
    float y;
    float z;

    Vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vec3() {}

    Vec3 operator-() { return Vec3(-x, -y, -z); }
    Vec3 operator+ (const Vec3& v) { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator- (const Vec3& v) { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator* (const float& n) { return Vec3(x * n, y * n, z * n); }
    Vec3 operator/ (const float& n) { return Vec3(x / n, y / n, z / n); }
    void operator+= (const Vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    Vec3 cross(const Vec3& v) { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    float dot(const Vec3& v) { return x * v.x + y * v.y + z * v.z; }
    float length() { return sqrt(x * x + y * y + z * z); }
    void normalize()
    {
        float len = length();
        x /= len;
        y /= len;
        z /= len;
    }
};

#endif /* Vector_h */

