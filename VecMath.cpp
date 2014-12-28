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

/**
 * Inverts a 4x4 matrix
 * source code from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
 * @param m - The matrix to be inverted
 * @param invOut - The inverted matrix to be returned
 * @return - True if the matrix has an inverse;
 * otherwise, false
 */
bool gluInvertMatrix(const float m[16], float invOut[16])
{
    float inv[16], det;
    int i;

    inv[0]  =  m[5]  * m[10] * m[15] -
               m[5]  * m[11] * m[14] -
               m[9]  * m[6]  * m[15] +
               m[9]  * m[7]  * m[14] +
               m[13] * m[6]  * m[11] -
               m[13] * m[7]  * m[10];

    inv[4]  = -m[4]  * m[10] * m[15] +
               m[4]  * m[11] * m[14] +
               m[8]  * m[6]  * m[15] -
               m[8]  * m[7]  * m[14] -
               m[12] * m[6]  * m[11] +
               m[12] * m[7]  * m[10];

    inv[8]  =  m[4]  * m[9]  * m[15] -
               m[4]  * m[11] * m[13] -
               m[8]  * m[5]  * m[15] +
               m[8]  * m[7]  * m[13] +
               m[12] * m[5]  * m[11] -
               m[12] * m[7]  * m[9];

    inv[12] = -m[4]  * m[9]  * m[14] +
               m[4]  * m[10] * m[13] +
               m[8]  * m[5]  * m[14] -
               m[8]  * m[6]  * m[13] -
               m[12] * m[5]  * m[10] +
               m[12] * m[6]  * m[9];

    inv[1]  = -m[1]  * m[10] * m[15] +
               m[1]  * m[11] * m[14] +
               m[9]  * m[2]  * m[15] -
               m[9]  * m[3]  * m[14] -
               m[13] * m[2]  * m[11] +
               m[13] * m[3]  * m[10];

    inv[5]  =  m[0]  * m[10] * m[15] -
               m[0]  * m[11] * m[14] -
               m[8]  * m[2]  * m[15] +
               m[8]  * m[3]  * m[14] +
               m[12] * m[2]  * m[11] -
               m[12] * m[3]  * m[10];

    inv[9]  = -m[0]  * m[9]  * m[15] +
               m[0]  * m[11] * m[13] +
               m[8]  * m[1]  * m[15] -
               m[8]  * m[3]  * m[13] -
               m[12] * m[1]  * m[11] +
               m[12] * m[3]  * m[9];

    inv[13] =  m[0]  * m[9]  * m[14] -
               m[0]  * m[10] * m[13] -
               m[8]  * m[1]  * m[14] +
               m[8]  * m[2]  * m[13] +
               m[12] * m[1]  * m[10] -
               m[12] * m[2]  * m[9];

    inv[2]  =  m[1]  * m[6]  * m[15] -
               m[1]  * m[7]  * m[14] -
               m[5]  * m[2]  * m[15] +
               m[5]  * m[3]  * m[14] +
               m[13] * m[2]  * m[7] -
               m[13] * m[3]  * m[6];

    inv[6]  = -m[0]  * m[6]  * m[15] +
               m[0]  * m[7]  * m[14] +
               m[4]  * m[2]  * m[15] -
               m[4]  * m[3]  * m[14] -
               m[12] * m[2]  * m[7] +
               m[12] * m[3]  * m[6];

    inv[10] =  m[0]  * m[5]  * m[15] -
               m[0]  * m[7]  * m[13] -
               m[4]  * m[1]  * m[15] +
               m[4]  * m[3]  * m[13] +
               m[12] * m[1]  * m[7] -
               m[12] * m[3]  * m[5];

    inv[14] = -m[0]  * m[5]  * m[14] +
               m[0]  * m[6]  * m[13] +
               m[4]  * m[1]  * m[14] -
               m[4]  * m[2]  * m[13] -
               m[12] * m[1]  * m[6] +
               m[12] * m[2]  * m[5];

    inv[3]  = -m[1]  * m[6]  * m[11] +
               m[1]  * m[7]  * m[10] +
               m[5]  * m[2]  * m[11] -
               m[5]  * m[3]  * m[10] -
               m[9]  * m[2]  * m[7] +
               m[9]  * m[3]  * m[6];

    inv[7]  =  m[0]  * m[6]  * m[11] -
               m[0]  * m[7]  * m[10] -
               m[4]  * m[2]  * m[11] +
               m[4]  * m[3]  * m[10] +
               m[8]  * m[2]  * m[7] -
               m[8]  * m[3]  * m[6];

    inv[11] = -m[0]  * m[5]  * m[11] +
               m[0]  * m[7]  * m[9] +
               m[4]  * m[1]  * m[11] -
               m[4]  * m[3]  * m[9] -
               m[8]  * m[1]  * m[7] +
               m[8]  * m[3]  * m[5];

    inv[15] =  m[0]  * m[5]  * m[10] -
               m[0]  * m[6]  * m[9] -
               m[4]  * m[1]  * m[10] +
               m[4]  * m[2]  * m[9] +
               m[8]  * m[1]  * m[6] -
               m[8]  * m[2]  * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (fabs(det) < EPSILON)
    {
        return false;
    }

    det = 1.0f / det;

    for (i = 0; i < 16; i++)
    {
        invOut[i] = inv[i] * det;
    }

    return true;
} /* gluInvertMatrix() */

/**
 * Copies a 4x4 matrix of floats into another 4x4 matrix of floats
 * from Professor Shafae
 * @param dest - The destination matrix to copy to
 * @param src - The source matrix to copy from
 */
void vecCopy4f(float *dest, const float *src)
{
    for(int i = 0; i < 4; i++)
    {
        dest[i] = src[i];
    }
} /* vecCopy4f() */

/**
 * Multiplies a 4D vector by a 4x4 matrix of floats for performing transformations
 * from Professor Shafae
 * @param vout - The returned vector where the result of the computation is stored
 * @param v - The input vector to be multiplied
 * @param m - The input matrix to multiply by
 */
void matMultVec4f(float vout[4], const float v[4], const float m[16])
{
    float c[4];
    vecCopy4f(c, v);
    vout[0] = m[0] * c[0] + m[4] * c[1] + m[8]  * c[2] + m[12] * c[3];
    vout[1] = m[1] * c[0] + m[5] * c[1] + m[9]  * c[2] + m[13] * c[3];
    vout[2] = m[2] * c[0] + m[6] * c[1] + m[10] * c[2] + m[14] * c[3];
    vout[3] = m[3] * c[0] + m[7] * c[1] + m[11] * c[2] + m[15] * c[3];
} /* matMultVec4f() */

/**
 * Multiplies a 4x4 matrix by another 4x4 matrix of floats; C = A * B
 * @param C - The returned matrix where the result of the computation is stored
 * @param A - The matrix to multiply by
 * @param B - The matrix to be multiplied
 */
void matMultMat4f(float C[16], const float A[16], const float B[16])
{
    /* Back up matrix A from being overwritten, in case arg C = arg A */
    float A00 = A[0];
    float A10 = A[1];
    float A20 = A[2];
    float A30 = A[3];
    float A01 = A[4];
    float A11 = A[5];
    float A21 = A[6];
    float A31 = A[7];
    float A02 = A[8];
    float A12 = A[9];
    float A22 = A[10];
    float A32 = A[11];
    float A03 = A[12];
    float A13 = A[13];
    float A23 = A[14];
    float A33 = A[15];

    /* Back up matrix B from being overwritten, in case arg C = arg B */
    float B00 = B[0];
    float B10 = B[1];
    float B20 = B[2];
    float B30 = B[3];
    float B01 = B[4];
    float B11 = B[5];
    float B21 = B[6];
    float B31 = B[7];
    float B02 = B[8];
    float B12 = B[9];
    float B22 = B[10];
    float B32 = B[11];
    float B03 = B[12];
    float B13 = B[13];
    float B23 = B[14];
    float B33 = B[15];

    /* C = A * B */
    C[0]  = A00 * B00 + A01 * B10 + A02 * B20 + A03 * B30;
    C[1]  = A10 * B00 + A11 * B10 + A12 * B20 + A13 * B30;
    C[2]  = A20 * B00 + A21 * B10 + A22 * B20 + A23 * B30;
    C[3]  = A30 * B00 + A31 * B10 + A32 * B20 + A33 * B30;
    C[4]  = A00 * B01 + A01 * B11 + A02 * B21 + A03 * B31;
    C[5]  = A10 * B01 + A11 * B11 + A12 * B21 + A13 * B31;
    C[6]  = A20 * B01 + A21 * B11 + A22 * B21 + A23 * B31;
    C[7]  = A30 * B01 + A31 * B11 + A32 * B21 + A33 * B31;
    C[8]  = A00 * B02 + A01 * B12 + A02 * B22 + A03 * B32;
    C[9]  = A10 * B02 + A11 * B12 + A12 * B22 + A13 * B32;
    C[10] = A20 * B02 + A21 * B12 + A22 * B22 + A23 * B32;
    C[11] = A30 * B02 + A31 * B12 + A32 * B22 + A33 * B32;
    C[12] = A00 * B03 + A01 * B13 + A02 * B23 + A03 * B33;
    C[13] = A10 * B03 + A11 * B13 + A12 * B23 + A13 * B33;
    C[14] = A20 * B03 + A21 * B13 + A22 * B23 + A23 * B33;
    C[15] = A30 * B03 + A31 * B13 + A32 * B23 + A33 * B33;
} /* matMultMat4f() */
