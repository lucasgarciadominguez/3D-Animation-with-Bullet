/**********************************************************************
*Project           : Bullet3D Practice
*
*Author : Lucas García
*
*
*Purpose : Physics Practice using Bullet that moves a tank and other features
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

using namespace std;
using namespace glt;

Graphics_3D_System::Graphics_3D_System() {
    sceneGraph = std::make_shared<glt::Render_Node>();
}



void Graphics_3D_System::initialize(const sf::Window& window) {
    if (!glt::initialize_opengl_extensions())
    {
        exit(-1);
    }
    create_scene_basics_glt();
    configure_scene_basics_glt(*sceneGraph);
    resetViewport(window);

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
}

void Graphics_3D_System::create_scene_basics_glt()
{
    shared_ptr< Camera      > camera(new Camera(20.f, 1.f, 50.f, 1.f));
    shared_ptr< Light       > light(new Light);

    sceneGraph->add("camera", camera);
    sceneGraph->add("light", light);
}

void Graphics_3D_System::add_Component(const std::string& name,Entity& entity, btVector3 scaleObject, btVector3 color)
{
    auto newModel = make_shared<Model>();

    auto default_material = Material::default_material();
    std::shared_ptr<Shader_Program> shader_program_shared = Material::default_material()->get_shader_program_pointer();

    auto material = std::make_shared<Material>("instance_"+name, shader_program_shared);
    material->set("material_color", Vector3(color.getX(), color.getY(), color.getZ()));

    newModel->add(shared_ptr< Drawable >(new Cube), material);

    newModel->scale(scaleObject.getX(), scaleObject.getY(), scaleObject.getZ());

    sceneGraph->add(name, newModel);
    auto graphicComponent = std::make_shared<Graphic_Component>(newModel);
    entity.addGraphicComponent(graphicComponent);

}
void Graphics_3D_System::add_ComponentKey(const std::string& name, Entity& entity, btVector3 scaleObject, btVector3 color)
{
    auto newModel = make_shared<Model_Obj>("../../assets/key.obj");

    auto default_material = Material::default_material();
    std::shared_ptr<Shader_Program> shader_program_shared = Material::default_material()->get_shader_program_pointer();

    auto material = std::make_shared<Material>("instance_" + name, shader_program_shared);
    material->set("material_color", Vector3(color.getX(), color.getY(), color.getZ()));

    newModel->scale(scaleObject.getX(), scaleObject.getY(), scaleObject.getZ());

    sceneGraph->add(name, newModel);
    auto graphicComponent = std::make_shared<Graphic_Component>(newModel);
    entity.addGraphicComponent(graphicComponent);

}
void Graphics_3D_System::add_ComponentSphere(const std::string& name, Entity& entity, btVector3 scaleObject, btVector3 color)
{
    auto newModel = make_shared<Model_Obj>("../../assets/sphere.obj"); // Cargar la esfera desde el archivo OBJ

    auto default_material = Material::default_material();
    std::shared_ptr<Shader_Program> shader_program_shared = Material::default_material()->get_shader_program_pointer();

    auto material = std::make_shared<Material>("instance_" + name, shader_program_shared);
    material->set("material_color", Vector3(color.getX(), color.getY(), color.getZ()));

    newModel->scale(scaleObject.getX(), scaleObject.getY(), scaleObject.getZ());

    sceneGraph->add(name, newModel);
    auto graphicComponent = std::make_shared<Graphic_Component>(newModel);
    entity.addGraphicComponent(graphicComponent);
}



void Graphics_3D_System::render() {
    sceneGraph->render();
}

void Graphics_3D_System::resetViewport(const sf::Window& window)
{
    GLsizei width = GLsizei(window.getSize().x);
    GLsizei height = GLsizei(window.getSize().y);
    sceneGraph->get_active_camera()->set_aspect_ratio(float(width) / height);
    glViewport(0, 0, width, height);
}

void Graphics_3D_System::configure_scene_basics_glt(glt:: Render_Node& scene)
{
    scene["light"]->translate(Vector3(10.f, 10.f, 10.f));
    scene["camera"]->translate(Vector3(10.f, 45.f, 5.f));
    scene["camera"]->rotate_around_x(-1.5f);
}

std::shared_ptr<glt::Render_Node> Graphics_3D_System::getSceneGraph() const {
    return sceneGraph;
}
