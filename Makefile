#
# $Id$
#
# File:   Makefile
# Author: Werner Jaeger
#
# Created on August 4, 2011, 3:17 PM
#
# Copyright 2011 Werner Jaeger.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# default configuration is release
DEFAULTCONF=Release

# Active Configuration
CONF ?= ${DEFAULTCONF}

# All Configurations
ALLCONFS=Debug Release 

# various directories
BUILDDIR = build/${CONF}
GENDIR = src/generated
DISTDIR = dist/${CONF}

# build
build: nbproject/qt-${CONF}.mk
	make -f nbproject/qt-${CONF}.mk ${DISTDIR}/L2tpIPsecVpnControlDaemon

# install
install: nbproject/qt-${CONF}.mk
	make -f nbproject/qt-${CONF}.mk install

# uninstall
uninstall: nbproject/qt-${CONF}.mk
	make -f nbproject/qt-${CONF}.mk uninstall

# clean
clean:
	rm -rf ${BUILDDIR}/*
	rm -rf ${GENDIR}/*
	rm -rf ${DISTDIR}/*
	rm -f nbproject/*.mk
	rm -f nbproject/*.bash
	rm -f *.mk

# clobber 
clobber:
	@for CONF in ${ALLCONFS}; \
	do \
	    make CONF=$${CONF} clean; \
	done

# help
help:
	@echo "This makefile supports the following configurations:"
	@echo "    ${ALLCONFS} (default = ${DEFAULTCONF})"
	@echo ""
	@echo "and the following targets:"
	@echo "    build  (default target)"
	@echo "    clean"
	@echo "    clobber"
	@echo "    install"
	@echo "    uninstall"
	@echo "    help"
	@echo ""
	@echo "Makefile Usage:"
	@echo "    make [CONF=<CONFIGURATION>] build"
	@echo "    make [CONF=<CONFIGURATION>] clean"
	@echo "    make clobber"
	@echo "    make [CONF=<CONFIGURATION>] [INSTALL_ROOT=<Base directory to install in>] install"
	@echo "    make [INSTALL_ROOT=<Base directory to uninstall from>] uninstall"
	@echo "    make help"
	@echo ""
	@echo "Target 'build' will build a specific configuration."
	@echo "Target 'clean' will remove all built files from a specific configuration."
	@echo "Target 'clobber' will remove all built files from all configurations"
	@echo "Target 'install' will install a specific configuration of the program"
	@echo "       in [INSTALL_ROOT]/usr/lib/l2tp-ipsec-vpn-daemon/"
	@echo "Target 'uninstall' will uninstall the program from [INSTALL_ROOT]/usr/lib/l2tp-ipsec-vpn-daemon/"
	@echo "Target 'help' prints this message"
	@echo ""

nbproject/qt-${CONF}.mk: nbproject/qt-${CONF}.pro
	qmake-qt4 -o qttmp-${CONF}.mk -after "OBJECTS_DIR=${BUILDDIR}" "DESTDIR=${DISTDIR}" nbproject/qt-${CONF}.pro
	mv -f qttmp-${CONF}.mk nbproject/qt-${CONF}.mk

