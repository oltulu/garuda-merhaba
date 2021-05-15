# **********************************************************************
# * Copyright (C) 2015 Garuda Authors
# *
# * Authors: Adrian
# *          Paul David Callahan
# *          Garuda & MEPIS Community <http://forum.mepiscommunity.org>
# *
# * This file is part of garuda-welcome.
# *
# * garuda-welcome is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * garuda-welcome is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with garuda-welcome.  If not, see <http://www.gnu.org/licenses/>.
# **********************************************************************/

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = garuda-welcome
TEMPLATE = app

# Build section
BUILD_PREFIX = $$(CA_BUILD_DIR)

isEmpty( BUILD_PREFIX ) {
        BUILD_PREFIX = ./build
}

MOC_DIR       = $$BUILD_PREFIX/moc-qt5
OBJECTS_DIR   = $$BUILD_PREFIX/obj-qt5
RCC_DIR	      = $$BUILD_PREFIX/qrc-qt5
UI_DIR        = $$BUILD_PREFIX/uic-qt5

# Disable QDebug on Release build
#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

# Unused not working
#CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

unix {
        isEmpty(PREFIX) {
                PREFIX = /usr
        }
        BINDIR = $$PREFIX/bin

        target.path = $$BINDIR

        desktop.path = $$PREFIX/share/applications/
        desktop.files = "garuda-welcome.desktop"

        icons.path = /usr/share/icons/hicolor/scalable/apps/
        icons.files = garuda-welcome.png

        SCRIPTS_PATH = $$PREFIX/share/garuda/

        scripts.files = scripts
        scripts.path  = $$SCRIPTS_PATH

        INSTALLS += target icons desktop scripts
}



SOURCES += main.cpp\
        garudawelcome.cpp

HEADERS  += garudawelcome.h

FORMS    += garudawelcome.ui

RESOURCES += \
    images.qrc

DISTFILES += \
    icons/Moodle.png \
    icons/bbb.png \
    icons/bitwarden.svg \
    icons/calamares.png \
    icons/chroot.png \
    icons/cleanup.png \
    icons/cryptpad.png \
    icons/element.svg \
    icons/forum.svg \
    icons/garuda-boot-options.png \
    icons/garuda-boot-repair.png \
    icons/garuda-green.png \
    icons/garuda-logo.png \
    icons/garuda-network-assistant.png \
    icons/garuda-settings.png \
    icons/gitlab.png \
    icons/logo.png \
    icons/nextcloud.svg \
    icons/partition.png \
    icons/privatebin.png \
    icons/qwikaccess.svg \
    icons/searx.svg \
    icons/software.png \
    icons/statping.png \
    icons/telegram.png \
    icons/timeshift.png \
    icons/twitter.png \
    icons/website.png \
    icons/whoogle.png


