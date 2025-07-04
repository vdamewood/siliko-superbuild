# CMakeLists.txt: Build Script
# Copyright 2012-2025 Vincent Damewood
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.18)

project(SilikoGuiGnome
	VERSION       0.0.0
	DESCRIPTION   "GUI frontend for Siliko for Gnome"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/siliko"
	LANGUAGES     C
)
set(PRERELEASE_SUFFIX "indev")

include(GNUInstallDirs)

if(NOT TARGET Siliko::SilikoCore)
	find_package(SilikoCore REQUIRED)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(Gtk REQUIRED gtk4 glib-2.0>=2.74 IMPORTED_TARGET)

find_program(GRESOURCE glib-compile-resources)
if(NOT GRESOURCE)
	message(FATAL_ERROR "Can't find glib-compile-resources.")
endif()

add_custom_command(
	OUTPUT gresource.h
	COMMAND "${GRESOURCE}"
		--generate-header
		--target=${CMAKE_CURRENT_BINARY_DIR}/gresource.h
		gresource.xml
	DEPENDS src/gresource.xml src/EvalWindow.ui
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_custom_command(
	OUTPUT gresource.c
	COMMAND "${GRESOURCE}"
		--generate-source
		--target=${CMAKE_CURRENT_BINARY_DIR}/gresource.c
		gresource.xml
	DEPENDS src/gresource.xml src/EvalWindow.ui
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(SilikoGuiGnome)
target_sources(SilikoGuiGnome
	PRIVATE
		src/Main.c
		src/EvalWindow.c
		gresource.c
		src/EvalWindow.h
		gresource.h
)
set_target_properties(SilikoGuiGnome PROPERTIES
	OUTPUT_NAME               siliko-gui-gnome
)
target_compile_features(SilikoGuiGnome PRIVATE c_std_11)
target_link_libraries(SilikoGuiGnome
	PRIVATE
		Siliko::SilikoCore
		PkgConfig::Gtk
)

install(TARGETS SilikoGuiGnome
RUNTIME
	DESTINATION  ${CMAKE_INSTALL_BINDIR}
	COMPONENT    SilikoGuiGnome
)

install(FILES src/siliko-gui-gnome.desktop
	DESTINATION  "${CMAKE_INSTALL_DATADIR}/applications"
	COMPONENT    SilikoGuiGnome
)

install(FILES src/siliko-gui-gnome.png
	DESTINATION  "${CMAKE_INSTALL_DATADIR}/icons/hicolor/48x48/apps"
	COMPONENT    SilikoGuiGnome
)
