//
//  vector.h
//  GLBox-XCode
//
//  Created by Eric Henderson on 9/18/12.
//
//

#ifndef vector_h
#define vector_h

#include <math.h>

struct vec2 {
    float x;
    float y;
    
    // +--------------+
    // | constructors |
    // +--------------+
    
    vec2(float s = 0.0) : x(s), y(s) {}
    vec2(float x, float y) : x(x), y(y) {}
    vec2(const vec2& v) { x = v.x; y = v.y; }
    
    // +--------------+
    // | index access |
    // +--------------+
    
    float& operator [] (int i) { return *(&x + i); }
    const float operator [] (int i) const { return *(&x + i); }
    
    // +-------------------------+
    // | non-modifying operators |
    // +-------------------------+
    
    // negation
    vec2 operator - () { return vec2(-x, -y); }
    
    // addition
    vec2 operator + (const vec2& v) const { return vec2(x+v.x, y+v.y); }
    
    // subtraction
    vec2 operator - (const vec2& v) const { return vec2(x-v.x, y-v.y); }
    
    // scalar multiplication
    vec2 operator * (const float s) const { return vec2(x*s, y*s); }
    
    // vector multiplication
    vec2 operator * (const vec2& v) const { return vec2(x*v.x, y*v.y); }
    
    // scalar multiplication helper
    friend vec2 operator * (const float s, const vec2& v) { return v * s; }
    
    // scalar division
    vec2 operator / (const float s) const { return (s == 0) ? vec2() : *this * (1.0 / s); }
    
    // dot product
    float operator ^ (const vec2& v) const { return x*v.x + y*v.y; }
    
    // length
    float operator ! () { return sqrtf(*this ^ *this); }
    
    // normalize
    vec2 operator ~ () { return *this / !(*this); }
    
    // +---------------------+
    // | modifying operators |
    // +---------------------+
    
    // addition
    vec2& operator += (const vec2& v) { x += v.x; y += v.y; return *this; }
    
    // subtraction
    vec2& operator -= (const vec2& v) { x -= v.x; y -= v.y; return *this; }
    
    // scalar multiplication
    vec2& operator *= (const float s) { x *= s; y *= s; return *this; }
    
    // vector multiplication
    vec2& operator *= (const vec2& v) { x *= v.x; y *= v.y; return *this; }
    
    // scalar division
    vec2& operator /= (const float s) { if(s == 0) { return *this; } *this *= (1.0/s); return *this; }
    
    // +----------------------+
    // | conversion operators |
    // +----------------------+
    
    operator const float* () const { return static_cast<const float*>( &x ); }
    
    operator float* () { return static_cast<float*>( &x ); }
};

struct vec3 {
    float x;
    float y;
    float z;
    
    // +--------------+
    // | constructors |
    // +--------------+
    
    vec3(float s = 0.0) : x(s), y(s), z(s) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    vec3(const vec3& v) { x = v.x; y = v.y; z = v.z; }
    vec3(const vec2& v, const float z) : z(z) { x = v.x; y = v.y; }
    
    // +--------------+
    // | index access |
    // +--------------+
    
    float& operator [] (int i) { return *(&x + i); }
    const float operator [] (int i) const { return *(&x + i); }
    
    // +-------------------------+
    // | non-modifying operators |
    // +-------------------------+
    
    // negation
    vec3 operator - () { return vec3(-x, -y, -z); }
    
    // addition
    vec3 operator + (const vec3& v) const { return vec3(x+v.x, y+v.y, z+v.z); }
    
    // subtraction
    vec3 operator - (const vec3& v) const { return vec3(x-v.x, y-v.y, z-v.z); }
    
    // scalar multiplication
    vec3 operator * (const float s) const { return vec3(x*s, y*s, z*s); }
    
    // vector multiplication
    vec3 operator * (const vec3& v) const { return vec3(x*v.x, y*v.y, z*v.z); }
    
    // scalar multiplication helper
    friend vec3 operator * (const float s, const vec3& v) { return v * s; }
    
    // scalar division
    vec3 operator / (const float s) const { return (s == 0) ? vec3() : *this * (1.0 / s); }
    
    // dot product
    float operator ^ (const vec3& v) const { return x*v.x + y*v.y + z*v.z; }
    
    // cross product
    vec3 operator & (const vec3& v) const { return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    
    // length
    float operator ! () { return sqrtf(*this ^ *this); }
    
    // normalize
    vec3 operator ~ () { return *this / !(*this); }
    
    // +---------------------+
    // | modifying operators |
    // +---------------------+
    
    // addition
    vec3& operator += (const vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    
    // subtraction
    vec3& operator -= (const vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    
    // scalar multiplication
    vec3& operator *= (const float s) { x *= s; y *= s; z *= s; return *this; }
    
    // vector multiplication
    vec3& operator *= (const vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    
    // scalar division
    vec3& operator /= (const float s) { if(s == 0) { return *this; } *this *= (1.0/s); return *this; }
    
    // cross product
    vec3& operator &= (const vec3& v) { x = y * v.z - z * v.y; y = z * v.x - x * v.z; z = x * v.y - y * v.x; return *this; }
    
    // +----------------------+
    // | conversion operators |
    // +----------------------+
    
    operator const float* () const { return static_cast<const float*>( &x ); }
    
    operator float* () { return static_cast<float*>( &x ); }
};
    
struct vec4 {
    float x;
    float y;
    float z;
    float w;
    
    // +--------------+
    // | constructors |
    // +--------------+
    
    vec4(float s = 0.0) : x(s), y(s), z(s), w(s) {}
    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    vec4(const vec4& v) { x = v.x; y = v.y; z = v.z; w = v.w; }
    vec4(const vec3& v, const float w = 1.0) : w(w) { x = v.x; y = v.y; z = v.z; }
    vec4(const vec2& v, const float z, const float w) : z(z), w(w) { x = v.x; y = v.y; }
    
    // +--------------+
    // | index access |
    // +--------------+
    
    float& operator [] (int i) { return *(&x + i); }
    const float operator [] (int i) const { return *(&x + i); }
    
    // +-------------------------+
    // | non-modifying operators |
    // +-------------------------+
    
    // negation
    vec4 operator - () { return vec4(-x, -y, -z, -w); }
    
    // addition
    vec4 operator + (const vec4& v) const { return vec4(x+v.x, y+v.y, z+v.z, w+v.w); }
    
    // subtraction
    vec4 operator - (const vec4& v) const { return vec4(x-v.x, y-v.y, z-v.z, w-v.w); }
    
    // scalar multiplication
    vec4 operator * (const float s) const { return vec4(x*s, y*s, z*s, w*s); }
    
    // vector multiplication
    vec4 operator * (const vec4& v) const { return vec4(x*v.x, y*v.y, z*v.z, w*v.w); }
    
    // scalar multiplication helper
    friend vec4 operator * (const float s, const vec4& v) { return v * s; }
    
    // scalar division
    vec4 operator / (const float s) const { return (s == 0) ? vec4() : *this * (1.0 / s); }
    
    // dot product
    float operator ^ (const vec4& v) const { return x*v.x + y*v.y + z*v.z + w*v.w; }
    
    // cross product
    vec3 operator & (const vec3& v) const { return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    
    // length
    float operator ! () { return sqrtf(*this ^ *this); }
    
    // normalize
    vec4 operator ~ () { return *this / !(*this); }
    
    // +---------------------+
    // | modifying operators |
    // +---------------------+
    
    // addition
    vec4& operator += (const vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
    
    // subtraction
    vec4& operator -= (const vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
    
    // scalar multiplication
    vec4& operator *= (const float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    
    // vector multiplication
    vec4& operator *= (const vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
    
    // scalar division
    vec4& operator /= (const float s) { if(s == 0) { return *this; } *this *= (1.0/s); return *this; }
    
    // +----------------------+
    // | conversion operators |
    // +----------------------+
    
    operator const float* () const { return static_cast<const float*>( &x ); }
        
    operator float* () { return static_cast<float*>( &x ); }
};

#endif
