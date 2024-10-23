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

/**
 * Scene constructor.
 * Initializes the window, graphics, physics systems, and the contact listener.
 */
Scene::Scene()
{
    // Create the window with OpenGL 3.2 core profile
    window.create(
        sf::VideoMode(1024, 720),  // Window size: 1024x720
        "Bullet Constraints",      // Window title
        sf::Style::Default,        // Default window style
        sf::ContextSettings(24, 0, 0, 3, 2, sf::ContextSettings::Core)  // OpenGL 3.2 core profile
    );

    // Initialize the graphics and physics systems
    graphics_system = std::make_shared<Graphics_3D_System>();
    physics_system = std::make_shared<Physics_3D_System>();

    // Initialize the contact listener for handling collisions
    contactListener = std::make_shared<ContactListener>();

    // Enable vertical synchronization (VSync)
    window.setVerticalSyncEnabled(true);

    // Initialize the graphics system with the window context
    graphics_system->initialize(window);
}

/**
 * Scene destructor.
 * Cleans up resources if necessary (handled automatically by smart pointers).
 */
Scene::~Scene() {
    // Cleanup if necessary (handled automatically by smart pointers)
}

/**
 * Adds a generic entity to the scene.
 * @param name Name of the entity.
 * @param entity Shared pointer to the entity.
 * @param origin The initial position of the entity.
 * @param shapeSize The size of the entity's collision shape.
 * @param mass The mass of the entity.
 * @param scale The scale of the entity's graphical representation.
 * @param color The color of the entity.
 */
void Scene::addEntity(const std::string& name, std::shared_ptr<Entity> entity,
    const btVector3& origin, const btVector3& shapeSize,
    btScalar mass, btVector3 scale, btVector3 color)
{
    // Add graphical and physical components to the entity
    graphics_system->add_Component(name, *entity, scale, color);
    physics_system->add_Component(*entity, origin, shapeSize, mass);

    // Store the entity in the entities map with its name as the key
    entities[name] = entity;
    entities[name]->position = origin;
    entities[name]->scale = scale;
}

/**
 * Adds a door to the scene.
 * @param name Name of the door.
 * @param entity Shared pointer to the door entity.
 * @param origin The initial position of the door.
 * @param shapeSize The size of the door's collision shape.
 * @param mass The mass of the door.
 * @param scale The scale of the door's graphical representation.
 * @param color The color of the door.
 */
void Scene::addDoor(const std::string& name, std::shared_ptr<Entity> entity,
    const btVector3& origin, const btVector3& shapeSize,
    btScalar mass, btVector3 scale, btVector3 color)
{
    // Add graphical and physical components to the door
    graphics_system->add_Component(name, *entity, scale, color);
    physics_system->add_Component(*entity, origin, shapeSize, mass);

    // Store the door in the entities map with its name as the key
    entities[name] = entity;
    entities[name]->position = origin;
    entities[name]->scale = scale;

    // Register the door with the contact listener for collision handling
    contactListener->addDoor(entity);
}

/**
 * Adds a key to the scene.
 * @param name Name of the key.
 * @param entity Shared pointer to the key entity.
 * @param origin The initial position of the key.
 * @param shapeSize The size of the key's collision shape.
 * @param mass The mass of the key.
 * @param scale The scale of the key's graphical representation.
 * @param color The color of the key.
 */
void Scene::addKey(const std::string& name, std::shared_ptr<Entity> entity,
    const btVector3& origin, const btVector3& shapeSize,
    btScalar mass, btVector3 scale, btVector3 color)
{
    // Add graphical component specific to the key
    graphics_system->add_ComponentKey(name, *entity, scale, color);

    // Add a sensor physics component for the key (using a different scale)
    const btVector3 scaleKey(0.25f, 1.0f, 2.0f);  // Specific scale for key collision shape
    physics_system->add_ComponentSensor(*entity, origin, scaleKey, mass);

    // Store the key in the entities map and set its position and scale
    entities[name] = entity;
    entities[name]->position = origin;
    entities[name]->scale = scale;

    // Store the key in the key variable and register it with the contact listener
    key = entity;
    contactListener->addKey(entity);
}

/**
 * Adds a platform to the scene.
 * @param name Name of the platform.
 * @param entity Shared pointer to the platform entity.
 * @param origin The initial position of the platform.
 * @param shapeSize The size of the platform's collision shape.
 * @param mass The mass of the platform.
 * @param scale The scale of the platform's graphical representation.
 * @param color The color of the platform.
 */
void Scene::addPlatform(const std::string& name, std::shared_ptr<Platform> entity,
    const btVector3& origin, const btVector3& shapeSize,
    btScalar mass, btVector3 scale, btVector3 color)
{
    // Add graphical and physical components to the platform
    graphics_system->add_Component(name, *entity, scale, color);
    physics_system->add_Component(*entity, origin, shapeSize, mass);

    // Store the platform in the entities map and set its position and scale
    entities[name] = entity;
    entities[name]->position = origin;
    entities[name]->scale = scale;

    // Set the platform for later reference
    platform = entity;
}

/**
 * Method for creating a tank and all its components.
 * Creates the chasis, the turret, the canyon...
 */
void Scene::addTank(const std::string& name, std::shared_ptr<Tank> tank)
{
    // Set mass for chassis and tracks (adjust as necessary)
    btScalar chasisMass = 1.0f;
    btScalar trackMass = 1.0f;

    // Define positions, scales, and colors for tank components
    btVector3 leftTrackPosition(-1.2f, 0.0f, 0.0f);
    btVector3 rightTrackPosition(1.2f, 0.0f, 0.0f);
    const btVector3 trackScale(0.25f, 0.25f, 1.0f);
    btVector3 trackColor(0.216f, 0.541f, 0.243f);

    // Left track: Add graphical and physical components
    graphics_system->add_Component("leftTrack", *tank->leftTrack, trackScale, trackColor);
    physics_system->add_ComponentSensor(*tank->leftTrack, leftTrackPosition, trackScale, trackMass);
    entities["leftTrack"] = tank->leftTrack;
    entities["leftTrack"]->position = leftTrackPosition;
    entities["leftTrack"]->scale = trackScale;

    // Right track: Add graphical and physical components
    graphics_system->add_Component("rightTrack", *tank->rightTrack, trackScale, trackColor);
    physics_system->add_ComponentSensor(*tank->rightTrack, rightTrackPosition, trackScale, trackMass);
    entities["rightTrack"] = tank->rightTrack;
    entities["rightTrack"]->position = rightTrackPosition;
    entities["rightTrack"]->scale = trackScale;

    // Chassis: Set position, scale, and color
    btVector3 chasisPosition(0.0f, 0.0f, 0.0f);
    btVector3 chasisScale(1.0f, 0.2f, 1.0f);
    btVector3 chasisColor(0.36f, 0.541f, 0.243f);
    graphics_system->add_Component("chassis", *tank->chasis, chasisScale, chasisColor);
    physics_system->add_ComponentSensor(*tank->chasis, chasisPosition, chasisScale, chasisMass);
    entities["chassis"] = tank->chasis;
    entities["chassis"]->position = chasisPosition;
    entities["chassis"]->scale = chasisScale;

    // Turret: Set position, scale, and color
    btVector3 turretPosition(0.0f, 0.55f, 0.0f);
    btVector3 turretScale(0.5f, 0.2f, 0.35f);
    btVector3 turretColor(0.255f, 0.529f, 0.278f);
    graphics_system->add_Component("turret", *tank->turret, turretScale, turretColor);
    physics_system->add_Component(*tank->turret, turretPosition, turretScale, 1.0f);
    entities["turret"] = tank->turret;
    entities["turret"]->position = turretPosition;
    entities["turret"]->scale = turretScale;

    // Cannon: Set position, scale, and color
    btVector3 canyonPosition(0.0f, 0.55f, -2.0f);
    btVector3 canyonScale(0.2f, 0.1f, 0.5f);
    btVector3 canyonColor(0.255f, 0.529f, 0.378f);
    graphics_system->add_Component("canyon", *tank->canyon, canyonScale, canyonColor);
    physics_system->add_Component(*tank->canyon, canyonPosition, canyonScale, 1.0f);
    entities["canyon"] = tank->canyon;
    entities["canyon"]->position = canyonPosition;
    entities["canyon"]->scale = canyonScale;

    // Add constraints for left and right tracks, turret, and cannon

    // Left track constraint (hinge)
    tank->leftTrackConstraint = std::make_shared<btHingeConstraint>(
        *tank->chasis->getBody(), *tank->leftTrack->getBody(),
        btVector3(-1.2f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f),
        btVector3(0, 1, 0), btVector3(0, 1, 0), false);

    // Right track constraint (hinge)
    tank->rightTrackConstraint = std::make_shared<btHingeConstraint>(
        *tank->chasis->getBody(), *tank->rightTrack->getBody(),
        btVector3(1.2f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f),
        btVector3(0, 1, 0), btVector3(0, 1, 0), false);

    // Turret constraint (fixed)
    btTransform frameInA, frameInB;
    frameInA = btTransform::getIdentity();
    frameInA.setOrigin(btVector3(0.0f, 0.50f, 0.0f));  // Turret position relative to chassis
    frameInB = btTransform::getIdentity();
    tank->turretConstraint = std::make_shared<btFixedConstraint>(
        *tank->chasis->getBody(), *tank->turret->getBody(), frameInA, frameInB);

    // Cannon constraint (fixed)
    btTransform cannonFrameInA, cannonFrameInB;
    cannonFrameInA = btTransform::getIdentity();
    cannonFrameInA.setOrigin(btVector3(0.0f, 0.0f, -0.85f));  // Cannon position relative to turret
    cannonFrameInB = btTransform::getIdentity();
    tank->canyonConstraint = std::make_shared<btFixedConstraint>(
        *tank->turret->getBody(), *tank->canyon->getBody(), cannonFrameInA, cannonFrameInB);

    // Add constraints to the dynamics world
    physics_system->getDynamicsWorld()->addConstraint(tank->turretConstraint.get());
    physics_system->getDynamicsWorld()->addConstraint(tank->canyonConstraint.get());
    physics_system->getDynamicsWorld()->addConstraint(tank->leftTrackConstraint.get());
    physics_system->getDynamicsWorld()->addConstraint(tank->rightTrackConstraint.get());

    // Register the tank with the contact listener
    contactListener->addTank(tank);

    // Set initial position and scale for the entire tank
    entities[name] = tank;
    entities[name]->position = btVector3(0, 2, 0);  // Set initial position
    entities[name]->scale = btVector3(1, 1, 1);     // Set initial scale
    tankCharacter = tank;

    // Set up projectiles for the tank's cannon
    btVector3 projectileScale(0.25f, 0.25f, 0.25f);
    btVector3 projectileColor(1.0f, 1.0f, 1.0f);

    // Get current cannon transformation
    btTransform canyonTransform;
    tank->canyon->getBody()->getMotionState()->getWorldTransform(canyonTransform);

    // Disable deactivation for tank components (keep them active)
    tank->chasis->getBody()->setActivationState(DISABLE_DEACTIVATION);
    tank->leftTrack->getBody()->setActivationState(DISABLE_DEACTIVATION);
    tank->rightTrack->getBody()->setActivationState(DISABLE_DEACTIVATION);

    // Calculate initial projectile position, displaced forward
    btVector3 projectilePosition = canyonTransform.getOrigin() + canyonTransform.getBasis() * btVector3(0, 30, -0.5f);

    // Initialize projectiles
    for (size_t i = 0; i < 10; i++) {
        shared_ptr<Projectile> projectile = make_shared<Projectile>();
        graphics_system->add_ComponentSphere("projectile" + std::to_string(i), *projectile.get(), projectileScale, projectileColor);
        physics_system->add_ComponentSphere(*projectile.get(), projectilePosition, projectileScale, 1.0f);
        entities["projectile" + std::to_string(i)] = projectile;
        entities["projectile" + std::to_string(i)]->position = canyonPosition;
        entities["projectile" + std::to_string(i)]->scale = projectileScale;
        projectile->setActive(false);  // Set initial state of projectile to inactive
        tank->projectiles.push_back(projectile);
    }
}


/**
 * Main loop of the scene.
 * Handles user input, updates the physics simulation, and renders the scene.
 */
void Scene::run()
{
    do
    {
        sf::Event event;

        // Process user input events
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                running = false;
                break;

            case sf::Event::Resized:
                graphics_system->resetViewport(window);
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Space)
                {
                    // Tank shoots a projectile when space is pressed
                    tankCharacter->shootProjectile(graphics_system, physics_system, entities);
                }
                break;
            }
        }

        // Handle continuous input for tank control
        handleTankMovement();

        // Step the physics simulation
        physics_system->stepSimulation(1.f / 60.f);

        // Move platform and handle contacts
        platform->movePlatform();
        contactListener->RunContacts(*physics_system->getDynamicsWorld());

        // Check if the door should be opened
        if (contactListener->openDoor)
        {
            contactListener->EnableDoor();
        }

        // Apply the updated physics transforms to the graphics entities
        updateGraphicsTransforms();

        // Clear the screen and render the scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        graphics_system->render();
        window.display();

    } while (running);

    // Clean up the dynamics world
    dynamicsWorld.reset();
}

/**
 * Handles tank movement based on keyboard input.
 * Controls the tank using WASD keys for forward, backward, and turning motions.
 */
void Scene::handleTankMovement()
{
    btTransform chassisTransform;
    tankCharacter->chasis->getBody()->getMotionState()->getWorldTransform(chassisTransform);
    btMatrix3x3 chassisRotation = chassisTransform.getBasis();

    const btVector3 forwardForce(0, 0, -10);  // Force applied forward
    const btVector3 backwardForce(0, 0, 10);  // Force applied backward

    // Move forward with 'W' key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        applyTankForce(forwardForce, chassisRotation);
    }

    // Move backward with 'S' key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        applyTankForce(backwardForce, chassisRotation);
    }

    // Turn left with 'A' key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        const btVector3 leftBackwardForce(0, 0, 10);
        const btVector3 rightForwardForce(0, 0, -10);
        applyTankTurningForce(leftBackwardForce, rightForwardForce, chassisRotation);
    }

    // Turn right with 'D' key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        const btVector3 leftForwardForce(0, 0, -10);
        const btVector3 rightBackwardForce(0, 0, 10);
        applyTankTurningForce(leftForwardForce, rightBackwardForce, chassisRotation);
    }
}

/**
 * Applies central force to both tank tracks for forward or backward movement.
 * @param force The force vector to apply.
 * @param rotation The current rotation of the tank chassis.
 */
void Scene::applyTankForce(const btVector3& force, const btMatrix3x3& rotation)
{
    btVector3 rotatedForce = rotation * force;
    tankCharacter->leftTrack->getBody()->applyCentralForce(rotatedForce);
    tankCharacter->rightTrack->getBody()->applyCentralForce(rotatedForce);
}

/**
 * Applies turning force to the left and right tracks of the tank.
 * @param leftForce The force applied to the left track.
 * @param rightForce The force applied to the right track.
 * @param rotation The current rotation of the tank chassis.
 */
void Scene::applyTankTurningForce(const btVector3& leftForce, const btVector3& rightForce, const btMatrix3x3& rotation)
{
    btVector3 rotatedLeftForce = rotation * leftForce;
    btVector3 rotatedRightForce = rotation * rightForce;

    tankCharacter->leftTrack->getBody()->applyCentralForce(rotatedLeftForce);
    tankCharacter->rightTrack->getBody()->applyCentralForce(rotatedRightForce);
}

/**
 * Updates the graphics transformations based on the current physics state.
 * Applies the physics transformation to the corresponding graphical models.
 */
void Scene::updateGraphicsTransforms()
{
    for (auto& pair : entities)
    {
        auto& entity = pair.second;

        // Only update entities with a valid physics body
        if (entity->getBody())
        {
            btTransform physicsTransform;
            entity->getBody()->getMotionState()->getWorldTransform(physicsTransform);

            glm::mat4 graphicsTransform;
            physicsTransform.getOpenGLMatrix(glm::value_ptr(graphicsTransform));

            // Update the graphical model's transformation
            if (entity->get_Graphic_Model())
            {
                entity->get_Graphic_Model()->set_transformation(graphicsTransform);
                btVector3 scale = entity->scale;
                entity->get_Graphic_Model()->scale(scale.getX(), scale.getY(), scale.getZ());
            }
        }
    }
}


void Scene::resetDynamicsWorld()
{
    dynamicsWorld.reset();
}
