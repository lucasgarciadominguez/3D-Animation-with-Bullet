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

Physics_3D_System::Physics_3D_System() : collisionDispatcher(&collisionConfiguration)
{
    dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
        &collisionDispatcher,
        &overlappingPairCache,
        &constraintSolver,
        &collisionConfiguration
    );

    dynamicsWorld->setGravity(btVector3(0, -10, 0));
    dynamicsWorld->getSolverInfo().m_linearSlop = 0.01; // Ajustar según sea necesario
    dynamicsWorld->getSolverInfo().m_restitution = 0.0; // Ajustar según sea necesario
}

Physics_3D_System::~Physics_3D_System()
{
    for (auto& collisionObject : collisionObjects) {
        dynamicsWorld->removeCollisionObject(collisionObject.get());
    }
    for (auto& collisionObject : sensorObjects) {
        dynamicsWorld->removeCollisionObject(collisionObject.get());
    }
    // Eliminar cuerpos rígidos del mundo
    for (auto& rigidBody : rigidBodies) {
        dynamicsWorld->removeRigidBody(rigidBody.get());
    }

    // Limpiar las formas de colisión, estados de movimiento y cuerpos rígidos
    collisionShapes.clear();
    motionStates.clear();
    rigidBodies.clear();
    collisionObjects.clear();
}

void Physics_3D_System::stepSimulation(float timeStep) {
    dynamicsWorld->stepSimulation(timeStep);
}

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

    auto physicsComponent = std::make_shared<Physics_Component>(collisionShape, motionState, rigidBody);
    entity.addPhysicsComponent(physicsComponent);


    // Opcional: Añadir un sensor si es necesario
    auto sensor = std::make_shared<btGhostObject>();
    sensor->setCollisionShape(collisionShape.get());
    sensor->setWorldTransform(transform);
    sensor->setCollisionFlags(sensor->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    dynamicsWorld->addCollisionObject(sensor.get());

    rigidBodies.push_back(rigidBody);
    motionStates.push_back(motionState);
    collisionShapes.push_back(collisionShape);
    sensorObjects.push_back(sensor);
}

void Physics_3D_System::add_ComponentCollision(Entity& entity,
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

    auto physicsComponent = std::make_shared<Physics_Component>(collisionShape, motionState, rigidBody);
    entity.addPhysicsComponent(physicsComponent);

    auto mPlayerObject= std::make_shared<btCollisionObject>();
    mPlayerObject->setCollisionShape(collisionShape.get());
    dynamicsWorld->addCollisionObject(mPlayerObject.get());

    rigidBodies.push_back(rigidBody);
    motionStates.push_back(motionState);
    collisionShapes.push_back(collisionShape);
    collisionObjects.push_back(mPlayerObject);
}
void Physics_3D_System::add_ComponentSphere(Entity& entity,
    const btVector3& origin, const btVector3& shapeSize, btScalar mass)
{
    // Crear la forma del proyectil (en este caso, una esfera)
    btScalar radius = .1f;
    auto collisionShape = std::make_shared<btSphereShape>(radius);

    // Crear el cuerpo rígido del proyectil
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(origin);

    btVector3 localInertia(0, 0, 0); // Los proyectiles son inmunes a la rotación
    collisionShape->calculateLocalInertia(mass, localInertia);

    auto motionState = std::make_shared<btDefaultMotionState>(startTransform);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motionState.get(), collisionShape.get(), localInertia);
    auto rigidBody = std::make_shared<btRigidBody>(info);

    // Establecer la velocidad inicial del proyectil
    dynamicsWorld->addRigidBody(rigidBody.get());

    auto physicsComponent = std::make_shared<Physics_Component>(collisionShape, motionState, rigidBody);
    entity.addPhysicsComponent(physicsComponent);
    // Agregar el cuerpo rígido al mundo de dinámica

    rigidBodies.push_back(rigidBody);
    motionStates.push_back(motionState);
    collisionShapes.push_back(collisionShape);
}
std::shared_ptr<btRigidBody> Physics_3D_System::add_ComponentAndReturnRigidBody(Entity& entity, const btVector3& origin, const btVector3& shapeSize, btScalar mass)
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

    auto physicsComponent = std::make_shared<Physics_Component>(collisionShape, motionState, rigidBody);
    entity.addPhysicsComponent(physicsComponent);

    rigidBodies.push_back(rigidBody);
    motionStates.push_back(motionState);
    collisionShapes.push_back(collisionShape);

    return rigidBody;
}



btDynamicsWorld* Physics_3D_System::getDynamicsWorld() const
{
	return dynamicsWorld.get();
}
