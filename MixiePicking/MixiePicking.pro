QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    src/OpenglWrapper/window.cpp \
    src/OpenglWrapper/wrap/glwidget.cpp \
    src/Picker/picker.cpp \
    src/main.cpp \
    src/OpenglWrapper/Shader/Shader.cpp \
    src/OpenglWrapper/Object/object.cpp \
    src/OpenglWrapper/Object/Loader/Loader.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/OpenglWrapper/window.h \
    src/OpenglWrapper/wrap/glwidget.h \
    src/Picker/picker.h \
    src/mainwindow.h \
    src/OpenglWrapper/Shader/Shader.h \
    src/OpenglWrapper/Object/object.h \
    src/OpenglWrapper/Object/Loader/Loader.h

LIBS += -lopengl32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data/shaderPack.qrc
