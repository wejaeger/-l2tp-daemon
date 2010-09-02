# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = L2tpIPsecVpnControlDaemon
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
QT = core network
SOURCES += src/main.cpp src/VpnClientConnection.cpp src/VpnControlDaemonApplication.cpp src/VpnControlDaemon.cpp
HEADERS += src/VpnControlDaemon.h src/VpnClientConnection.h src/VpnControlDaemonApplication.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = src/generated
RCC_DIR = src/generated
UI_DIR = src/generated
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += ../qtservice-2.6_1/src 
LIBS += 
include(../qtservice-2.6_1/src/qtservice.pri)
