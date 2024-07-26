/**
 * \author Lucas Garcia
 * \version 1.0
 * \date 2024-06-05
 */

 /**
  * \class ContactListener
  * \brief Gets important info from selected contacts and enable some methods when this contacts are solved
  *
  * Enables the door opening when the key is contacted by the tank. The door moves from a selected point to another.
  */
#pragma once

#include <memory>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <Render_Node.hpp>
#include <Entity.h>
#include <Tank.h>

using namespace std;
using namespace glt;

class ContactListener {
private:
    std::shared_ptr<Entity> key;
    std::shared_ptr<Tank> tank;
    std::shared_ptr<Entity> door;

    std::shared_ptr<btVector3> startPoint;
    std::shared_ptr<btVector3>  endPoint;
    std::shared_ptr<btVector3>  velocity;
public:
    bool openDoor = false;
public:
    ContactListener()
    {
        startPoint = make_shared< btVector3>(16, 3, 0);
        endPoint = make_shared< btVector3>(16, 10, 0);
        velocity = make_shared< btVector3>(0, 0.05, 0.);
    }

    void addKey(std::shared_ptr<Entity> newKey) 
    {
        key = newKey;

    }
    void addTank(std::shared_ptr<Tank> newTank)
    {
        tank = newTank;
        door->getBody()->setIgnoreCollisionCheck(tank->leftTrack->getBody(), true);
        door->getBody()->setIgnoreCollisionCheck(tank->rightTrack->getBody(), true);
        door->getBody()->setIgnoreCollisionCheck(tank->chasis->getBody(), true);
        tank->leftTrack->getBody()->setIgnoreCollisionCheck(door->getBody(), true);
        tank->rightTrack->getBody()->setIgnoreCollisionCheck(door->getBody(), true);
        tank->chasis->getBody()->setIgnoreCollisionCheck(door->getBody(), true);
    }
    void addDoor(std::shared_ptr<Entity> newDoor)
    {
        door = newDoor;
    }
    /**
 * \brief The door moves from a point to another during certain time
 */
    void EnableDoor()
    {
        btTransform transform;
        door->getBody()->getMotionState()->getWorldTransform(transform);
        btVector3 currentPosition = transform.getOrigin();

        // Check if the platform has reached the end point
        if ((currentPosition - *endPoint).length() < velocity->length()) {
            *velocity = btVector3(0, 0, 0);
            openDoor = false;
        }

        // Update the position of the platform
        btVector3 newPosition = currentPosition + *velocity;
        transform.setOrigin(newPosition);
        door->getBody()->getMotionState()->setWorldTransform(transform);
        door->getBody()->setWorldTransform(transform);
    }
    /**
 * \brief If the contact is between the key and some of the tank pieces, then the door is open.
 */
    void RunContacts(btDynamicsWorld& dynamicsWorld)
    {
        btCollisionObject& collisionTankLeftTrack = *tank->leftTrack->getBody();
        btCollisionObject& collisionTankRightTrack = *tank->rightTrack->getBody();
        btCollisionObject& collisionTankChassis = *tank->chasis->getBody();
        btCollisionObject& collisionKey = *key->getBody();

        int manifold_count = dynamicsWorld.getDispatcher()->getNumManifolds();
        for (int i = 0; i < manifold_count; i++)
        {
            btPersistentManifold* manifold =
                dynamicsWorld.getDispatcher()->getManifoldByIndexInternal(i);
            btCollisionObject* object_a = (btCollisionObject*)(manifold->getBody0());
            btCollisionObject* object_b = (btCollisionObject*)(manifold->getBody1());
            int numContacts = manifold->getNumContacts();
            for (int j = 0; j < numContacts; j++)
            {
                btManifoldPoint& point = manifold->getContactPoint(j);
                if (point.getDistance() < 0.f)
                {
                    if
                        (
                            (object_a == &collisionTankLeftTrack && object_b == &collisionKey) ||
                            (object_a == &collisionKey && object_b == &collisionTankLeftTrack) || (object_a == &collisionTankRightTrack && object_b == &collisionKey) ||
                            (object_a == &collisionKey && object_b == &collisionTankRightTrack) || (object_a == &collisionTankChassis && object_b == &collisionKey) ||
                            (object_a == &collisionKey && object_b == &collisionTankChassis)
                            )
                    {
                        key->setActive(false);
                        if (door && door->getBody()) 
                        {
                            openDoor = true;
                        }
   
                    }
                }
            }
        }

    }



};