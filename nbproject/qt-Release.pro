# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = L2tpIPsecVpnControlDaemon
VERSION = 0.9.2
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core network
SOURCES += src/main.cpp src/VpnClientConnection.cpp src/VpnControlDaemonApplication.cpp src/VpnControlDaemon.cpp
HEADERS += src/VpnControlDaemon.h src/VpnClientConnection.h src/VpnControlDaemonApplication.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
MOC_DIR = src/generated
RCC_DIR = src/generated
UI_DIR = src/generated
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += -s  
target.path = /usr/lib/l2tp-ipsec-vpn-daemon
INSTALLS += target
