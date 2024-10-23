/**********************************************************************
*Project           : Bullet3D Practice
*
*Author : Lucas García
*
*
*Purpose : Physics Practice using Bullet that moves a tank and other features
*
**********************************************************************/

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Physics_3D_System.h"
#include "Physics_Component.h"
#include "Entity.h"

using namespace std;
using namespace glt;

const float GRAVITY = -10.0f;
const float LINEAR_SLOP = 0.01f;
const float RESTITUTION = 0.0f;

/**
 * Constructor for the 3D Physics System.
 * Initializes the physics world and sets up basic simulation parameters.
 */
Physics_3D_System::Physics_3D_System() : collisionDispatcher(&collisionConfiguration)
{
    // Create the dynamics world for physics simulation
    dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
        &collisionDispatcher,
        &overlappingPairCache,
        &constraintSolver,
        &collisionConfiguration
    );

    // Configure gravity and solver parameters
    dynamicsWorld->setGravity(btVector3(0, GRAVITY, 0));  // Set gravity in Y-axis
    dynamicsWorld->getSolverInfo().m_linearSlop = LINEAR_SLOP;  // Linear precision in simulation
    dynamicsWorld->getSolverInfo().m_restitution = RESTITUTION;  // Restitution coefficient for physics
}


/**
 * Destructor for the 3D Physics System.
 * Cleans up physics objects, including collision objects, sensor objects, rigid bodies,
 * and removes them from the dynamics world.
 */
Physics_3D_System::~Physics_3D_System()
{
    // Remove all collision objects from the dynamics world
    for (auto& collisionObject : collisionObjects) {
        dynamicsWorld->removeCollisionObject(collisionObject.get());
    }

    // Remove all sensor objects from the dynamics world
    for (auto& collisionObject : sensorObjects) {
        dynamicsWorld->removeCollisionObject(collisionObject.get());
    }

    // Remove all rigid bodies from the dynamics world
    for (auto& rigidBody : rigidBodies) {
        dynamicsWorld->removeRigidBody(rigidBody.get());
    }

    // Clear the collision shapes, motion states, and rigid bodies
    collisionShapes.clear();
    motionStates.clear();
    rigidBodies.clear();
    collisionObjects.clear();
}

/**
 * Step the simulation forward by the given time step.
 * @param timeStep The time step for the simulation.
 */
void Physics_3D_System::stepSimulation(float timeStep) {
    dynamicsWorld->stepSimulation(timeStep);
}
/**
 * Add a rigid body component to the entity.
 * @param entity The entity to add the physics component to.
 * @param origin The initial position of the component.
 * @param shapeSize The size of the collision shape.
 * @param mass The mass of the object.
 */
void Physics_3D_System::add_Component(Entity& entity, 
    const btVector3& origin, const btVector3& shapeSize, btScalar mass)
{
    auto collisionShape = std::make_shared<btBoxShape>(shapeSize);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(origin);

    btVector3 localInertia(0, 0, 0);
    if (mass != 0.f)
        collisionShape->calculateLocalInertia(mass, localInertia);

    auto motionState = std::make_shared<btDefaultMotionState>(transform);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motionState.get(), collisionShape.get(), localInertia);
    auto rigidBody = std::make_shared<btRigidBody>(info);

    dynamicsWorld->addRigidBody(rigidBody.get());

    auto physicsComponent = std::make_shared<Physics_Component>(collisionShape,motionState, rigidBody);
    entity.addPhysicsComponent(physicsComponent);

    rigidBodies    .push_back (rigidBody);
    motionStates   .push_back (motionState);
    collisionShapes.push_back (collisionShape);
}
void Physics_3D_System::add_ComponentSensor(Entity& entity,
    const btVector3& origin, const btVector3& shapeSize, btScalar mass)
{
    Physics_3D_System::add_Component(entity,origin,shapeSize,mass);

    // Create and add sensor object
    auto sensor = std::make_shared<btGhostObject>();
    sensor->setCollisionShape(entity.getBody()->getCollisionShape());
    sensor->setWorldTransform(entity.getBody()->getWorldTransform());
    sensor->setCollisionFlags(sensor->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    dynamicsWorld->addCollisionObject(sensor.get());

    sensorObjects.push_back(sensor);
}
/**
 * Add a collision component to the entity.
 * @param entity The entity to add the collision component to.
 * @param origin The initial position of the object.
 * @param shapeSize The size of the collision shape.
 * @param mass The mass of the object.
 */
void Physics_3D_System::add_ComponentCollision(Entity& entity,
    const btVector3& origin, const btVector3& shapeSize, btScalar mass)
{
    Physics_3D_System::add_Component(entity, origin, shapeSize, mass);

    auto mPlayerObject= std::make_shared<btCollisionObject>();
    mPlayerObject->setCollisionShape(entity.getBody()->getCollisionShape());
    dynamicsWorld->addCollisionObject(mPlayerObject.get());

    collisionObjects.push_back(mPlayerObject);
}

/**
 * Add a sphere component to the entity.
 * @param entity The entity to add the physics component to.
 * @param origin The initial position of the sphere.
 * @param shapeSize The size of the sphere (currently unused).
 * @param mass The mass of the sphere.
 */
void Physics_3D_System::add_ComponentSphere(Entity& entity,
    const btVector3& origin, const btVector3& shapeSize, btScalar mass)
{
    // Create the collision shape for the projectile (in this case, a sphere)
    btScalar radius = 0.1f;
    auto collisionShape = std::make_shared<btSphereShape>(radius);

    // Create the rigid body for the projectile
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(origin);

    // Calculate the local inertia (projectiles are immune to rotation)
    btVector3 localInertia(0, 0, 0);
    collisionShape->calculateLocalInertia(mass, localInertia);

    // Create the motion state and rigid body for the projectile
    auto motionState = std::make_shared<btDefaultMotionState>(startTransform);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motionState.get(), collisionShape.get(), localInertia);
    auto rigidBody = std::make_shared<btRigidBody>(info);

    // Add the rigid body to the dynamics world
    dynamicsWorld->addRigidBody(rigidBody.get());

    // Create and add the physics component to the entity
    auto physicsComponent = std::make_shared<Physics_Component>(collisionShape, motionState, rigidBody);
    entity.addPhysicsComponent(physicsComponent);

    // Add the rigid body, motion state, and collision shape to the internal lists
    rigidBodies.push_back(rigidBody);
    motionStates.push_back(motionState);
    collisionShapes.push_back(collisionShape);
}
/**
 * Add a rigid body component to the entity and return the rigid body.
 * @param entity The entity to add the physics component to.
 * @param origin The initial position of the object.
 * @param shapeSize The size of the collision shape.
 * @param mass The mass of the object.
 * @return The created rigid body.
 */
std::shared_ptr<btRigidBody> Physics_3D_System::add_ComponentAndReturnRigidBody(Entity& entity, const btVector3& origin, const btVector3& shapeSize, btScalar mass)
{

    Physics_3D_System::add_Component(entity, origin, shapeSize, mass);

    auto rigidBody = entity.getSharedBody();

    // Returns the pointer to the rigidbody
    return rigidBody;
}
/**
 * Get the dynamics world.
 * @return A pointer to the dynamics world.
 */
btDynamicsWorld* Physics_3D_System::getDynamicsWorld() const
{
	return dynamicsWorld.get();
}
