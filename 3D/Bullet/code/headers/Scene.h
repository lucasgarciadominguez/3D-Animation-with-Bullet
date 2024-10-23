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
#include <vector>

#include <btBulletDynamicsCommon.h>
#include "Render_Node.hpp"
#include "Graphics_3D_System.h"
#include <Physics_3D_System.h>
#include <Platform.h>

class Physics_3D_System;
class Graphics_3D_System;
class Entity;
class Tank;
class ContactListener;

class Scene
{
private:
    std::shared_ptr < Graphics_3D_System > graphics_system;
    std::shared_ptr < Physics_3D_System > physics_system;
    std::shared_ptr <ContactListener > contactListener;

    std::unique_ptr< btDiscreteDynamicsWorld > dynamicsWorld;
    sf::Window window;
    std::map<std::string, std::shared_ptr <Entity >> entities;

    std::shared_ptr <Tank > tankCharacter;
    std::shared_ptr <Entity > key;
    std::shared_ptr <Platform> platform;
    bool running = true;
    int  frame = 0;

public:

    Scene();
    ~Scene();

    void addEntity(const std::string& name, std::shared_ptr<Entity> entity, const btVector3& origin,
        const btVector3& shapeSize, btScalar mass, btVector3 scale,btVector3 color);
    void addDoor(const std::string& name, std::shared_ptr<Entity> entity, const btVector3& origin,
        const btVector3& shapeSize, btScalar mass, btVector3 scale, btVector3 color);
    void addKey(const std::string& name, std::shared_ptr<Entity> entity, const btVector3& origin,
        const btVector3& shapeSize, btScalar mass, btVector3 scale, btVector3 color);
    void addPlatform(const std::string& name, std::shared_ptr<Platform> entity, const btVector3& origin,
        const btVector3& shapeSize, btScalar mass, btVector3 scale, btVector3 color);
    void addTank(const std::string& name, std::shared_ptr<Tank> tank);
    void run();
    void handleTankMovement();
    void applyTankForce(const btVector3& force, const btMatrix3x3& rotation);
    void applyTankTurningForce(const btVector3& leftForce, const btVector3& rightForce, const btMatrix3x3& rotation);
    void updateGraphicsTransforms();
    void resetDynamicsWorld(); // new mehtod for resetting the dynamicsword

};