/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Point3.cpp
 *
 * A C++ module implementing a 3D point class to represent
 * a position in space.
 */

#include "Point3.h"

/**
 * Copy constructor with Vec4
 * @param v - A 4D vector to convert into a 3D point
 */
Point3::Point3(const Vec4& v)
    /* It is up to the caller to avoid division by zero */
    : x(v.x / v.w)
    , y(v.y / v.w)
    , z(v.z / v.w)
{
    /* empty */
} /* Copy constructor with Vec4 */

/**
 * Returns the difference of this point minus another point as a 3D vector
 * @param p - A 3D point to subtract from this point
 * @return The difference of this point minus another point as a 3D vector
 */
Vec3 Point3::operator-(const Point3& p) const
{
    return Vec3((x - p.x), (y - p.y), (z - p.z));
} /* operator-() */
