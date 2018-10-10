
TEMPLATE = app
TARGET = Particle

QT += gui opengl

CONFIG += c++11

INCLUDEPATH += .

# Input
HEADERS += glwidget.h mainwindow.h \
    trianglemesh.h \
    plyreader.h \
    geometry.h \
    particle.h \
    animation.h
FORMS += mainwindow.ui
SOURCES += glwidget.cpp main.cpp mainwindow.cpp \
    trianglemesh.cpp \
    plyreader.cpp \
    geometry.cpp \
    particle.cpp \
    animation.cpp

DISTFILES += \
    shaders/simpleshader.vert \
    shaders/simpleshader.frag \
    shaders/particle.frag \
    shaders/particle.vert \
    shaders/basic.frag \
    shaders/basic.vert

RESOURCES += \
    resources.qrc
