/* Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Camera.h
 *
 * A C++ module implementing a 3D camera class which is
 * defined by an eye position, a reference (lookat) point,
 * and an up vector.
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Point3.h"
#include "Quaternion.h"
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

    /* Member functions */
    void Rotate(Quaternion rotation);

private:
    /* Private helper functions */
    Vec3 GetGazeVector() const;
    Vec3 GetRightVector() const;
}; /* Camera class */

#endif /* CAMERA_H_ */
