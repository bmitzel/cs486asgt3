/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: AxisAlignedBoundingBox.cpp
 *
 * A C++ module implementing a 3D axis-aligned bounding box
 */

#include "AxisAlignedBoundingBox.h"

/**
 * Default constructor initializes the data members to zero
 */
AxisAlignedBoundingBox::AxisAlignedBoundingBox()
    : left(0.0f)
    , right(0.0f)
    , top(0.0f)
    , bottom(0.0f)
    , front(0.0f)
    , back(0.0f)
{
    /* empty */
} /* Default constructor */

/**
 * Calculates and returns the center point of the box
 * @return - The center point of the box
 */
Point3 AxisAlignedBoundingBox::GetCenter() const
{
    float center[] = {(left + right) / 2, (bottom + top) / 2, (back + front) / 2, 1.0f};
    float modelviewInverse[16];

    /* Invert the modelview matrix */
    if (gluInvertMatrix(modelview, modelviewInverse))
    {
        /* Undo the camera transformation matrix but keep the model's rotation, scale, and
         * translation transformation matrix in order to calculate the bounding box's center
         * in model space and get its proper location
         */
        matMultVec4f(center, center, modelviewInverse);
        matMultVec4f(center, center, transform);
    }
    else
    {
        /* The modelview matrix is not invertible, so just return garbage */
        center[0] = 999999999.0f;
        center[1] = 999999999.0f;
        center[2] = 999999999.0f;
    }

    return Point3(center[0], center[1], center[2]);
} /* AxisAlignedBoundingBox::GetCenter() */

/**
 * Recalculates the axis-aligned bounding box based on the model's current modelview matrix
 * @param faceList - The model's face list
 * @param modelview - The model's modelview matrix
 */
void AxisAlignedBoundingBox::Recalculate(FaceList* faceList, float mv[], float tform[])
{
    /* Save the modelview matrix */
    for (int i =0; i < 16; i++)
    {
        modelview[i] = mv[i];
    }

    /* Save the model's transform (rotate, scale, translate) matrix */
    for (int i =0; i < 16; i++)
    {
        transform[i] = tform[i];
    }

    /* Start with the first vertex */
    bool isFirstVertex = true;

    /* Iterate through all the vertices in the face list */
    for (int i = 0; i < faceList->vc; i++)
    {
        float point[4];
        float vector[4];

        /* Initialize a 4D point to the current 3D vertex */
        for (int j = 0; j < 3; j++)
        {
            point[j] = faceList->vertices[i][j];
        }

        point[3] = 1.0f;

        /* Apply the model's transform matrix and store the result in a 4D vector */
        matMultVec4f(vector, point, modelview);

        /* Initialize the bounding box values using the first vertex's values */
        if (isFirstVertex)
        {
            left = right = vector[0]; // x component
            top = bottom = vector[1]; // y component
            front = back = vector[2]; // z component
            isFirstVertex = false;
        }
        /* Compare the other vertices' values to the bounding box's values
         * to find the min and max along each axis
         */
        else
        {
            if (vector[0] < left)
            {
                left = vector[0];
            }
            else if (vector[0] > right)
            {
                right = vector[0];
            }

            if (vector[1] < bottom)
            {
                bottom = vector[1];
            }
            else if (vector[1] > top)
            {
                top = vector[1];
            }

            if (vector[2] < back)
            {
                back = vector[2];
            }
            else if (vector[2] > front)
            {
                front = vector[2];
            }
        }
    }
} /* AxisAlignedBoundingBox::Recalculate() */
