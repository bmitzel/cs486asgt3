/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Point3.h
 *
 * A C++ module implementing a 3D point class to represent
 * a position in space.
 */

#ifndef POINT3_H_
#define POINT3_H_

#include "Vec3.h"
#include "Vec4.h"

/* Forward declarations */
class Vec3;
class Vec4;

class Point3
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
    Point3(float a = 0.0f, float b = 0.0f, float c = 0.0f)
        : x(a)
        , y(b)
        , z(c)
    {
        /* empty */
    }

    /* Copy constructor with Vec4 */
    Point3(const Vec4& v);

    /* Member functions */
    Vec3 operator-(const Point3& p) const;
}; /* Point3 class */

#endif /* POINT3_H_ */
