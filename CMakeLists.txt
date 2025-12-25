# CMakeLists.txt: Build Script
# Copyright 2012-2025 Vincent Damewood
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.30)

option(BUILD_CLI    "Build calculator command-line tool"   Off)
option(BUILD_COCOA  "Build calculator GUI app for macOS"   Off)
option(BUILD_GTK    "Build calculator GUI app for GNOME"   Off)
option(BUILD_WINAPI "Build calculator GUI app for Windows" Off)

project(SilikoSuperBuild)

include(CTest)

add_subdirectory(siliko)

if(BUILD_CLI)
	add_subdirectory(siliko-calc-cli)
endif()

if(BUILD_COCOA)
	add_subdirectory(siliko-calc-cocoa)
endif()

if(BUILD_GTK)
	add_subdirectory(siliko-calc-gtk)
endif()

if(BUILD_WINAPI)
	add_subdirectory(siliko-calc-winapi)
endif()
