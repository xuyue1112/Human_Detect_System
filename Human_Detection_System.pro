#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T14:11:22
#
#-------------------------------------------------

QT       += core gui
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Human_Detection_System
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        qcustomplot.cpp \
    recognise_result.cpp

HEADERS  += mainwindow.h \
        qcustomplot.h \
    recognise_result.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv \

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so \
        /usr/local/lib/libopencv_imgproc.so.2.4 \
        /usr/local/lib/libopencv_video.so \
        /usr/local/lib/libopencv_objdetect.so.2.4.13 \

RESOURCES += \
    images/images.qrc \
    images/orl.qrc \
    images/me_and_friends.qrc


LIBS += /usr/local/lib/libopenbr.so

LIBS += /usr/local/lib/libopencv_core.so.2.4
