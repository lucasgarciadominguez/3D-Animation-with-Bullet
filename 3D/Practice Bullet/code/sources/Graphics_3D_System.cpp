/**********************************************************************
* Project           : Bullet3D Practice
*
* Author            : Lucas García
*
* Purpose           : Physics Practice using Bullet that moves a tank
*                     and other features.
*
**********************************************************************/

#include "Graphics_3D_System.h"
#include "Scene.h"
#include <Light.hpp>
#include <Model.hpp>
#include <Cube.hpp>
#include "Entity.h"
#include <OpenGL.hpp>
#include <Model_Obj.hpp>
#include <Render_Node.hpp>
#include "Graphic_Component.h"
#include <Material.hpp>
#include "Shader_Program.hpp"
#include <iostream>

using namespace std;
using namespace glt;

/**
 * Constructor for the 3D Graphics System.
 * Initializes the scene graph.
 */
Graphics_3D_System::Graphics_3D_System() {
    sceneGraph = std::make_shared<glt::Render_Node>();
}

/**
 * Initialize the 3D Graphics System, setting up OpenGL and the scene basics.
 * @param window The window used for rendering.
 */
void Graphics_3D_System::initialize(const sf::Window& window) {
    if (!glt::initialize_opengl_extensions()) {
        std::cerr << "Error initializing OpenGL extensions." << std::endl;
        exit(-1); // Handle this more gracefully in production code
    }
    create_scene_basics_glt();
    configure_scene_basics_glt(*sceneGraph);
    resetViewport(window);

    // Set the background color for OpenGL
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
}

/**
 * Create basic components for the scene, including the camera and light.
 */
void Graphics_3D_System::create_scene_basics_glt() {
    auto camera = std::make_shared<Camera>(20.f, 1.f, 50.f, 1.f);
    auto light = std::make_shared<Light>();

    sceneGraph->add("camera", camera);
    sceneGraph->add("light", light);
}

/**
 * Helper function to simplify the creation of components.
 */
void Graphics_3D_System::addComponent(const std::string& name, Entity& entity, const std::string& modelPath, btVector3 scaleObject, btVector3 color) {
    
    auto newModel = modelPath.empty() ? std::make_shared<Model>() : std::make_shared<Model_Obj>(modelPath);

    //auto default_material = Material::default_material();
    //std::shared_ptr<Shader_Program> shader_program_shared = Material::default_material()->get_shader_program_pointer();

    //auto material = std::make_shared<Material>("instance_" + name, shader_program_shared);
    //material->set("material_color", Vector3(color.getX(), color.getY(), color.getZ()));
    std::shared_ptr<Shader_Program> shader_program_shared = Material::default_material()->get_shader_program_pointer();

    auto material = std::make_shared<Material>("instance_" + name, shader_program_shared);
    material->set("material_color", Vector3(color.getX(), color.getY(), color.getZ()));

    if (modelPath.empty()) {
        // Default to a cube if no model path is provided
        newModel->add(std::make_shared<Cube>(), material);
    }

    newModel->scale(scaleObject.getX(), scaleObject.getY(), scaleObject.getZ());
    sceneGraph->add(name, newModel);

    auto graphicComponent = std::make_shared<Graphic_Component>(newModel);
    entity.addGraphicComponent(graphicComponent);
}

/**
 * Add a cube component to the scene.
 */
void Graphics_3D_System::add_Component(const std::string& name, Entity& entity, btVector3 scaleObject, btVector3 color) {
    addComponent(name, entity, "", scaleObject, color);
}

/**
 * Add a key model component to the scene.
 */
void Graphics_3D_System::add_ComponentKey(const std::string& name, Entity& entity, btVector3 scaleObject, btVector3 color) {
    addComponent(name, entity, "../../assets/key.obj", scaleObject, color);
}

/**
 * Add a sphere model component to the scene.
 */
void Graphics_3D_System::add_ComponentSphere(const std::string& name, Entity& entity, btVector3 scaleObject, btVector3 color) {
    addComponent(name, entity, "../../assets/sphere.obj", scaleObject, color);
}

/**
 * Render the scene.
 */
void Graphics_3D_System::render() {
    sceneGraph->render();
}

/**
 * Reset the viewport based on the window size.
 */
void Graphics_3D_System::resetViewport(const sf::Window& window) {
    GLsizei width = static_cast<GLsizei>(window.getSize().x);
    GLsizei height = static_cast<GLsizei>(window.getSize().y);
    sceneGraph->get_active_camera()->set_aspect_ratio(static_cast<float>(width) / height);
    glViewport(0, 0, width, height);
}

/**
 * Configure the basic scene, positioning the camera and light.
 */
void Graphics_3D_System::configure_scene_basics_glt(glt::Render_Node& scene) {
    scene["light"]->translate(Vector3(10.f, 10.f, 10.f));
    scene["camera"]->translate(Vector3(10.f, 45.f, 5.f));
    scene["camera"]->rotate_around_x(-1.5f);
}

/**
 * Get the scene graph.
 * @return A shared pointer to the scene graph.
 */
std::shared_ptr<glt::Render_Node> Graphics_3D_System::getSceneGraph() const {
    return sceneGraph;
}