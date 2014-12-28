/* Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Trackball.h
 *
 * A C++ module implementing a virtual trackball for
 * calculating a rotation quaternion based on two input
 * window coordinates which, when projected onto the
 * trackball's surface, define the beginning and end
 * of the trackball's path.
 */

#ifndef TRACKBALL_H_
#define TRACKBALL_H_

#define _USE_MATH_DEFINES
#include <cmath>

#include "Point3.h"
#include "Quaternion.h"
#include "Vec3.h"
#include "VecMath.h"

enum State {OFF, ON};

class Trackball
{
public:
	/* Default constructor */
	Trackball();

	/* Member functions */
	const Point3& GetPoint1() const;
	const Point3& GetPoint2() const;
	int GetState() const;
	void SetCenter(int x, int y);
	void SetPoint1(float x, float y);
	void SetPoint1(Point3 p);
    void SetPoint2(float x, float y);
    void SetRadius(float r);
    void SetState(State status);
	Quaternion GetRotation();

private:
	/* Private data members */
	int centerX;            /* the x component of the trackball's center */
	int centerY;            /* the y component of the trackball's center */
    Point3 point1;          /* the first point used in calculating the trackball's rotation */
    Point3 point2;          /* the second point used in calculating the trackball's rotation */
    float radius;           /* the trackball's radius */
	int state;              /* the trackball's on/off state */

	/* Private helper functions */
	Point3 ProjectToSphere(float x, float y) const;
}; /* Trackball class */

#endif /* TRACKBALL_H_ */
