/**********************************************************************
*Project           : Bullet3D Practice
*
*Author : Lucas García
*
*
*Purpose : Physics Practice using Bullet that moves a tank and other features
*
**********************************************************************/

#include <memory>
#include <vector>
#include <iostream>
#include <SFML/Window.hpp>
#include <btBulletDynamicsCommon.h>

#include "Scene.h"
#include "Entity.h"
#include <Cube.hpp>
#include <Light.hpp>
#include <Model.hpp>
#include <Platform.h>
#include <OpenGL.hpp>
#include <Model_Obj.hpp>
#include <Render_Node.hpp>
#include "Tank.h"

using namespace std;
using namespace glt;

int main ()
{
    shared_ptr< Scene > newScene= make_shared<Scene>();

    shared_ptr < Entity> ground= make_shared<Entity>();
    btVector3 position(-10, -2, 0);
    btVector3 scale (15.f, 1.f, 25.f);
    btVector3 colorGround (.75f, .75f, .75f);
    newScene->addEntity("ground", ground, btVector3(position.getX(), position.getY(), position.getZ()), btVector3(scale.getX(), scale.getY(), scale.getZ()),
        0.f,scale, colorGround);

    shared_ptr < Entity> ground2 = make_shared<Entity>();
    btVector3 positionGround2(30, -2, 0);
    newScene->addEntity("ground2", ground2, btVector3(positionGround2.getX(), positionGround2.getY(), positionGround2.getZ()), btVector3(scale.getX(), scale.getY(), scale.getZ()),
        0.f, scale, colorGround);

    shared_ptr < Entity> wall = make_shared<Entity>();
    btVector3 positionWall(16, 3, 15);
    btVector3 scaleWall(1.f, 5.f, 10.f);
    btVector3 colorWall(0.541f, 0.518f, 0.435f);

    newScene->addEntity("wall", wall, btVector3(positionWall.getX(), positionWall.getY(), positionWall.getZ()), btVector3(scaleWall.getX(), scaleWall.getY(), scaleWall.getZ()),
        0.f, scaleWall, colorWall);
    wall->getBody()->setRestitution(0);

    shared_ptr < Entity> wall2 = make_shared<Entity>();
    btVector3 positionWall2(16, 3, -15);

    newScene->addEntity("wall2", wall2, btVector3(positionWall2.getX(), positionWall2.getY(), positionWall2.getZ()), btVector3(scaleWall.getX(), scaleWall.getY(), scaleWall.getZ()),
        0.f, scaleWall, colorWall);
    wall2->getBody()->setRestitution(0);

    shared_ptr < Entity> door = make_shared<Entity>();
    btVector3 positionDoor(16, 3, 0);
    btVector3 scaleDoor(1.f, 5.f, 4.5f);
    btVector3 colorDoor(0.85f, .85f, .85f);

    newScene->addDoor("door", door, btVector3(positionDoor.getX(), positionDoor.getY(), positionDoor.getZ()), btVector3(scaleDoor.getX(), scaleDoor.getY(), scaleDoor.getZ()),
        0.f, scaleDoor, colorDoor);

    // Create dynamic cube entity
    shared_ptr < Entity> cube= make_shared<Entity>();
    btVector3 positionColumn(40, 0, -20);

    btVector3 scaleCube(1.f, 5.f, 1.f);
    btVector3 colorCube(.216f, .541f, .243f);
    newScene->addEntity("cube", cube, btVector3(positionColumn.getX(), positionColumn.getY(), positionColumn.getZ()), btVector3(scaleCube.getX(), scaleCube.getY(), scaleCube.getZ()),
        1.f, scaleCube, colorCube);

    shared_ptr < Entity> cube2 = make_shared<Entity>();

    btVector3 positionColumn2(40, 0, 20);
    newScene->addEntity("cube2", cube2, btVector3(positionColumn2.getX(), positionColumn2.getY(), positionColumn2.getZ()), btVector3(scaleCube.getX(), scaleCube.getY(), scaleCube.getZ()),
        1.f, scaleCube, colorCube);

    shared_ptr < Entity> cube3 = make_shared<Entity>();

    btVector3 positionColumn3(20, 0, 20);
    newScene->addEntity("cube3", cube3, btVector3(positionColumn3.getX(), positionColumn3.getY(), positionColumn3.getZ()), btVector3(scaleCube.getX(), scaleCube.getY(), scaleCube.getZ()),
        1.f, scaleCube, colorCube);

    shared_ptr < Entity> cube4 = make_shared<Entity>();

    btVector3 positionColumn4(20, 0, -20);
    newScene->addEntity("cube4", cube4, btVector3(positionColumn4.getX(), positionColumn4.getY(), positionColumn4.getZ()), btVector3(scaleCube.getX(), scaleCube.getY(), scaleCube.getZ()),
        1.f, scaleCube, colorCube);

    shared_ptr < Platform> platform = make_shared<Platform>();
    btVector3 positionPlatform(10, -1.f, -20);
    btVector3 scalePlatform(5.f, 0.f, 5.f);
    btVector3 colorPlatform(0.85f, .85f, .85f);
    newScene->addPlatform("platform", platform, btVector3(positionPlatform.getX(), positionPlatform.getY(), positionPlatform.getZ()), btVector3(scalePlatform.getX(), scalePlatform.getY(), scalePlatform.getZ()),
        0.f, scalePlatform, colorPlatform);

    shared_ptr < Entity> key = make_shared<Entity>();
    btVector3 positionKey(-6, 2, -12);

    btVector3 scaleKey(1.f, 1.f, 1.f);
    btVector3 colorKey(0.5f, .5f, .5f);

    newScene->addKey("key", key, btVector3(positionKey.getX(), positionKey.getY(), positionKey.getZ()), btVector3(scaleKey.getX(), scaleKey.getY(), scaleKey.getZ()),
        1.f, scaleKey, colorKey);

    shared_ptr<Tank> tank = make_shared<Tank>();
    newScene->addTank("tank", tank);
    newScene->run();

    return EXIT_SUCCESS;
}
