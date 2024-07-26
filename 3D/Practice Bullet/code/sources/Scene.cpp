/**********************************************************************
*Project           : Bullet3D Practice
*
*Author : Lucas García
*
*
*Purpose : Physics Practice using Bullet that moves a tank and other features
*
**********************************************************************/

#include "Scene.h"
#include "Graphics_3D_System.h"
#include "Entity.h"
#include "Tank.h"
#include <Projectile.h>
#include "ContactListener.h"

Scene::Scene() 
{

       window.create
       (
           sf::VideoMode(1024, 720),
           "Bullet Constraints",
           sf::Style::Default,
           sf::ContextSettings(24, 0, 0, 3, 2, sf::ContextSettings::Core)      // Se usa OpenGL 3.2 core profile
       );
    graphics_system = std::make_shared<Graphics_3D_System>
        ();
    physics_system = std::make_shared<Physics_3D_System>
        ();
    contactListener = std::make_shared<ContactListener>();
    window.setVerticalSyncEnabled(true);
    graphics_system->initialize(window);

    
}

Scene::~Scene() {
    // Cleanup if necessary
}

void Scene::addEntity(const std::string& name, std::shared_ptr<Entity> entity, const btVector3& origin,
    const btVector3& shapeSize, btScalar mass, btVector3 scale, btVector3 color)
{
    graphics_system->add_Component( name,*entity, scale,color);
    physics_system->add_Component( *entity, origin,shapeSize,mass);

    entities[name] = entity;
    entities[name]->position = origin;
    entities[name]->scale = scale;
}
void Scene::addDoor(const std::string& name, std::shared_ptr<Entity> entity, const btVector3& origin,
    const btVector3& shapeSize, btScalar mass, btVector3 scale, btVector3 color)
{
    graphics_system->add_Component(name, *entity, scale, color);
    physics_system->add_Component(*entity, origin, shapeSize, mass);

    entities[name] = entity;
    entities[name]->position = origin;
    entities[name]->scale = scale;
    //entities[name]->getBody()->setGravity(btVector3(0, 0, 0));

    contactListener->addDoor(entity);
}
void Scene::addKey(const std::string& name, std::shared_ptr<Entity> entity, const btVector3& origin,
    const btVector3& shapeSize, btScalar mass, btVector3 scale, btVector3 color)
{
    graphics_system->add_ComponentKey(name, *entity, scale, color);
    const btVector3 scaleKey(.25f, 1.f, 2.f);
    physics_system->add_ComponentSensor(*entity, origin, scaleKey, mass);

    entities[name] = entity;
    entities[name]->position = origin;
    entities[name]->scale = scale;
    key = entity;
    contactListener->addKey(entity);
}

void Scene::addPlatform(const std::string& name, std::shared_ptr<Platform> entity, const btVector3& origin,
    const btVector3& shapeSize, btScalar mass, btVector3 scale, btVector3 color)
{
    graphics_system->add_Component(name, *entity, scale, color);
    physics_system->add_Component(*entity, origin, shapeSize, mass);

    entities[name] = entity;
    entities[name]->position = origin;
    entities[name]->scale = scale;
    platform = entity;

}

void Scene::addTank(const std::string& name, std::shared_ptr<Tank> tank)
{
    btScalar chasisMass = 1.0f; // Ajusta este valor según sea necesario
    btScalar trackMass = 1.0f; // Ajusta este valor según sea necesario
    btScalar displacementX = -3.0f; // Ajusta este valor según sea necesario
    btScalar displacementZ = -3.0f; // Ajusta este valor según sea necesario

    // Left track
    btVector3 leftTrackPosition(-1.2f, .0f, 0.0f);
    const btVector3 trackScale(.25f, .25f, 1.f);
    btVector3 trackColor(0.216f, 0.541f, 0.243f);
    graphics_system->add_Component("leftTrack", *tank->leftTrack, trackScale, trackColor);
    physics_system->add_ComponentSensor(*tank->leftTrack, leftTrackPosition, trackScale, trackMass);
    entities["leftTrack"] = tank->leftTrack;
    entities["leftTrack"]->position = leftTrackPosition;  // Set initial position
    entities["leftTrack"]->scale = trackScale;     // Set initial scale

    //// Right track
    btVector3 rightTrackPosition(1.2f, .0f, 0.0f);
    graphics_system->add_Component("rightTrack", *tank->rightTrack, trackScale, trackColor);
    physics_system->add_ComponentSensor(*tank->rightTrack, rightTrackPosition, trackScale, trackMass);
    entities["rightTrack"] = tank->rightTrack;
    entities["rightTrack"]->position = rightTrackPosition;  // Set initial position
    entities["rightTrack"]->scale = trackScale;     // Set initial scale

    // Chassis
    btVector3 chasisPosition(0.0f, 0.0f, 0.0f);
    btVector3 chasisScale(1.0f, 0.2f, 1.f);
    btVector3 chasisColor(0.36f, 0.541f, 0.243f);
    graphics_system->add_Component("chassis", *tank->chasis, chasisScale, chasisColor);
    physics_system->add_ComponentSensor(*tank->chasis, chasisPosition, chasisScale, chasisMass);
    entities["chassis"] = tank->chasis;
    entities["chassis"]->position = chasisPosition;
    entities["chassis"]->scale = chasisScale;
    
    //// Turret
    btVector3 turretPosition(0.0f, .55f, 0.0f);
    btVector3 turretScale(0.5f, 0.2f, 0.35f);
    btVector3 turretColor(0.255f, 0.529f, 0.278f);
    graphics_system->add_Component("turret", *tank->turret, turretScale, turretColor);
    physics_system->add_Component(*tank->turret, turretPosition, turretScale, 1.0f);
    entities["turret"] = tank->turret;
    entities["turret"]->position = turretPosition;  // Set initial position
    entities["turret"]->scale = turretScale;     // Set initial scale

    //// Canyon
    btVector3 canyonPosition(.0f, .55f, -2.f);
    btVector3 canyonScale(0.2f, 0.1f, 0.5f);
    btVector3 canyonColor(0.255f, 0.529f, 0.378f);
    graphics_system->add_Component("canyon", *tank->canyon, canyonScale, canyonColor);
    physics_system->add_Component(*tank->canyon, canyonPosition, canyonScale, 1.0f);
    entities["canyon"] = tank->canyon;
    entities["canyon"]->position = canyonPosition;  // Set initial position
    entities["canyon"]->scale = canyonScale;     // Set initial scale

    //// Add constraints
    tank->leftTrackConstraint = make_shared<btHingeConstraint>(*tank->chasis->getBody(), *tank->leftTrack->getBody(),
        btVector3(-1.2f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f), btVector3(0, 1, 0), btVector3(0, 1, 0),false);

    tank->rightTrackConstraint = make_shared<btHingeConstraint>(*tank->chasis->getBody(), *tank->rightTrack->getBody(),
        btVector3(1.2f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f), btVector3(0, 1, 0), btVector3(0, 1, 0),false);

    btTransform frameInA, frameInB;
    frameInA = btTransform::getIdentity();
    frameInA.setOrigin(btVector3(0.0f, .50f, 0.0f));  // Position of the turret relative to the chassis
    frameInB = btTransform::getIdentity();
    tank->turretConstraint = std::make_shared<btFixedConstraint>(*tank->chasis->getBody(), *tank->turret->getBody(), frameInA, frameInB);

    // Use btFixedConstraint for the cannon to fix it to the turret
    btTransform cannonFrameInA, cannonFrameInB;
    cannonFrameInA = btTransform::getIdentity();
    cannonFrameInA.setOrigin(btVector3(0.0f, 0.0f, -0.85f));  // Position of the cannon relative to the turret
    cannonFrameInB = btTransform::getIdentity();
    tank->canyonConstraint = std::make_shared<btFixedConstraint>(*tank->turret->getBody(), *tank->canyon->getBody(), cannonFrameInA, cannonFrameInB);

    physics_system->getDynamicsWorld()->addConstraint(tank->turretConstraint.get());
    physics_system->getDynamicsWorld()->addConstraint(tank->canyonConstraint.get());

    physics_system->getDynamicsWorld()->addConstraint(tank->leftTrackConstraint.get());
    physics_system->getDynamicsWorld()->addConstraint(tank->rightTrackConstraint.get());

    contactListener->addTank(tank);

    entities[name] = tank;
    entities[name]->position = btVector3(0, 2, 0);  // Set initial position
    entities[name]->scale = btVector3(1, 1, 1);     // Set initial scale
    tankCharacter = tank;
    btVector3 projectileScale(0.25f, 0.25f, 0.25f);
    btVector3 projectileColor(1.f, 1.f, 1.f);
    // Obtener la transformación actual del cañón
    btTransform canyonTransform;
    tank->canyon->getBody()->getMotionState()->getWorldTransform(canyonTransform);

    tank->chasis->getBody()->setActivationState(DISABLE_DEACTIVATION);  //disable the deactivation that bullet makes  
    tank->leftTrack->getBody()->setActivationState(DISABLE_DEACTIVATION);//for objects that are resting
    tank->rightTrack->getBody()->setActivationState(DISABLE_DEACTIVATION);

    // Calcular la posición inicial del proyectil, desplazada hacia adelante
    btVector3 projectilePosition = canyonTransform.getOrigin() + canyonTransform.getBasis() * btVector3(0, 30, -0.5f);
    for (size_t i = 0; i < 10; i++)
    {

        shared_ptr<Projectile> projectile = make_shared<Projectile>();
        graphics_system->add_ComponentSphere("projectile"+i, *projectile.get(), projectileScale, projectileColor);
        physics_system->add_ComponentSphere(*projectile.get(), projectilePosition, projectileScale, 1.0f);
        entities["projectile" + i] = projectile;
        entities["projectile" + i]->position = canyonPosition;  // Set initial position
        entities["projectile" + i]->scale = projectileScale;     // Set initial scale
        projectile->setActive(false);
        tank->projectiles.push_back(projectile);
    }
}

void Scene::run()
{
    do
    {
        // Read the user input:
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    running = false;
                    break;
                }

                case sf::Event::Resized:
                {
                    graphics_system->resetViewport(window);
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        tankCharacter->shootProjectile(graphics_system, physics_system, entities);
                        break;

                    }
                }
            }
        }
        // Handle input for tank control
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            btTransform chassisTransform;
            tankCharacter->chasis->getBody()->getMotionState()->getWorldTransform(chassisTransform);
            btMatrix3x3 chassisRotation = chassisTransform.getBasis();

            btVector3 forwardForce(0, 0, -10); // Reducir la fuerza aplicada
            btVector3 rotatedForwardForce = chassisRotation * forwardForce;
            tankCharacter->leftTrack->getBody()->applyCentralForce(rotatedForwardForce);
            tankCharacter->rightTrack->getBody()->applyCentralForce(rotatedForwardForce);

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            btTransform chassisTransform;
            tankCharacter->chasis->getBody()->getMotionState()->getWorldTransform(chassisTransform);
            btMatrix3x3 chassisRotation = chassisTransform.getBasis();

            btVector3 backwardForce(0, 0, 10);
            btVector3 rotatedBackwardForce = chassisRotation * backwardForce;
            tankCharacter->leftTrack->getBody()->applyCentralForce(rotatedBackwardForce);
            tankCharacter->rightTrack->getBody()->applyCentralForce(rotatedBackwardForce);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            btTransform chassisTransform;
            tankCharacter->chasis->getBody()->getMotionState()->getWorldTransform(chassisTransform);
            btMatrix3x3 chassisRotation = chassisTransform.getBasis();

            btVector3 leftBackwardForce(0, 0, 10);
            btVector3 rightForwardForce(0, 0, -10);
            btVector3 rotatedLeftBackwardForce = chassisRotation * leftBackwardForce;
            btVector3 rotatedRightForwardForce = chassisRotation * rightForwardForce;
            tankCharacter->leftTrack->getBody()->applyCentralForce(rotatedLeftBackwardForce);
            tankCharacter->rightTrack->getBody()->applyCentralForce(rotatedRightForwardForce);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            btTransform chassisTransform;
            tankCharacter->chasis->getBody()->getMotionState()->getWorldTransform(chassisTransform);
            btMatrix3x3 chassisRotation = chassisTransform.getBasis();

            btVector3 leftForwardForce(0, 0, -10);
            btVector3 rightBackwardForce(0, 0, 10);
            btVector3 rotatedLeftForwardForce = chassisRotation * leftForwardForce;
            btVector3 rotatedRightBackwardForce = chassisRotation * rightBackwardForce;
            tankCharacter->leftTrack->getBody()->applyCentralForce(rotatedLeftForwardForce);
            tankCharacter->rightTrack->getBody()->applyCentralForce(rotatedRightBackwardForce);
        }

        physics_system->stepSimulation(1.f / 60.f); 
        platform->movePlatform();
        contactListener->RunContacts(*physics_system->getDynamicsWorld());

        if (contactListener->openDoor)
        {

            contactListener->EnableDoor();
        }

        
        // Apply the physics transform to the graphics model:
        for (auto& pair : entities)
        {
                auto& entity = pair.second;
                if (entity->getBody())
                {
                    btTransform physics_transform;
                    entity->getBody()->getMotionState()->getWorldTransform(physics_transform);

                    glm::mat4 graphics_transform;
                    physics_transform.getOpenGLMatrix(glm::value_ptr(graphics_transform));

                    if (entity->get_Graphic_Model())
                    {
                        entity->get_Graphic_Model()->set_transformation(graphics_transform);
                        btVector3 scale = entity->scale;
                        entity->get_Graphic_Model()->scale(scale.getX(), scale.getY(), scale.getZ());
                    }
                }

        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Render the scene:
        graphics_system->render();

        window.display();
    } while (running);

    dynamicsWorld.reset();
}


void Scene::resetDynamicsWorld()
{
    dynamicsWorld.reset();
}
