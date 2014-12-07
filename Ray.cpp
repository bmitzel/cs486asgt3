/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Ray.cpp
 *
 * A C++ module implementing a 3D ray class using a 3D point
 * as the origin and a normalized 3D vector as the direction
 */

#include "Ray.h"

/**
 * Default constructor
 * @param near - A 3D point defining the origin of the ray
 * @param far - Another 3D point on the ray for calculating its direction
 */
Ray::Ray(const Point3& near, const Point3& far)
    : origin(near.x, near.y, near.z)
    , direction(far.x - near.x, far.y - near.y, far.z - near.z)
{
    /* Normalize the ray's direction vector */
    direction = direction.Normalize();
}
