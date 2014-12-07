/*
 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 *
 * Filename: Scene.h
 *
 * This is a C++ definition of a Scene object which
 * contains a list of 3D Models and a Camera object.
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <list>

#include "Camera.h"
#include "Model.h"

class Scene
{
public:
    /* Default constructor */
    Scene();

    /* Destructor */
    ~Scene();

    /* Member functions */
    void Insert(const char* filename, const Point3& pos);
    std::list<Model*>* GetModels();
    Camera* GetCamera();

private:
    /* Private member variables */
    std::list<Model*> models;
    Camera camera;
}; /* Scene class */

#endif /* SCENE_H_ */
