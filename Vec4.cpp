/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Vec4.cpp
 *
 * A C++ module implementing a 4D vector class to represent
 * a magnitude and direction in space.
 */

#include "Vec4.h"

/**
 * Copy constructor with Point3
 * @param v - A 3D point to convert to a 4D vector
 */
Vec4::Vec4(const Point3& p)
    : x(p.x)
    , y(p.y)
    , z(p.z)
    , w(1.0f)
{
    /* empty */
} /* Copy constructor with Point3 */

/**
 * Copy constructor with Vec3
 * @param v - A 3D vector to convert to a 4D vector
 */
Vec4::Vec4(const Vec3& v)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(0.0f)
{
    /* empty */
} /* Copy constructor with Vec3 */

/**
 * Returns the length of the vector
 * @return - The length of the vector
 */
float Vec4::Length() const
{
    return sqrt((x * x) + (y * y) + (z * z) + (w * w));
} /* Length() */

/**
 * Returns the squared length of the vector
 * @return - The squared length of the vector
 */
float Vec4::Length2() const
{
    return (x * x) + (y * y) + (z * z) + (w * w);
} /* Length2() */

/**
 * Returns the negated vector
 * @return - The negated vector
 */
Vec4 Vec4::Negate() const
{
    return Vec4(-x, -y, -z, -w);
} /* Negate() */

/**
 * Returns the normalized vector
 * @return - The normalized vector
 */
Vec4 Vec4::Normalize() const
{
    float length = Length();
    return Vec4((x / length), (y / length), (z / length), (w / length));
} /* Normalize() */

/**
 * Returns the sum of this plus another Vec4
 * @param v - A 4D vector to sum with this vector
 * @return - The sum of this plus another Vec4
 */
Vec4 Vec4::operator+(const Vec4& v) const
{
    return Vec4((x + v.x), (y + v.y), (z + v.z), (w + v.w));
} /* operator+() */

/**
 * Returns the difference of this minus another Vec4
 * @param v - A 4D vector to subtract from this vector
 * @return - The difference of this minus another Vec4
 */
Vec4 Vec4::operator-(const Vec4& v) const
{
    return Vec4((x - v.x), (y - v.y), (z - v.z), (w - v.w));
} /* operator-() */

/**
 * Returns the product of this times a scalar
 * @param c - A scalar to multiply with this vector
 * @return - The product of this times a scalar
 */
Vec4 Vec4::operator*(float c) const
{
    return Vec4((c * x), (c * y), (c * z), (c * w));
} /* operator*() */

/**
 * Returns the quotient of this divided by a scalar
 * @param c - A scalar to divide into this vector
 * @return - The quotient of this divided by a scalar
 */
Vec4 Vec4::operator/(float c) const
{
    return Vec4((x / c), (y / c), (z / c), (w / c));
} /* operator/() */

/**
 * Assigns another Vec4's values to this vector and returns this vector
 * @param v - A 4D vector to copy into this vector
 * @return - A constant reference to this vector
 */
const Vec4& Vec4::operator=(const Vec4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this; /* enables cascading assignments */
} /* operator=() */

/**
 * Prints this vector's values
 * @param f - A file object to print to
 */
void Vec4::Print(FILE* f) const
{
    fprintf(f,
        "Vec4:\n"
        " x = %f\n"
        " y = %f\n"
        " z = %f\n"
        " w = %f\n"
        , x, y, z, w
    );
} /* Print() */
