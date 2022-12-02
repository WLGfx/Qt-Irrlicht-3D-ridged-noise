#include "irrcube.h"

#include "irrwidget.h"

using namespace irr;

IrrCube::IrrCube(QObject *parent)
    : QObject{parent}
{
    IrrWidget *widget = (IrrWidget*)parent;
    device = widget->getDevice();
    smgr = device->getSceneManager();

    //cube = smgr->addCubeSceneNode(20.0f);

    camera = smgr->addCameraSceneNode();
    camera->setPosition(irr::core::vector3df(1, 1, -5));
    camera->setTarget(irr::core::vector3df(0, 0, 0));
    camera->updateAbsolutePosition();

    scene::ISceneNodeAnimator* anim = 0;
    anim = smgr->createFlyCircleAnimator (core::vector3df(0,10,0),80.0f, 0.0005f);
    camera->addAnimator(anim);
    anim->drop();

    light = smgr->addLightSceneNode(0, core::vector3df(0, 40, -40),
                                    video::SColorf(1.0f, 1.0f, 1.0f),
                                    20.0f);
}

void IrrCube::update() {
    core::dimension2du size = device->getVideoDriver()->getScreenSize();
    camera->setAspectRatio((float)size.Width / size.Height);

    core::vector3df pos = camera->getPosition();
    light->setPosition(pos);
}
