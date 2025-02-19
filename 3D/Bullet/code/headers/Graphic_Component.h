/**********************************************************************
*Project           : Bullet3D Practice
*
*Author : Lucas Garc�a
*
*
*Purpose : Physics Practice using Bullet that moves a tank and other features
*
**********************************************************************/

/**
* \class Graphic_Component
* \brief Whatever "object" that is in scene and has a graphic and a physic component
*
* Adds a graphic component for visualization and a physic component for having collisions in the scene
*/

#pragma once

#include <memory>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <Render_Node.hpp>

using namespace std;
using namespace glt;

class Graphic_Component {
private:
    std::shared_ptr< Node > model;
public:

    Graphic_Component(std::shared_ptr< Node > newModel) : model(newModel) {};

    Node* getModel()
    {
        return model.get();
    }

};