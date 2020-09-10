HEADERS      = \
    point3D.h \
    OGLLabs.h \
    OShowOptions.h \
    data.h \
    camera.h
SOURCES	     = \
               point3D.cpp \
     OGLLabs.cpp\
    main.cpp \
    OShowOptions.cpp \
    data.cpp \
    camera.cpp
QT           += widgets gui
win32:TARGET = ../OGLLabs

RESOURCES += \
    shaders.qrc