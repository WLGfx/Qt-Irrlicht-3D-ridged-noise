QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    irrcube.cpp \
    irrwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    noisemesh.cpp

HEADERS += \
    FastNoiseLite.h \
    Simplex.h \
    irrcube.h \
    irrwidget.h \
    mainwindow.h \
    noisemesh.h

FORMS += \
    mainwindow.ui

win32 {
LIBS += -LC:\Users\44776\Documents\MEGAsync\WLGfx-Software-2022\irrlicht-1.8.5\bin\Win32-gcc
INCLUDEPATH += C:\Users\44776\Documents\MEGAsync\WLGfx-Software-2022\irrlicht-1.8.5\include
} else {
#LIBS += -L/home/wlgfx/MEGAsync/WLGfx-Software-2022/irrlicht-1.8.5/lib/Linux
INCLUDEPATH += /home/wlgfx/MEGAsync/WLGfx-Software-2022/irrlicht-1.8.5/include
LIBS += -lX11 -lXext -lGL -lGLU -lXxf86vm -lpthread
}

LIBS += -lIrrlicht

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
