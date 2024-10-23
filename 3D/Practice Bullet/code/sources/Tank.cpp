/**********************************************************************
* Project           : Bullet3D Practice
*
* Author            : Lucas García
*
* Purpose           : Physics Practice using Bullet that moves a tank and other features.
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

/**
 * Constructor for the Tank class.
 * Initializes the tank components (left track, right track, chassis, turret, and cannon).
 */
Tank::Tank()
    : nextProjectileIndex(0)  // Initialize the projectile index
{
    // Initialize the tank components
    leftTrack = make_shared<Entity>();
    rightTrack = make_shared<Entity>();
    chasis = make_shared<Entity>();
    turret = make_shared<Entity>();
    canyon = make_shared<Entity>();
}

/**
 * Destructor for the Tank class.
 * No explicit cleanup required here, as shared_ptr will automatically manage memory.
 */
Tank::~Tank() {
    // Destructor cleanup if necessary (automatic with shared_ptr)
}

/**
 * Fires a projectile from the tank's cannon.
 * @param graphicsSystem Shared pointer to the graphics system.
 * @param physicsSystem Shared pointer to the physics system.
 * @param entities Map of the entities in the scene.
 */
void Tank::shootProjectile(shared_ptr<Graphics_3D_System> graphicsSystem, shared_ptr<Physics_3D_System> physicsSystem,
    std::map<std::string, std::shared_ptr<Entity>> entities)
{
    // Get the current transformation of the cannon
    btTransform canyonTransform;
    canyon->getBody()->getMotionState()->getWorldTransform(canyonTransform);

    // Calculate the initial position of the projectile, slightly forward of the cannon
    btVector3 projectilePosition = canyonTransform.getOrigin() + canyonTransform.getBasis() * btVector3(0, 0, -0.5f);

    // Set the new transformation for the projectile
    btTransform newTransform;
    newTransform.setIdentity();  // Reset to identity matrix (no scaling or rotation)
    newTransform.setOrigin(projectilePosition);  // Set the new position of the projectile

    // Calculate the shooting force in the forward direction of the cannon
    btVector3 forwardDirection = canyonTransform.getBasis() * btVector3(0, 0, -1);  // Direction the cannon is facing
    btScalar shootingForce = 3000.0f;  // Magnitude of the shooting force
    btVector3 shootingForceVector = forwardDirection * shootingForce;  // Force vector for the projectile

    // Activate the next projectile and apply the transformation and force
    auto& projectile = projectiles[nextProjectileIndex];  // Get the current projectile
    projectile->setActive(true);  // Activate the projectile
    projectile->getBody()->setWorldTransform(newTransform);  // Set the new transform (position)
    projectile->getBody()->applyCentralForce(shootingForceVector);  // Apply the shooting force

    // Update the index to point to the next projectile in the list
    nextProjectileIndex = (nextProjectileIndex + 1) % projectiles.size();  // Wrap around when the last projectile is reached
}
