# qimhangul - Qt input module for hangul
# Copyright (C) 2020 Choe Hwanjin
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA

cmake_minimum_required(VERSION 3.1)

project(qimhangul)

find_package(Qt5Core 5.0.0 REQUIRED)
find_package(Qt5Gui 5.0.0 REQUIRED)
find_package(Qt5Widgets 5.0.0 REQUIRED)
find_package(Qt5X11Extras 5.0.0)

find_package(PkgConfig)
pkg_check_modules(LIBHANGUL libhangul REQUIRED)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(qimhangulinputcontextplugin_SOURCES
    candidatelist.cpp
    candidatelist.h
    qinputcontexthangul.cpp
    qinputcontexthangul.h
    qinputcontextpluginhangul.cpp
    qinputcontextpluginhangul.h
)

if (Qt5X11Extras_FOUND)
    list(APPEND qimhangulinputcontextplugin_SOURCES qinputcontexthangul_x11.cpp)
else()
    list(APPEND qimhangulinputcontextplugin_SOURCES qinputcontexthangul_dummy.cpp)
endif() # if (Qt5X11Extras_FOUND)

add_library(qimhangulinputcontextplugin
    MODULE
        ${qimhangulinputcontextplugin_SOURCES}
)

set_target_properties(qimhangulinputcontextplugin
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY platforminputcontexts
)

target_include_directories(qimhangulinputcontextplugin
    PRIVATE
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
        ${LIBHANGUL_INCLUDE_DIRS}
)

target_link_libraries(qimhangulinputcontextplugin
    PRIVATE
        ${Qt5Widgets_LIBRARIES}
        ${Qt5Gui_LIBRARIES}
        ${Qt5Core_LIBRARIES}
        ${LIBHANGUL_LINK_LIBRARIES}
)

if (Qt5X11Extras_FOUND)
target_compile_definitions(qimhangulinputcontextplugin
    PRIVATE
        HAVE_QTX11EXTRAS=1
)

target_include_directories(qimhangulinputcontextplugin
    PRIVATE
        ${Qt5X11Extras_INCLUDE_DIRS}
)

target_link_libraries(qimhangulinputcontextplugin
    PRIVATE
        ${Qt5X11Extras_LIBRARIES}
)
endif() # if (Qt5X11Extras_FOUND)
