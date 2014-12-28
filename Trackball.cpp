/* Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Trackball.cpp
 *
 * A C++ module implementing a virtual trackball for
 * calculating a rotation quaternion based on two input
 * window coordinates which, when projected onto the
 * trackball's surface, define the beginning and end
 * of the trackball's path.
 */

#include "Trackball.h"

/**
 * Default constructor initializes the trackball with zero rotation
 */
Trackball::Trackball()
    : centerX(0)
    , centerY(0)
    , radius(0.0f)
    , state(OFF)
{
    /* empty */
} /* Default constructor */

/**
 * Gets the value stored in point1
 * @return - The value stored in point1
 */
const Point3& Trackball::GetPoint1() const
{
    return point1;
} /* Trackball::GetPoint1() */

/**
 * Gets the value stored in point2
 * @return - The value stored in point2
 */
const Point3& Trackball::GetPoint2() const
{
    return point2;
} /* Trackball::GetPoint2() */

/**
 * Returns the current on/off state of the trackball
 */
int Trackball::GetState() const
{
    return state;
} /* Trackball::GetState() */

/**
 * Sets the center coordinate of the trackball
 */
void Trackball::SetCenter(int x, int y)
{
    centerX = x;
    centerY = y;
} /* Trackball::SetCenter() */

/**
 * Sets the first point used in calculating the trackball's rotation
 * Projects the passed x and y coordinate components onto the virtual trackball
 * @param x - The x component of the point
 * @param y - The y component of the point
 */
void Trackball::SetPoint1(float x, float y)
{
    point1 = ProjectToSphere(x, y);
} /* Trackball::SetPoint1() */

/**
 * Sets the first point used in calculating the trackball's rotation
 * Sets point1 equal to the passed point p
 * @param p - The point to set equal to point1
 */
void Trackball::SetPoint1(Point3 p)
{
    point1 = p;
} /* Trackball::SetPoint1() */

/**
 * Sets the second point used in calculating updates to the trackball's rotation
 * @param x - The x component of the point
 * @param y - The y component of the point
 */
void Trackball::SetPoint2(float x, float y)
{
    point2 = ProjectToSphere(x, y);
} /* Trackball::SetPoint2() */

/**
 * Sets the trackball's radius
 * @param r - The new radius value to set
 */
void Trackball::SetRadius(float r)
{
    radius = r;
} /* Trackball::SetRadius() */

/**
 * Sets the trackball's state to ON or OFF
 * @param status - The state to set the trackball to
 */
void Trackball::SetState(State status)
{
    state = status;
} /* Trackball::SetState() */

/**
 * Calculates the trackball's rotation by using vectors to point1 and point2
 * The cross product of the vectors gives us the axis of rotation
 * The dot product of the vectors gives us the angle of rotation
 * @return - The trackball's rotation
 */
Quaternion Trackball::GetRotation()
{
    /* Check for unequal starting and ending points */
    if (point1.x != point2.x || point1.y != point2.y)
    {
        /* Calculate the axis and distance of the trackball's rotation */
        Vec3 a = Vec3(point1).Normalize();
        Vec3 b = Vec3(point2).Normalize();
        Vec3 axis = cross(a, b).Normalize();
        Vec3 difference = point2 - point1;
        float distance = difference.Length() / (2.0f * radius);

        /* Clamp the distance to values between 0.0 and 1.0 */
        if (distance > 1.0f)
        {
            distance = 1.0f;
        }

        /* Calculate the angle of rotation */
        float angle = 2.0f * asin(distance);

        /* Add the new axis and angle to the trackball's rotation */
        return Quaternion(axis, angle);
    }
    else
    {
        return Quaternion(); // zero rotation
    }
} /* Trackball::UpdateRotation() */

/**
 * Projects window coordinates onto the trackball
 * @return - The projected point on the trackball
 */
Point3 Trackball::ProjectToSphere(float x, float y) const
{
    /* Adjust the x and y components to be in relation to the center of the trackball */
    x -= centerX;
    y -= centerY;

    /* The z component of the projected coordinates */
    float z;

    /* The distance from the center of the trackball to the window coordinates */
    float d = sqrtf((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));

    /* Constant for determining whether to project to sphere or hyperbolic sheet */
    float t = radius * M_SQRT1_2;

    /* Tracking on the sphere */
    if (d < t)
    {
        z = -sqrtf(radius * radius - d * d);
    }
    /* Tracking outside the sphere on the hyperbolic sheet
     * i.e. near the corners of the graphical window
     */
    else
    {
        z = -(t * t / d);
    }

    return Point3(x, y, z);
} /* Trackball::ProjectToSphere() */
