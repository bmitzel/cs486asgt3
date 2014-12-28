/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: AxisAlignedBoundingBox.h
 *
 * A C++ module implementing a 3D axis-aligned bounding box
 */

#ifndef AXISALIGNEDBOUNDINGBOX_H_
#define AXISALIGNEDBOUNDINGBOX_H_

#include "FaceList.h"
#include "Point3.h"
#include "VecMath.h"

class AxisAlignedBoundingBox
{
public:
    /* Public data members */
    float left;
    float right;
    float top;
    float bottom;
    float front;
    float back;

    /* Default constructor */
    AxisAlignedBoundingBox();

    /* Member functions */
    Point3 GetCenter() const;
    void Recalculate(FaceList* faceList, float modelview[], float transform[]);

private:
    /* Private data members */
    float modelview[16];
    float transform[16];
}; /* class AxisAlignedBoundingBox */

#endif /* AXISALIGNEDBOUNDINGBOX_H_ */
