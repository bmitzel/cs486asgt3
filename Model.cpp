/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Model.cpp
 *
 * A C++ module implementing a 3D model class to represent
 * objects drawn on a display.
 */

#include "Model.h"

/**
 * Default constructor
 * @param filename - The filename of a PLY model to load
 * @param pos - The position of the center of the model in world space
 */
Model::Model(const char* filename, const Point3& pos)
    : rotationSpeed(130.0f)
    , translationSpeed(2.5f)
    , isDrawingBoundingBox(false)
{
    /* Load the PLY model and initialize its position */
    faceList = readPlyModel(filename);
    faceList->center[0] = static_cast<double>(pos.x);
    faceList->center[1] = static_cast<double>(pos.y);
    faceList->center[2] = static_cast<double>(pos.z);

    /* Set the model scaling */
    scaleFactor = 0.5 / faceList->radius;
    scaledRadius = scaleFactor * faceList->radius;

    /* Initialize the starting height and rotation of the model */
    startingHeight = static_cast<double>(pos.y);
    randomDegrees = static_cast<double>(rand() % 360);
    fprintf(stderr, "randomDegrees = %f\n", randomDegrees);
    rotation = randomDegrees;
    randomRadians = randomDegrees * (2.0 * M_PI) / 360.0;
} /* Default constructor */

/**
 * Destructor
 */
Model::~Model()
{
    delete faceList;
} /* Destructor */

/**
 * Updates the model's transformation
 */
void Model::Update()
{
    float elapsedTime = static_cast<float>(GetElapsedTime());

    /* Rotate the model */
    rotation = randomDegrees + elapsedTime * rotationSpeed;

    /* Translate the model */
    faceList->center[1] =
            startingHeight + (0.4 * sin(translationSpeed * (elapsedTime + randomRadians)));
} /* Model::Update() */

/**
 * Returns the model's bounding box
 * @return - The model's bounding box
 */
AxisAlignedBoundingBox* Model::GetBoundingBox()
{
    return &boundingBox;
} /* Model::GetBoundingBox() */

/**
 * Returns the model's rotation
 * return - The model's rotation
 */
float Model::GetRotation() const
{
    return rotation;
} /* Model::GetRotation() */

/**
 * Returns the model's scaling factor
 * @return - The model's scaling factor
 */
double Model::GetScaleFactor() const
{
    return scaleFactor;
} /* Model::GetScaleFactor() */

/**
 * Returns the model's scaled radius
 * @return - The model's scaled radius
 */
double Model::GetScaledRadius() const
{
    return scaledRadius;
} /* Model::GetScaledRadius() */

/**
 * Returns true if the model should draw its bounding box
 * @return - True if the model should draw its bounding box
 */
bool Model::GetIsDrawingBoundingBox() const
{
    return isDrawingBoundingBox;
} /* Model::GetIsDrawingBoundingBox() */

/**
 * Sets whether the model should draw its bounding box
 * @param flag - True if the model should draw its bounding box
 */
void Model::SetIsDrawingBoundingBox(bool flag)
{
    isDrawingBoundingBox = flag;
} /* Model::SetIsDrawingBoundingBox() */

/**
 * Toggles whether the model should draw its bounding box
 */
void Model::ToggleDrawingBoundingBox()
{
    isDrawingBoundingBox = !isDrawingBoundingBox;
} /* Model::ToggleDrawingBoundingBox() */

/**
 * Tests if a ray intersects the model's bounding volume
 * @param ray - The ray with which to test for intersection
 * @return - True if the ray intersects the model's bounding volume;
 * otherwise, false
 */
bool Model::Intersects(const Ray& ray) const
{
    /* Slabs method of ray/AABB intersect from Real-Time Rendering, 3rd edition */
    Point3 center = boundingBox.GetCenter();
    float tMin = -999999999.0;
    float tMax = +999999999.0;
    Vec3 v = center - ray.origin;
    float p[] = {v.x, v.y, v.z};
    float d[] = {ray.direction.x, ray.direction.y, ray.direction.z};
    float halfLengths[] = {(boundingBox.right - boundingBox.left) / 2,
            (boundingBox.top - boundingBox.bottom) / 2, (boundingBox.front - boundingBox.back) / 2};

    for (int i = 0; i < 3; i++)
    {
        float e = p[i];
        float f = d[i];

        if (fabs(f) > EPSILON)
        {
            float fInverse = 1 / f;
            float t1 = (e + halfLengths[i]) * fInverse;
            float t2 = (e - halfLengths[i]) * fInverse;

            if (t1 > t2)
            {
                std::swap(t1, t2);
            }

            if (t1 > tMin)
            {
                tMin = t1;
            }

            if (t2 < tMax)
            {
                tMax = t2;
            }

            if (tMin > tMax || tMax < 0)
            {
                return false;
            }
        }
        else if (-e - halfLengths[i] > 0 || -e + halfLengths[i] < 0)
        {
            return false;
        }

    }

    return true;
} /* Model::Intersects() */

/**
 * Returns the model's list of faces
 * @return - The model's list of faces
 */
FaceList* Model::GetFaceList() const
{
    return faceList;
} /* Model::GetFaceList() */

/**
 * Calculates the elapsed time since the function is first called
 * from Professor Shafae
 * @return - The elapsed time since the function is first called
 */
double Model::GetElapsedTime()
{
    static bool     isFirstTime = true; /* If this is the first function call */
    static struct   timeval start;      /* The static start time */
    struct          timeval now;        /* The current time */
    struct          timeval elapsed;    /* The elapsed time (now - start) */

    /* Save the start time if this is the first function call */
    if (isFirstTime)
    {
        isFirstTime = false;
        gettimeofday(&start, NULL);
    }

    /* Get the current time, and calculate the elapsed time */
    gettimeofday(&now, NULL);
    elapsed.tv_sec = now.tv_sec - start.tv_sec;
    elapsed.tv_usec = now.tv_usec - start.tv_usec;
    if (elapsed.tv_usec < 0)
    {
        --elapsed.tv_sec;
        elapsed.tv_usec += 1000000;
    }

    /* Return the elapsed time in seconds */
    return (static_cast<uint64_t>(elapsed.tv_sec)
           * static_cast<uint64_t>(1000000)
           + static_cast<uint64_t>(elapsed.tv_usec))
           / 1000000.0;
} /* Model::GetElapsedTime() */
