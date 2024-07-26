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

#include <btBulletDynamicsCommon.h>
#include "Entity.h"
#include <memory>

class Platform :public Entity {
public:
     std::shared_ptr<btVector3> startPoint;
     std::shared_ptr<btVector3>  endPoint;
     std::shared_ptr<btVector3>  velocity;
    Platform() 
    {
        startPoint = make_shared< btVector3>(10, -1., -20);
        endPoint = make_shared< btVector3>(10, -1, 20);
        velocity = make_shared< btVector3>(0, 0, 0.025);
    } 

    ~Platform() = default;

    void movePlatform( )
    {
        btTransform transform;
        getBody()->getMotionState()->getWorldTransform(transform);
        btVector3 currentPosition = transform.getOrigin();

        // Check if the platform has reached the end point
        if ((currentPosition - *endPoint).length() < velocity->length()) {
            // Reverse direction
            std::swap(startPoint, endPoint);
            *velocity = -*velocity;
        }

        // Update the position of the platform
        btVector3 newPosition = currentPosition + *velocity;
        transform.setOrigin(newPosition);
        getBody()->getMotionState()->setWorldTransform(transform);
        getBody()->setWorldTransform(transform);
    }
};
