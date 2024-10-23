/**********************************************************************
*Project           : Bullet3D Practice
*
*Author : Lucas Garc�a
*
*
*Purpose : Physics Practice using Bullet that moves a tank and other features
*
**********************************************************************/

#pragma once

#include <btBulletDynamicsCommon.h>
#include "Entity.h"

class Projectile :public Entity {
public:
    Projectile() = default;

    ~Projectile() = default;
};