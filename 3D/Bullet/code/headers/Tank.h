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
#include <glm/glm.hpp>
#include <Render_Node.hpp>
#include "Graphic_Component.h"
#include "Physics_Component.h"
#include "Graphics_3D_System.h"
#include "Physics_3D_System.h"
#include "Entity.h"

using namespace std;
using namespace glt;

class Projectile;
class Graphics_3D_System;
class Physics_3D_System;
class Graphic_Component;
class Physics_Component;

class Tank : public Entity{

    public:
        Tank();
        ~Tank();

        void shootProjectile(shared_ptr<Graphics_3D_System> graphicsSystem, shared_ptr<Physics_3D_System> physicsSystem,
            std::map<std::string, std::shared_ptr<Entity>> entities);

    public:
        shared_ptr<Entity> leftTrack;
        shared_ptr<Entity> rightTrack;
        shared_ptr<Entity> chasis;
        shared_ptr<Entity> turret;
        shared_ptr<Entity> canyon;
        shared_ptr<btHingeConstraint> leftTrackConstraint;
        shared_ptr<btHingeConstraint> rightTrackConstraint;
        shared_ptr<btFixedConstraint> turretConstraint;
        shared_ptr<btFixedConstraint> canyonConstraint;
        std::shared_ptr<Projectile> projectile;
        vector< shared_ptr< Projectile > > projectiles;
        size_t nextProjectileIndex; // index of the next projectile available

};