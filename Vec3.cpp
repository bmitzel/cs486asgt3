/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Vec3.cpp
 *
 * A C++ module implementing a 3D vector class to represent
 * a magnitude and direction in space.
 */

#include "Vec3.h"

/**
 * Copy constructor with Point3
 * @param v - A 3D point to convert to a 3D vector
 */
Vec3::Vec3(const Point3& p)
    : x(p.x)
    , y(p.y)
    , z(p.z)
{
    /* empty */
} /* Copy constructor with Point3 */

/**
 * Copy constructor with Vec4
 * @param v - A 4D vector to convert to a 3D vector
 */
Vec3::Vec3(const Vec4& v)
    : x(v.x)
    , y(v.y)
    , z(v.z)
{
    /* empty */
} /* Copy constructor with Vec4 */

/**
 * Returns the length of the vector
 * @return - The length of the vector
 */
float Vec3::Length() const
{
    return sqrt((x * x) + (y * y) + (z * z));
} /* Length() */

/**
 * Returns the squared length of the vector
 * @return - The squared length of the vector
 */
float Vec3::Length2() const
{
    return (x * x) + (y * y) + (z * z);
} /* Length2() */

/**
 * Returns the negated vector
 * @return - The negated vector
 */
Vec3 Vec3::Negate() const
{
    return Vec3(-x, -y, -z);
} /* Negate() */

/**
 * Returns the normalized vector
 * @return - The normalized vector
 */
Vec3 Vec3::Normalize() const
{
    float length = Length();
    return Vec3((x / length), (y / length), (z / length));
} /* Normalize() */

/**
 * Returns the sum of this plus another Vec3
 * @param v - A 3D vector to sum with this vector
 * @return - The sum of this plus another Vec3
 */
Vec3 Vec3::operator+(const Vec3& v) const
{
    return Vec3((x + v.x), (y + v.y), (z + v.z));
} /* operator+() */

/**
 * Returns the difference of this minus another Vec3
 * @param v - A 3D vector to subtract from this vector
 * @return - The difference of this minus another Vec3
 */
Vec3 Vec3::operator-(const Vec3& v) const
{
    return Vec3((x - v.x), (y - v.y), (z - v.z));
} /* operator-() */

/**
 * Returns the product of this times a scalar
 * @param c - A scalar to multiply with this vector
 * @return - The product of this times a scalar
 */
Vec3 Vec3::operator*(float c) const
{
    return Vec3((c * x), (c * y), (c * z));
} /* operator*() */

/**
 * Returns the quotient of this divided by a scalar
 * @param c - A scalar to divide into this vector
 * @return - The quotient of this divided by a scalar
 */
Vec3 Vec3::operator/(float c) const
{
    return Vec3((x / c), (y / c), (z / c));
} /* operator/() */

/**
 * Assigns another Vec3's values to this vector and returns this vector
 * @param v - A 3D vector to copy into this vector
 * @return - A constant reference to this vector
 */
const Vec3& Vec3::operator=(const Vec3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this; /* enables cascading assignments */
} /* operator=() */

/**
 * Prints this vector's values
 * @param f - A file object to print to
 */
void Vec3::Print(FILE* f) const
{
    fprintf(f,
        "Vec3:\n"
        " x = %f\n"
        " y = %f\n"
        " z = %f\n"
        , x, y, z
    );
} /* Print() */
