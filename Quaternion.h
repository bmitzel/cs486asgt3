/* Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Quaternion.h
 *
 * A C++ module implementing a quaternion object
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <cmath>

#include "Vec3.h"
#include "VecMath.h"

class Quaternion
{
public:
    /* Default constructor */
    Quaternion();

    /* Overloaded constructors */
	Quaternion(const Vec3& axis, float angle);
	Quaternion(float scalar, const Vec3& vector);

	/* Operator overloads */
	Vec3 operator*(const Vec3& rhs) const;

private:
	/* Private data members */
	Vec3 axis;
	float angle;
}; /* Quaternion class */

#endif /* QUATERNION_H_ */
