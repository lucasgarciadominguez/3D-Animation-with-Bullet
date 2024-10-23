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
#include <OpenGL.hpp>
#include <Model_Obj.hpp>
#include <Render_Node.hpp>
#include "Physics_Component.h"

using namespace std;
using namespace glt;
/**
 * Get the rigid body associated with this entity's physics component.
 * @return A pointer to the rigid body, or nullptr if the physics component is not set.
 */
btRigidBody* Entity::getBody() const {
    if (!physicsComponent) {
        std::cerr << "Error: Physics component is null." << std::endl;
        return nullptr;
    }
    return physicsComponent->getRigidbody();
}
/**
 * Get the rigid body associated with this entity's physics component.
 * @return A pointer to the rigid body, or nullptr if the physics component is not set.
 */
std::shared_ptr<btRigidBody> Entity::getSharedBody() const {
    if (!physicsComponent) {
        std::cerr << "Error: Physics component is null." << std::endl;
        return nullptr;
    }
    return physicsComponent->getSharedRigidbody();
}
/**
 * Get the graphic model associated with this entity's graphic component.
 * @return A pointer to the graphic model, or nullptr if the graphic component is not set.
 */
Node* Entity::get_Graphic_Model()
{
    if (!graphicComponent) {
        std::cerr << "Error: Graphic component is null." << std::endl;
        return nullptr;
    }
    return graphicComponent->getModel();
}

/**
 * Assign a new graphic component to the entity.
 * @param newGraphicComponent The new graphic component to assign.
 */
void Entity::addGraphicComponent(std::shared_ptr<Graphic_Component> newGraphicComponent) {
    if (!newGraphicComponent) {
        std::cerr << "Error: Attempted to add a null graphic component." << std::endl;
        return;
    }
    graphicComponent = newGraphicComponent;
}

/**
 * Assign a new physics component to the entity.
 * @param newPhysicsComponent The new physics component to assign.
 */
void Entity::addPhysicsComponent(std::shared_ptr<Physics_Component> newPhysicsComponent) {
    if (!newPhysicsComponent) {
        std::cerr << "Error: Attempted to add a null physics component." << std::endl;
        return;
    }
    physicsComponent = newPhysicsComponent;
}

/**
 * Set the entity's active state. If active, the graphic model becomes visible.
 * @param state The new active state.
 */
void Entity::setActive(bool state) {
    if (graphicComponent) {
        get_Graphic_Model()->set_visible(state);
    }
    active = state;
}

/**
 * Get the transformation matrix of the entity based on its physics component.
 * @return The transformation matrix (glm::mat4).
 */
glm::mat4 Entity::getTransform() const {
    if (!physicsComponent) {
        std::cerr << "Error: Physics component is null, returning identity matrix." << std::endl;
        return glm::mat4(1.0f);  // Return identity matrix as fallback
    }
    return physicsComponent->getTransform();
}