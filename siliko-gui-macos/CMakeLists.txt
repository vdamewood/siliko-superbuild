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

project(SilikoGuiMacOS
	VERSION       0.0.0
	DESCRIPTION   "GUI frontend for Siliko for macOS"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/siliko"
	LANGUAGES     OBJC
)
set(PRERELEASE_SUFFIX "indev")

include(GNUInstallDirs)

set(SILIKO_GUI_MACOS_BUNDLE_COPYRIGHT "Copyright 2012 - 2025 Vincent Damewood")
set(SILIKO_GUI_MACOS_INSTALL_APPDIR Applications  CACHE STRING
	"Locaton to install application bundle"
)

if(NOT TARGET Siliko::SilikoCore)
	find_package(SilikoCore REQUIRED)
endif()

find_program(IBTOOL ibtool)
if (NOT IBTOOL)
	message(FATAL_ERROR "Command ibtool not found. Can't build GUI. Quitting.")
endif()
find_program(ACTOOL actool)
if (NOT IBTOOL)
	message(FATAL_ERROR "Command actool not found. Can't build icon. Quitting.")
endif()

add_custom_command(OUTPUT MainMenu.nib
	COMMAND "${IBTOOL}" --compile MainMenu.nib "${CMAKE_CURRENT_SOURCE_DIR}/src/MainMenu.xib"
	COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/src/MainMenu.xib"
)
set_source_files_properties(MainMenu.nib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_custom_command(OUTPUT
	Assets.car
	SilikoIcon.icns
COMMAND ACTOOL
	--platform macosx
	--minimum-deployment-target 14.0
	--output-partial-info-plist /dev/null
	--compile ${CMAKE_CURRENT_BINARY_DIR}
	--app-icon SilikoIcon
	${CMAKE_CURRENT_SOURCE_DIR}/src/SilikoIcon.icon
COMMENT "Compiling application icon"
)
set_source_files_properties(Assets.car PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(SilikoIcon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(SilikoGuiMacOS MACOSX_BUNDLE)
target_sources(SilikoGuiMacOS
	PRIVATE
		src/Delegate.h
		src/Delegate.m
		src/Main.m
		MainMenu.nib
		SilikoIcon.icns
		Assets.car
)
set_target_properties(SilikoGuiMacOS PROPERTIES
	OUTPUT_NAME                         "Siliko GUI"
	MACOSX_BUNDLE_GUI_IDENTIFIER        "com.vdamewood.SilikoGuiMacOS"
	MACOSX_BUNDLE_BUNDLE_NAME           "Siliko GUI"
	MACOSX_BUNDLE_SHORT_VERSION_STRING  ${PROJECT_VERSION}
	MACOSX_BUNDLE_BUNDLE_VERSION        ${PROJECT_VERSION}
	MACOSX_BUNDLE_COPYRIGHT             "${SILIKO_GUI_MACOS_BUNDLE_COPYRIGHT}"
	MACOSX_BUNDLE_ICON_FILE             SilikoIcon
	MACOSX_BUNDLE_INFO_PLIST            "${CMAKE_CURRENT_SOURCE_DIR}/src/Info.plist.in"
)
target_compile_options(SilikoGuiMacOS
	PRIVATE
		"-fobjc-arc"
)
target_compile_features(SilikoGuiMacOS PRIVATE c_std_11)
target_link_libraries(SilikoGuiMacOS
	PRIVATE
		Siliko::SilikoCore
		"-framework Cocoa"
)
siliko_core_fixup_bundle(SilikoGuiMacOS)

install(TARGETS SilikoGuiMacOS
BUNDLE
	DESTINATION  ${SILIKO_GUI_MACOS_INSTALL_APPDIR}
	COMPONENT    SilikoGuiMacOS
)
