/* Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Quaternion.cpp
 *
 * A C++ module implementing a quaternion object
 */

#include "Quaternion.h"

/**
 * Default constructor initializes the quaternion with zero rotation
 */
Quaternion::Quaternion()
    : axis(0.0f, 0.0f, 0.0f)
    , angle(1.0f)
{
    /* empty */
} /* Default constructor */

/**
 * Overloaded constructor initializes the quaternion for rotation by an angle around an axis
 */
Quaternion::Quaternion(const Vec3& w, float theta)
    : axis(w)
    , angle(theta)
{
    /* empty */
} /* Overloaded constructor */

/**
 * Overloaded constructor initializes the quaternion from a real scalar and imaginary vector
 */
Quaternion::Quaternion(float scalar, const Vec3& vector)
    : axis(vector.x / sqrt(1 - scalar * scalar), vector.y / sqrt(1 - scalar * scalar),
            vector.z / sqrt(1 - scalar * scalar))
    , angle(2 * acos(scalar))
{
    /* empty */
} /* Overloaded constructor */

/**
 * Rotates a 3D vector by the quaternion's axis and angle
 * @param rhs - The vector to rotate
 * @return - The rotated vector
 */
Vec3 Quaternion::operator*(const Vec3& rhs) const
{
    /* Set up Q-bar, Q, and v for calculating the rotation */
    float delta = sin(angle / 2);
    float gamma = cos(angle / 2);
    float Q_bar[] = { gamma         ,  delta * axis.z, -delta * axis.y, delta * axis.x
                    ,-delta * axis.z,  gamma         ,  delta * axis.x, delta * axis.y
                    , delta * axis.y, -delta * axis.x,  gamma         , delta * axis.z
                    ,-delta * axis.x, -delta * axis.y, -delta * axis.z, gamma         };
    float Q[] = { gamma         ,  delta * axis.z, -delta * axis.y, -delta * axis.x
                ,-delta * axis.z,  gamma         ,  delta * axis.x, -delta * axis.y
                , delta * axis.y, -delta * axis.x,  gamma         , -delta * axis.z
                , delta * axis.x,  delta * axis.y,  delta * axis.z,  gamma         };
    float v[] = {rhs.x, rhs.y, rhs.z, 0.0f};

    /* Calculate the rotation matrix R = Q-bar * Q, and apply the rotation to vector v */
    float R[16];
    matMultMat4f(R, Q_bar, Q);
    matMultVec4f(v, v, R);

    return Vec3(v[0], v[1], v[2]);
} /* Quaternion::operator*() */
