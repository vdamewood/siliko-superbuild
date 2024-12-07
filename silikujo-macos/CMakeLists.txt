# CMakeLists.txt: Build Script
# Copyright 2012-2024 Vincent Damewood
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

project(SilikujoForMacOS
	VERSION       0.0.0
	DESCRIPTION   "GUI frontend for Siliko for macOS"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/siliko"
	LANGUAGES     OBJC
)
set(PRERELEASE_SUFFIX "indev")

include(GNUInstallDirs)

set(SILIKUJO_BUNDLE_COPYRIGHT "Copyright 2012 - 2024 Vincent Damewood")
set(SILIKUJO_INSTALL_APPDIR Applications  CACHE STRING
	"Locaton to install application bundle"
)

if(NOT TARGET Siliko::SilikoCore)
	find_package(SilikoCore REQUIRED)
endif()

find_program(IBTOOL ibtool)
if (NOT IBTOOL)
	message(FATAL_ERROR "Command ibtool not found. Can't build GUI. Quitting.")
endif()

add_custom_command(OUTPUT MainMenu.nib
	COMMAND "${IBTOOL}" --compile MainMenu.nib "${CMAKE_CURRENT_SOURCE_DIR}/src/MainMenu.xib"
	COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/src/MainMenu.xib"
)
set_source_files_properties(MainMenu.nib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(src/silikujo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(SilikujoForMacOS MACOSX_BUNDLE)
target_sources(SilikujoForMacOS
	PRIVATE
		src/Delegate.h
		src/Delegate.m
		src/Main.m
		MainMenu.nib
		src/silikujo.icns
)
set_target_properties(SilikujoForMacOS PROPERTIES
	OUTPUT_NAME                         Silikujo
	MACOSX_BUNDLE_GUI_IDENTIFIER        "com.vdamewood.SilikujoForMacOS"
	MACOSX_BUNDLE_BUNDLE_NAME           "Silikujo"
	MACOSX_BUNDLE_SHORT_VERSION_STRING  ${PROJECT_VERSION}
	MACOSX_BUNDLE_BUNDLE_VERSION        ${PROJECT_VERSION}
	MACOSX_BUNDLE_COPYRIGHT             "${SILIKUJO_BUNDLE_COPYRIGHT}"
	MACOSX_BUNDLE_ICON_FILE             src/silikujo.icns
	MACOSX_BUNDLE_INFO_PLIST            "${CMAKE_CURRENT_SOURCE_DIR}/src/Info.plist.in"
)
target_compile_options(SilikujoForMacOS
	PRIVATE
		"-fobjc-arc"
)
target_compile_features(SilikujoForMacOS PRIVATE c_std_11)
target_link_libraries(SilikujoForMacOS
	PRIVATE
		Siliko::SilikoCore
		"-framework Cocoa"
)
siliko_core_fixup_bundle(SilikujoForMacOS)

install(TARGETS SilikujoForMacOS
BUNDLE
	DESTINATION  ${SILIKUJO_INSTALL_APPDIR}
	COMPONENT    Silikujo
)
