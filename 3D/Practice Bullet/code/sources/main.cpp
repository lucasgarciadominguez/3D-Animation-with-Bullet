/*
    Bullet Continuous Collision Detection and Physics Library
    Copyright (c) 2003-2007 Erwin Coumans  http://continuousphysics.com/Bullet/

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the use of this software.
    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it freely,
    subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
       If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.

    This is the official 'Hello, World' example cleaned up for additional simplicity and clarity.
*/

#include <memory>
#include <vector>
#include <iostream>

#include <Cube.hpp>
#include <Light.hpp>
#include <Model.hpp>
#include <OpenGL.hpp>
#include <Model_Obj.hpp>
#include <Render_Node.hpp>

#include <SFML/Window.hpp>
#include <btBulletDynamicsCommon.h>

using namespace std;
using namespace glt;

namespace
{

    shared_ptr< Render_Node > create_scene ()
    {
        // Se crean los elementos (nodos) y la escena a la que se añadirán:

        shared_ptr< Render_Node > scene (new Render_Node);
        shared_ptr< Model       > model (new Model);
        shared_ptr< Camera      > camera(new Camera(20.f, 1.f, 50.f, 1.f));
        shared_ptr< Light       > light (new Light);

        model->add (shared_ptr< Drawable >(new Cube), Material::default_material ());

        // Se añaden los nodos a la escena:

        scene->add ("cube",   model );
        scene->add ("camera", camera);
        scene->add ("light" , light );

        return scene;
    }

    void configure_scene (Render_Node & scene)
    {
        scene["light" ]->translate (Vector3(10.f, 10.f, 10.f));
        scene["camera"]->translate (Vector3( 0.f,  0.f,  5.f));
    }

    void reset_viewport (const sf::Window & window, Render_Node & scene)
    {
        GLsizei width  = GLsizei(window.getSize ().x);
        GLsizei height = GLsizei(window.getSize ().y);

        scene.get_active_camera ()->set_aspect_ratio (float(width) / height);

        glViewport (0, 0, width, height);
    }

}

int main ()
{
    /* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- +\
                                       PHYSISCS WORLD INITIALIZATION
    \+ -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */

    // Collision configuration contains default setup for memory, collision setup.
    // Advanced users can create their own configuration.

    btDefaultCollisionConfiguration collisionConfiguration;

    // Use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded).

    btCollisionDispatcher collisionDispatcher(&collisionConfiguration);

    // btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.

    btDbvtBroadphase overlappingPairCache;

    // The default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded).

    btSequentialImpulseConstraintSolver constraintSolver;

    // Create and configure the physiscs world:

    shared_ptr< btDiscreteDynamicsWorld > dynamicsWorld
    (
        new btDiscreteDynamicsWorld
        (
            &collisionDispatcher,
            &overlappingPairCache,
            &constraintSolver,
            &collisionConfiguration
        )
    );

    dynamicsWorld->setGravity (btVector3(0, -10, 0));

    /* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- +\
                                            RIGID BODIES SETUP
    \+ -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */

    // Keep track of the shapes, states and rigid bodies.
    // Make sure to reuse collision shapes among rigid bodies whenever possible!

    vector< shared_ptr< btRigidBody          > > rigidBodies;
    vector< shared_ptr< btDefaultMotionState > > motionStates;
    vector< shared_ptr< btCollisionShape     > > collisionShapes;

    // GROUND
    {
        shared_ptr< btCollisionShape > shape(new btBoxShape(btVector3(1, 1, 1)));

        btTransform transform;
        transform.setIdentity ();
        transform.setOrigin (btVector3(0, -4, 0));

        // Using motionstate is optional for static objects.

        shared_ptr< btDefaultMotionState >       state(new btDefaultMotionState(transform));
        btRigidBody::btRigidBodyConstructionInfo info (0, state.get (), shape.get ());
        shared_ptr< btRigidBody >                body (new btRigidBody(info));

        body->setRestitution (0.7f);

        // Add the body to the dynamics world.

        dynamicsWorld->addRigidBody (body.get ());

        // Save the smart pointers for automatic cleanup.

        rigidBodies    .push_back (body );
        motionStates   .push_back (state);
        collisionShapes.push_back (shape);
    }

    // DYNAMIC OBJECT

    shared_ptr< btRigidBody > cube_body;

    {
        shared_ptr< btCollisionShape > shape(new btBoxShape(btVector3(1, 1, 1)));

        // Create Dynamic Objects.

        btTransform transform;
        transform.setIdentity ();
        transform.setOrigin (btVector3(0, 1, 0));

        btScalar  mass = 1.f;
        btVector3 localInertia(0, 0, 0);

        shape->calculateLocalInertia (mass, localInertia);

        // Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

        shared_ptr< btDefaultMotionState >       state(new btDefaultMotionState(transform));
        btRigidBody::btRigidBodyConstructionInfo info (mass, state.get (), shape.get (), localInertia);
        
        cube_body.reset (new btRigidBody(info));

        cube_body->setRestitution (1.0);

        dynamicsWorld->addRigidBody (cube_body.get ());

        // Save the smart pointers for automatic cleanup.

        rigidBodies    .push_back (cube_body);
        motionStates   .push_back (state);
        collisionShapes.push_back (shape);
    }
    
    // CONSTRAINT

    shared_ptr< btHingeConstraint > constraint;

    {
        constraint = make_shared< btHingeConstraint > (*cube_body, btVector3{ 0, 0, 0 }, btVector3{ 0, 0, 1 });

        constraint->enableAngularMotor (true, 2, 10);

        dynamicsWorld->addConstraint (constraint.get ());
    }
    /* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- +\
                                                SIMULATION
    \+ -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */

    // Se crea la ventana y el contexto de OpenGL asociado a ella:

    sf::Window window
    (
        sf::VideoMode(1024, 720),
        "Bullet Constraints",
        sf::Style::Default,
        sf::ContextSettings(24, 0, 0, 3, 2, sf::ContextSettings::Core)      // Se usa OpenGL 3.2 core profile
    );

    // Se determinan las características de OpenGL disponibles en la máquina:

	if (!glt::initialize_opengl_extensions())
	{
		exit (-1);
	}

    // Se activa la sincronización vertical:

    window.setVerticalSyncEnabled (true);

    // Se crea y se configura la escena:

    shared_ptr< Render_Node > scene = create_scene ();

    configure_scene (*scene);

    Node * cube_model = scene->get ("cube");

    // Se inicializan algunos elementos de OpenGL:

    reset_viewport (window, *scene);

    glClearColor (0.2f, 0.2f, 0.2f, 1.f);

    bool running = true;
    int  frame   = 0;

    do
    {
        // Read the user input:

        sf::Event event;

        while (window.pollEvent (event))
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
                    reset_viewport (window, *scene);
                    break;
                }
            }
        }

        // Perform the simulation:

        dynamicsWorld->stepSimulation (1.f / 60.f);

        // Apply the physics transform to the graphics model:

        btTransform physics_transform;

        cube_body->getMotionState ()->getWorldTransform (physics_transform);

        glm::mat4 graphics_transform;

        physics_transform.getOpenGLMatrix (glm::value_ptr(graphics_transform));

        cube_model->set_transformation (graphics_transform);

        cube_model->scale (1.f);

        // Render the scene:

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->render ();

        window.display ();
    }
    while (running);

    // Se debe destruir el mundo físico antes de que se destruyan los cuerpos rígidos:

    dynamicsWorld.reset ();

    return EXIT_SUCCESS;
}
