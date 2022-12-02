#ifndef IRRWIDGET_H
#define IRRWIDGET_H

#include <QObject>
#include <QWidget>

#include <irrlicht.h>

#include "irrcube.h"
#include "noisemesh.h"

class IrrWidget : public QObject
{
    Q_OBJECT
public:
    explicit IrrWidget(QObject *parent = nullptr);
    ~IrrWidget();

    void update();
    void draw();

    irr::IrrlichtDevice *getDevice() { return device; }
    void update_noise(int seed, int size, float pscale, float nscale, float cutoff);

private:
    QWidget *irrTarget;

    irr::SIrrlichtCreationParameters irrParams;
    irr::IrrlichtDevice *device = 0;
    irr::video::IVideoDriver *driver;
    irr::scene::ISceneManager *smgr;

    void initIrrWidget();

    IrrCube *cubeDefaultScene;

    NoiseMesh *noiseMesh;

signals:

};

#endif // IRRWIDGET_H
