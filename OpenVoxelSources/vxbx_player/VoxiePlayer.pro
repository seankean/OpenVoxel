#-------------------------------------------------
#
# Project created by QtCreator 2011-08-04T17:51:09
#
#-------------------------------------------------

QT       += core gui opengl network

TARGET = VoxiePlayer
TEMPLATE = app


SOURCES += \
main.cpp\
widget.cpp\
PortListener.cpp

HEADERS  += \
widget.h \
config.h\
PortListener.h 



FORMS +=

OTHER_FILES += \
USER_MANUAL.txt

include(../../3rdparty/qextserialport-1.2beta1/src/qextserialport.pri)

CONFIG += \
debug \
console 
