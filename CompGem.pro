QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    convex_hull.cpp \
    drawwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    polygon_intersection.cpp \
    spline.cpp \
    tracing.cpp \
    triangulation.cpp

HEADERS += \
    convex_hull.h \
    drawwidget.h \
    geometry.h \
    mainwindow.h \
    polygon_intersection.h \
    spline.h \
    tracing.h \
    triangulation.h

FORMS += \
    Triangulation.ui \
    convex_hull.ui \
    mainwindow.ui \
    polygon_intersection.ui \
    spline.ui \
    tracing.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Elephant.obj \
    Low Poly Elephant-blendswap.obj \
    Piramida.obj \
    duck.obj
