/**********************************************************************
*Project           : Bullet3D Practice
*
*Author : Lucas García
*
*
*Purpose : Physics Practice using Bullet that moves a tank and other features
*
**********************************************************************/

#include <iostream>

#include "Entity.h"
#include <glm/gtc/type_ptr.hpp>
#include <Model.hpp>
#include <Cube.hpp>
#include "Entity.h"
#include <OpenGL.hpp>
#include <Model_Obj.hpp>
#include <Render_Node.hpp>
#include "Physics_Component.h"

using namespace std;
using namespace glt;

btRigidBody* Entity::getBody() const {
    if (physicsComponent) {
        return physicsComponent->getRigidbody();
    }
    else {
        std::cerr << "Error: Physics component is null." << std::endl;
        return nullptr;
    }
}

Node* Entity::get_Graphic_Model()
{
    return graphicComponent->getModel();
}

void Entity::addGraphicComponent(std::shared_ptr <Graphic_Component> newGraphicComponent)
{
    graphicComponent = newGraphicComponent;
}

void Entity::addPhysicsComponent(std::shared_ptr <Physics_Component> newPhysicsComponent)
{
    physicsComponent = newPhysicsComponent;
}

void Entity::setActive(bool state)
{

    if (state)
    {
        get_Graphic_Model()->set_visible(true);
        active = state;
    }
    else
    {
        get_Graphic_Model()->set_visible(false);
        active = state;
    }


}

glm::mat4 Entity::getTransform() const 
{
    return physicsComponent->getTransform();
}