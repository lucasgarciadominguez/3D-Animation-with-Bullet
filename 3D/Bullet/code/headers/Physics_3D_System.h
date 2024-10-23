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

#include <SFML/Window.hpp>
#include "Render_Node.hpp"
#include "Scene.h"

class btGhostObject;
class btCollisionObject;
class Entity;

class Physics_3D_System
{
    private:
        btDefaultCollisionConfiguration collisionConfiguration;

        // Use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded).

        btCollisionDispatcher collisionDispatcher;

        // btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.

        btDbvtBroadphase overlappingPairCache;

        // The default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded).

        btSequentialImpulseConstraintSolver constraintSolver;

        std::unique_ptr< btDiscreteDynamicsWorld > dynamicsWorld;

        std::vector< std::shared_ptr< btRigidBody          > > rigidBodies;
        std::vector< std::shared_ptr< btDefaultMotionState > > motionStates;
        std::vector< std::shared_ptr< btCollisionShape     > > collisionShapes;
        std::vector< std::shared_ptr< btGhostObject        > > sensorObjects;
        std::vector< std::shared_ptr< btCollisionObject    > > collisionObjects;

    public:

        Physics_3D_System();

        ~Physics_3D_System();

        void stepSimulation(float timeStep);
        std::shared_ptr<btRigidBody> add_ComponentAndReturnRigidBody(Entity& entity, const btVector3& origin, const btVector3& shapeSize, btScalar mass);
        void add_Component(Entity& entity,
            const btVector3& origin, const btVector3& shapeSize, btScalar mass);
        void add_ComponentSensor(Entity& entity,
            const btVector3& origin, const btVector3& shapeSize, btScalar mass);
        void add_ComponentCollision(Entity& entity,
            const btVector3& origin, const btVector3& shapeSize, btScalar mass);
        void add_ComponentSphere(Entity& entity,
            const btVector3& origin, const btVector3& shapeSize, btScalar mass);
        std::shared_ptr<btRigidBody> createRigidBody(const btVector3& origin, const btVector3& shapeSize, btScalar mass);
        btDynamicsWorld* getDynamicsWorld() const;

   
};
