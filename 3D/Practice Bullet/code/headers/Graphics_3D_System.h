/**********************************************************************
*Project           : Bullet3D Practice
*
*Author : Lucas García
*
*
*Purpose : Physics Practice using Bullet that moves a tank and other features
*
**********************************************************************/

#pragma once

#include <SFML/Window.hpp>
#include <memory>
#include "Render_Node.hpp"
#include "Scene.h"

class Entity;

class Graphics_3D_System {
private:
    std::shared_ptr<glt::Render_Node> sceneGraph;
public:

    Graphics_3D_System();

    ~Graphics_3D_System() = default;

    void initialize(const sf::Window& window);
    void create_scene_basics_glt();
    void add_Component(const std::string& name, Entity& entity, btVector3 scale, btVector3 color);
    void add_ComponentKey(const std::string& name, Entity& entity, btVector3 scaleObject, btVector3 color);
    void add_ComponentSphere(const std::string& name, Entity& entity, btVector3 scaleObject, btVector3 color);
    void render();
    void resetViewport(const sf::Window& window);
    void configure_scene_basics_glt(glt::Render_Node& scene);
    std::shared_ptr<glt::Render_Node> getSceneGraph() const;
    std::shared_ptr < glt:: Shader_Program > shader_program_shared; // Utiliza un shared_ptr para mantener una referencia al Shader_Program

};