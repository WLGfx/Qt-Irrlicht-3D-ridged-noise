#include "irrwidget.h"

using namespace irr;

IrrWidget::IrrWidget(QObject *parent)
    : QObject{parent}
{
    irrTarget = (QWidget*)parent;
    initIrrWidget();

    noiseMesh = new NoiseMesh(device, 150, 3.0f, 0.9f);
}

IrrWidget::~IrrWidget() {
    delete cubeDefaultScene;
    device->closeDevice();
    device = 0;
}

void IrrWidget::initIrrWidget() {
    QSize size = irrTarget->size();

    irrParams.DriverType = video::EDT_OPENGL;
    irrParams.WindowId = (void*)irrTarget->winId();
    irrParams.WindowSize.set(size.width(), size.height());
    irrParams.HandleSRGB = true;
    irrParams.Vsync = true;
    irrParams.AntiAlias = true;
    irrParams.IgnoreInput = false;

    device = createDeviceEx(irrParams);
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();

    device->setResizable(true);

    //fog color-final values= amount of fog, distance
    driver->setFog(video::SColor(0,0,0,0),
                   video::EFT_FOG_LINEAR,
                   0,120);

    cubeDefaultScene = new IrrCube(this);
}

void IrrWidget::update() {
    cubeDefaultScene->update();
}

void IrrWidget::draw() {
    if (!device || !device->run()) return;

    QSize qsize = irrTarget->size();
    core::dimension2du isize(qsize.width(), qsize.height());
    driver->OnResize(isize);

    //driver->beginScene(true, true, video::SColor(255, 100, 100, 140));
    driver->beginScene(true, true, video::SColor(80, 0, 0, 0));
    smgr->drawAll();
    driver->endScene();
}

void IrrWidget::update_noise(int seed, int size, float pscale, float nscale, float cutoff) {
    noiseMesh->regenerate(seed, size, pscale, nscale, cutoff);
    device->setResizable(true);//irrTarget->size();
}
