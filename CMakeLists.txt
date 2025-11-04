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

cmake_minimum_required(VERSION 3.18)

option(SILIKO_BUILD_CLI  "Build siliko-calc command-line tool" On)
option(SILIKO_BUILD_GUI  "Build siliko-calc GUI app for the current system" On)

project(SilikoSuperBuild)

add_subdirectory(siliko)

if(SILIKO_BUILD_CLI)
	add_subdirectory(siliko-calc-cli)
endif()

if(SILIKO_BUILD_GUI)
	if(APPLE)
		add_subdirectory(siliko-calc-cocoa)
	elseif(UNIX)
		add_subdirectory(siliko-calc-gtk)
	elseif(WIN32)
		add_subdirectory(siliko-calc-winapi)
	else()
		message(FATAL_ERROR "A GUI program is not available for this system.")
	endif()
endif()
