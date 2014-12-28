/* Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Camera.cpp
 *
 * A C++ module implementing a 3D camera class which is
 * defined by an eye position, a reference (lookat) point,
 * and an up vector.
 */

#include "Camera.h"

/**
 * Default constructor
 * @param eyeX - Component x of the eye position
 * @param eyeY - Component y of the eye position
 * @param eyeZ - Component z of the eye position
 * @param refX - Component x of the reference point
 * @param refY - Component y of the reference point
 * @param refZ - Component z of the reference point
 * @param upX - Component x of the up vector
 * @param upY - Component y of the up vector
 * @param upZ - Component z of the up vector
 */
Camera::Camera(float eyeX, float eyeY, float eyeZ,
               float refX, float refY, float refZ,
               float upX , float upY , float upZ )
    : eyePosition(eyeX, eyeY, eyeZ)
    , refPoint(refX, refY, refZ)
    , upVector(upX, upY, upZ)
{
    upVector = upVector.Normalize();
}/* Default constructor */

/**
 * Rotates the camera by a given quaternion
 * @param rotation - The quaternion representing the axis and angle of rotation
 */
void Camera::Rotate(Quaternion rotation)
{
    /* Get the gaze vector and rotate it */
    Vec3 gazeVector = (rotation * GetGazeVector()).Normalize();

    /* Update the camera's reference point using the new gaze vector */
    refPoint = eyePosition + gazeVector;

    /* Get the right vector and rotate it with zero roll (zero y component) */
    Vec3 rightVector = (rotation * GetRightVector());
    rightVector.y = 0.0f;
    rightVector = rightVector.Normalize();

    /* Calculate the new up vector */
    upVector = cross(rightVector, gazeVector).Normalize();
} /* Camera::Rotate() */

/**
 * Returns the camera's gaze vector
 * @return - The camera's gaze vector
 */
Vec3 Camera::GetGazeVector() const
{
    return Vec3(refPoint.x - eyePosition.x, refPoint.y - eyePosition.y,
            refPoint.z - eyePosition.z).Normalize();
} /* Camera::GetGazeVector() */

/**
 * Returns the camera's right vector
 * @return - The camera's right vector
 */
Vec3 Camera::GetRightVector() const
{
    return cross(GetGazeVector(), upVector).Normalize();
} /* Camera::GetRightVector() */
