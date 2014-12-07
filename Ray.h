/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Ray.h
 *
 * A C++ module implementing a 3D ray class using a 3D point
 * as the origin and a normalized 3D vector as the direction
 */

#ifndef RAY_H_
#define RAY_H_

#include "Point3.h"
#include "Vec3.h"

class Ray
{
public:
    /* Public data members */
    Point3 origin;
    Vec3 direction;

    /* Default constructor */
    Ray(const Point3& near, const Point3& far);
}; /* Ray class */

#endif /* RAY_H_ */
