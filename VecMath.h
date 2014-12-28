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

#define EPSILON 0.00001

float dot(const Vec3& a, const Vec3& b);
Vec3 cross(const Vec3& a, const Vec3& b);
void vecCopy4f(float *dest, const float *src); /* from Professor Shafae */
bool gluInvertMatrix(const float m[16], float invOut[16]); /* from StackOverflow.com */
void matMultVec4f(float vout[4], const float v[4], const float m[16]); /* from Professor Shafae */
void matMultMat4f(float C[16], const float A[16], const float B[16]);

#endif /* VECMATH_H_ */
