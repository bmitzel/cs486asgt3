/* Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Camera.h
 *
 * ...
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Point3.h"
#include "Vec3.h"

class Camera
{
public:
    /* Public data members */
    Point3 eyePosition;
    Point3 refPoint;
    Vec3 upVector;

    /* Default constructor */
    Camera(float eyeX, float eyeY, float eyeZ,
           float refX, float refY, float refZ,
           float upX , float upY , float upZ );
}; /* Camera class */

#endif /* CAMERA_H_ */
