/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Scene.cpp
 *
 * This is a C++ implementation of a Scene object which
 * contains a list of 3D Models and a Camera object.
 */

#include "Scene.h"

/**
 * Default constructor initializes the camera
 */
Scene::Scene()
    : camera(0.0f, 1.5f, 6.0f, 0.0f, 1.5f, 0.0f, 0.0f, 1.0f, 0.0f)
{
    /* empty */
} /* Default constructor */

/**
 * Destructor releases dynamically-allocated memory
 */
Scene::~Scene()
{
    while (!models.empty())
    {
        delete models.back();
        models.pop_back();
    }
} /* Destructor */

/**
 * Inserts a new model into the scene
 * @param filename - The name of the file containing a PLY model to insert
 * @param pos - The 3D position where the center of the model will be located
 */
void Scene::Insert(const char* filename, const Point3& pos)
{
    Model* newModel = new Model(filename, pos);
    models.push_back(newModel);
} /* Insert() */

/**
 * Returns the list of models contained in the scene
 * @return A constant reference to the list of models contained in the scene
 */
std::list<Model*>* Scene::GetModels()
{
    return &models;
} /* GetModels() */

/**
 * Returns the camera
 * @return A constant reference to the camera object
 */
Camera* Scene::GetCamera()
{
    return &camera;
} /* GetCamera() */
