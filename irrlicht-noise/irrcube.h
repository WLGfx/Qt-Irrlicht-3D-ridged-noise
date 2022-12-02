#ifndef IRRCUBE_H
#define IRRCUBE_H

#include <QObject>

#include <irrlicht.h>



class IrrCube : public QObject
{
    Q_OBJECT
public:
    explicit IrrCube(QObject *parent = nullptr);

    void update();

private:
    irr::IrrlichtDevice *device;
    irr::scene::ISceneManager *smgr;

    irr::scene::IMeshSceneNode *cube;
    irr::scene::ICameraSceneNode *camera;
    irr::scene::ILightSceneNode *light;

signals:

};

#endif // IRRCUBE_H
