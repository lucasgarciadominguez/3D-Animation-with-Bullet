/**********************************************************************
*Project           : Bullet3D Practice
*
*Author : Lucas García
*
*
*Purpose : Physics Practice using Bullet that moves a tank and other features
*
**********************************************************************/

#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <Render_Node.hpp>

using namespace std;
using namespace glt;

class Physics_Component
{
private:
    std::shared_ptr<btCollisionShape> collisionShape;
    std::shared_ptr<btDefaultMotionState> motionState;
    std::shared_ptr<btRigidBody> rigidBody;


public:
    Physics_Component(std::shared_ptr<btCollisionShape> newcollisionShape,
    std::shared_ptr<btDefaultMotionState> newmotionState,
    std::shared_ptr<btRigidBody> newrigidBody)
    {
        collisionShape = newcollisionShape;
        motionState = newmotionState;
        rigidBody = newrigidBody;
    }
    ~Physics_Component() = default;

    btRigidBody* getRigidbody()
    {
        return rigidBody.get();
    }
    std::shared_ptr<btRigidBody> getSharedRigidbody()
    {
        return rigidBody;
    }
    glm::mat4 getTransform() const {
        btTransform transform;
        rigidBody->getMotionState()->getWorldTransform(transform);

        glm::mat4 graphics_transform;
        transform.getOpenGLMatrix(glm::value_ptr(graphics_transform));
        return graphics_transform;
    }
};
