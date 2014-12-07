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
    , isDrawingBoundingSphere(false)
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
 * Returns true if the model should draw its bounding sphere
 * @return - True if the model should draw its bounding sphere
 */
bool Model::GetIsDrawingBoundingSphere() const
{
    return isDrawingBoundingSphere;
} /* Model::GetIsDrawingBoundingSphere() */

/**
 * Sets whether the model should draw its bounding sphere
 * @param flag - True if the model should draw its bounding sphere
 */
void Model::SetIsDrawingBoundingSphere(bool flag)
{
    isDrawingBoundingSphere = flag;
} /* Model::SetIsDrawingBoundingSphere() */

/**
 * Toggles whether the model should draw its bounding sphere
 */
void Model::ToggleDrawingBoundingSphere()
{
    isDrawingBoundingSphere = !isDrawingBoundingSphere;
} /* Model::ToggleDrawingBoundingSphere() */

/**
 * Tests if a ray intersects the model's bounding volume
 * @param ray - The ray with which to test for intersection
 * @return - True if the ray intersects the model's bounding volume
 */
bool Model::Intersects(const Ray& ray) const
{
    /* If the ray's origin is inside the bounding sphere, it intersects */
    Vec3 l = Point3(faceList->center[0], faceList->center[1], faceList->center[2]) - ray.origin;
    float lSquared = l.Length2();
    float rSquared = scaledRadius * scaledRadius;
    if (lSquared < rSquared)
    {
        return true;
    }

    /* Find the scalar projection of l onto the ray's direction */
    float s = dot(l, ray.direction);

    /* If s < 0, the ray points away from the sphere */
    if (s < 0.0f)
    {
        return false;
    }

    /* Find mSquared using the Pythagorean Theorem and check for intersection */
    float sSquared = s * s;
    float mSquared = lSquared - sSquared;
    if (mSquared > rSquared)
    {
        return false;
    }

    /* The ray intersects */
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
