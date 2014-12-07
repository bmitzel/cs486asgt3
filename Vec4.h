/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Vec4.h
 *
 * A C++ module implementing a 4D vector class to represent
 * a magnitude and direction in space.
 */

#ifndef VEC4_H_
#define VEC4_H_

#include <cmath>
#include <cstdio>

#include "Point3.h"
#include "Vec3.h"

/* Forward declarations */
class Point3;
class Vec3;

class Vec4
{
public:
    /* Public data members */
    float x;
    float y;
    float z;
    float w;

    /**
     * Default constructor
     * @param a - Component x value
     * @param b - Component y value
     * @param c - Component z value
     * @param d - Component w value
     */
    Vec4(float a = 0.0f, float b = 0.0f, float c = 0.0f, float d = 0.0f)
        : x(a)
        , y(b)
        , z(c)
        , w(d)
    {
        /* empty */
    } /* Default constructor */

    /* Copy constructor with Point3 */
    Vec4(const Point3& p);

    /* Copy constructor with Vec3 */
    Vec4(const Vec3& v);

    /**
     * Copy constructor with Vec4
     * @param v - A 4D vector
     */
    Vec4(const Vec4& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(v.w)
    {
        /* empty */
    } /* Copy constructor with Vec4 */

    /* Public member functions */
    float Length() const;
    float Length2() const;
    Vec4 Negate() const;
    Vec4 Normalize() const;
    Vec4 operator+(const Vec4& v) const;
    Vec4 operator-(const Vec4& v) const;
    Vec4 operator*(float c) const;
    Vec4 operator/(float c) const;
    const Vec4& operator=(const Vec4& v);
    void Print(FILE* f) const;
}; /* Vec4 class */

#endif /* VEC4_H_ */
