/**********************************************************************
*Project           : Bullet3D Practice
*
*Author : Lucas García
*
*
*Purpose : Physics Practice using Bullet that moves a tank and other features
*
**********************************************************************/

 /**
  * \class Entity
  * \brief Whatever "object" that is in scene and has a graphic and a physic component
  *
  * Adds a graphic component for visualization and a physic component for having collisions in the scene
  */

#pragma once

#include <memory>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <Render_Node.hpp>
#include "Graphic_Component.h"
#include "Physics_Component.h"

using namespace std;
using namespace glt;

class Entity {
public:
    btVector3 position;
    btVector3 rotation;
    btVector3 scale;
public:
    Entity() =  default;
    ~Entity() = default;
    /**
 * \brief Gets the rigidbody component of the entity.
 * \param[out] btRigidBody* Gets the physic component of the entity in a pointer of a btRigidBody class of Bullet.
 */
    btRigidBody* getBody() const;
    std::shared_ptr<btRigidBody> getSharedBody() const;
    /**
 * \brief Gets the transform component of the entity.
 * \param[out] mat4 Gets the transform matrix in a mat4 class of glm.
 */
    glm::mat4 getTransform() const;
    Node* get_Graphic_Model();
    void addGraphicComponent(std::shared_ptr <Graphic_Component>  model);
    void addPhysicsComponent(std::shared_ptr <Physics_Component>  model);
    /**
* \brief Makes the object visible when is selected
*/
    void setActive(bool state);


    bool isActive() const { return active; }
private:
    bool active;
    std::shared_ptr < Entity> parent;
    std::shared_ptr < Graphic_Component> graphicComponent;
    std::shared_ptr < Physics_Component> physicsComponent;
};