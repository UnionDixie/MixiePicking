QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    src/openglwrapper/scene/scene.cpp \
    src/openglwrapper/window.cpp \
    src/openglwrapper/wrap/glwidget.cpp \
    src/picker/picker.cpp \
    src/main.cpp \
    src/openglwrapper/shader/shader.cpp \
    src/openglwrapper/object/object.cpp \
    src/openglwrapper/object/loader/loader.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/openglwrapper/scene/scene.h \
    src/openglwrapper/window.h \
    src/openglwrapper/wrap/glwidget.h \
    src/picker/picker.h \
    src/mainwindow.h \
    src/openglwrapper/shader/shader.h \
    src/openglwrapper/object/object.h \
    src/openglwrapper/object/loader/loader.h

LIBS += -lopengl32

RESOURCES += \
    data/shaderPack.qrc
