/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: VectorMath.cpp
 *
 * A C++ module implementing helper functions for 3D vector
 * math operations.
 */

#include "VecMath.h"

/**
 * Returns the dot product of two 3D vectors
 * @param a - The first 3D vector to multiply
 * @param b - The second 3D vector to multiply
 * @return - The dot product of the two 3D vectors
 */
float dot(const Vec3& a, const Vec3& b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
} /* dot() */

/**
 * Returns the cross product of two 3D vectors
 * @param a - The first 3D vector to cross
 * @param b - The second 3D vector to cross
 * @return - The cross product of the two 3D vectors
 */
Vec3 cross(const Vec3& a, const Vec3& b)
{
    return Vec3(
            ((a.y * b.z) - (a.z * b.y)),
            ((a.z * b.x) - (a.x * b.z)),
            ((a.x * b.y) - (a.y * b.x))
               );
} /* cross() */
