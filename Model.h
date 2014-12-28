/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Model.h
 *
 * A C++ module implementing a 3D model class to represent
 * objects drawn on a display.
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <cmath>
#include <cstdlib>
#include <stdint.h>
#include <sys/time.h>

#include "AxisAlignedBoundingBox.h"
#include "PlyModel.h"
#include "Ray.h"
#include "Vec3.h"
#include "VecMath.h"

#define EPSILON 0.00001

class Model
{
public:
    /* Overloaded constructor */
    Model(const char* filename, const Point3& pos);

    /* Destructor */
    ~Model();

    /* Member functions */
    void Update();
    AxisAlignedBoundingBox* GetBoundingBox();
    float GetRotation() const;
    double GetScaleFactor() const;
    double GetScaledRadius() const;
    bool GetIsDrawingBoundingBox() const;
    void SetIsDrawingBoundingBox(bool flag);
    void ToggleDrawingBoundingBox();
    bool Intersects(const Ray& ray) const;
    FaceList* GetFaceList() const;

private:
    /* Private data members */
    FaceList* faceList; /* contains center and radius of bounding sphere */
    AxisAlignedBoundingBox boundingBox;
    float rotation;             /* the model's current rotation in degrees */
    float rotationSpeed;        /* the number of degrees the model rotates per second */
    double startingHeight;      /* the model's starting position's y component */
    double randomDegrees;       /* the random offset to use in calculating rotation */
    double randomRadians;       /* the random offset to use in calculating translation */
    double translationSpeed;    /* the multiplier for the model's translation during update */
    double scaleFactor;         /* the factor by which to scale the model */
    double scaledRadius;        /* the bounding sphere's radius after scaling */
    bool isDrawingBoundingBox;

    /* Private helper functions */
    double GetElapsedTime(); /* from Professor Shafae */
}; /* Model class */

#endif /* MODEL_H_ */
