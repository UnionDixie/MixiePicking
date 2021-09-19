QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    src/OpenglWrapper/scene/scene.cpp \
    src/OpenglWrapper/window.cpp \
    src/OpenglWrapper/wrap/glwidget.cpp \
    src/Picker/picker.cpp \
    src/main.cpp \
    src/OpenglWrapper/shader/Shader.cpp \
    src/OpenglWrapper/object/object.cpp \
    src/OpenglWrapper/object/loader/Loader.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/OpenglWrapper/scene/scene.h \
    src/OpenglWrapper/window.h \
    src/OpenglWrapper/wrap/glwidget.h \
    src/Picker/picker.h \
    src/mainwindow.h \
    src/OpenglWrapper/shader/Shader.h \
    src/OpenglWrapper/object/object.h \
    src/OpenglWrapper/object/loader/Loader.h

LIBS += -opengl32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data/shaderPack.qrc
