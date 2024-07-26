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
#include <vector>

#include "Tank.h"
#include <glm/gtc/type_ptr.hpp>
#include <Model.hpp>
#include <Cube.hpp>
#include "Entity.h"
#include <OpenGL.hpp>
#include <Model_Obj.hpp>
#include <Render_Node.hpp>
#include "Physics_Component.h"
#include <Projectile.h>
#include <Graphics_3D_System.h>
#include <Physics_3D_System.h>

using namespace std;
using namespace glt;

Tank::Tank() {
    leftTrack = make_shared<Entity>();
    rightTrack = make_shared<Entity>();
    chasis = make_shared<Entity>();
    turret=make_shared<Entity>();
    canyon = make_shared<Entity>();
    nextProjectileIndex=0;


}

Tank::~Tank() {
    // Destructor cleanup if necessary
}
void Tank::shootProjectile(shared_ptr<Graphics_3D_System> graphicsSystem,shared_ptr<Physics_3D_System> physicsSystem,
    std::map<std::string, std::shared_ptr <Entity >> entities) {

    //// Obtener la transformación actual del cañón
    btTransform canyonTransform;
    canyon->getBody()->getMotionState()->getWorldTransform(canyonTransform);

    //// Calcular la posición inicial del proyectil, desplazada hacia adelante
    btVector3 projectilePosition = canyonTransform.getOrigin() + canyonTransform.getBasis() * btVector3(0, 0, -0.5f);
    btTransform newTransform;
    newTransform.setIdentity(); // Restablecer la transformación a la identidad (sin cambios)
    newTransform.setOrigin(projectilePosition); // Establecer la nueva posición del RigidBody

    // Aplicar una fuerza de disparo a la bala
    btVector3 forwardDirection = canyonTransform.getBasis() * btVector3(0, 0, -1);
    btScalar shootingForce = 3000.0f;  // Magnitud de la fuerza de disparo
    btVector3 shootingForceVector = forwardDirection * shootingForce;  // Vector de fuerza de disparo

    projectiles[nextProjectileIndex]->setActive(true);
    projectiles[nextProjectileIndex]->getBody()->setWorldTransform(newTransform);
    projectiles[nextProjectileIndex]->getBody()->applyCentralForce(shootingForceVector);

    if (nextProjectileIndex==projectiles.size()-1)
    {
        nextProjectileIndex=0;
    }
    else
    {
        nextProjectileIndex++;
    }
}