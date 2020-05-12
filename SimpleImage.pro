QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SimpleImage
TEMPLATE = app
CONFIG += C++11

TEMPLATE = app

INCLUDEPATH += ./ \
    dialog/ \
    tools/  \
    mainwindow/

HEADERS += \
    dialog/aboutdialog.h \
    dialog/dialog_gaussblur.h \
    mainwindow/mainwindow.h    \
    mainwindow/graphicsview.h   \
    mainwindow/graphicsview.h   \
    tools/algorithm.h \
    tools/gaussblur.h \
    tools/medianfilter.h \

SOURCES += \
    dialog/aboutdialog.cpp \
    dialog/dialog_gaussblur.cpp \
    mainwindow/mainwindow.cpp  \
    mainwindow/graphicsview.cpp \
    tools/algorithm.cpp \
    tools/gaussblur.cpp \
    tools/medianfilter.cpp  \
    main.cpp

FORMS    += mainwindow/mainwindow.ui \
    dialog/aboutdialog.ui \
    dialog/gaussblurdialog.ui

RESOURCES += \
    res/ImageDir.qrc

RC_FILE = res/ico.rc

