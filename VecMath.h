/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: VectorMath.h
 *
 * A C++ module implementing helper functions for 3D vector
 * math operations.
 */

#ifndef VECMATH_H_
#define VECMATH_H_

#include "Vec3.h"

float dot(const Vec3& a, const Vec3& b);
Vec3 cross(const Vec3& a, const Vec3& b);

#endif /* VECMATH_H_ */
