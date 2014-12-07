/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Vec3.h
 *
 * A C++ module implementing a 3D vector class to represent
 * a magnitude and direction in space.
 */

#ifndef VEC3_H_
#define VEC3_H_

#include <cmath>
#include <cstdio>

#include "Point3.h"
#include "Vec4.h"

/* Forward declarations */
class Point3;
class Vec4;

class Vec3
{
public:
    /* Public data members */
    float x;
    float y;
    float z;

    /**
     * Default constructor
     * @param a - Component x value
     * @param b - Component y value
     * @param c - Component z value
     */
    Vec3(float a = 0.0f, float b = 0.0f, float c = 0.0f)
        : x(a)
        , y(b)
        , z(c)
    {
        /* empty */
    } /* Default constructor */

    /* Copy constructor with Point3 */
    Vec3(const Point3& p);

    /* Copy constructor with Vec3 */
    Vec3(const Vec3& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
    {
        /* empty */
    } /* Copy constructor with Vec3 */

    /* Copy constructor with Vec4 */
    Vec3(const Vec4& v);

    /* Public member functions */
    float Length() const;
    float Length2() const;
    Vec3 Negate() const;
    Vec3 Normalize() const;
    Vec3 operator+(const Vec3& v) const;
    Vec3 operator-(const Vec3& v) const;
    Vec3 operator*(float c) const;
    Vec3 operator/(float c) const;
    const Vec3& operator=(const Vec3& v);
    void Print(FILE* f) const;
}; /* Vec3 class */

#endif /* VEC3_H_ */
