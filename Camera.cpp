/* Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Camera.cpp
 *
 * ...
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
    /* empty */
}/* Default constructor */
