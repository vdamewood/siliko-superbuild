# CMakeLists.txt: Build Script
# Copyright 2012-2023 Vincent Damewood
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

project(Siliko
	VERSION       0.0.0
	DESCRIPTION   "Mathematical expression parser written in C"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/siliko"
	LANGUAGES     C
)

set(SILIKO_COPYRIGHT "Copyright 2012 - 2023 Vincent Damewood")

#########################
# Configuration Options #
#########################

include(GNUInstallDirs)

option(SILIKO_BUILD_SHARED_LIBRARY  "Build Siliko as a shared library (build as a static library is off)" On)
option(SILIKO_BUILD_CLI             "Build Silikilo, a command-line interface for Siliko" On)
option(SILIKO_BUILD_GUI             "Build Silikujo, a GUI interface for Siliko" On)
option(SILIKO_PACKAGE               "Add 'package' and 'package_source' build targets for packages" On)
option(SILIKO_PACKAGE_ZIP           "Package as zip file (ignored if SILIKO_PACKAGE is off)" On)
option(SILIKO_PACKAGE_7ZIP          "Package as 7zip file (ignored if SILIKO_PACKAGE is off)" On)
option(SILIKO_PACKAGE_TXZ           "Package as XZ-compressed tar file (ignored if SILIKO_PACKAGE is off)" On)

set(SILIKO_PACKAGE_VENDOR  "Unknown Vendor"  CACHE STRING
	"The name of the vendor when generating packages (ignored if SILIKO_PACKAGE is off)"
)

if(APPLE)
	option(SILIKO_BUILD_FRAMEWORK   "Build Siliko as a framework." On)
	option(SILIKO_PACKAGE_DMG       "Package as DMG file (ignored if SILIKO_PACKAGE is off)" On)

	set(SILIKO_INSTALL_APPDIR Applications  CACHE STRING
		"Locaton to install application bundles"
	)
	set(SILIKO_INSTALL_FRAMEWORKDIR  Frameworks  CACHE STRING
		"Locaton to install framework (ignored if SILIKO_BUILD_FRAMEWORK is off)"
	)
endif()

if(WIN32)
	option(SILIKO_PACKAGE_MSI       "Package as MSI Installer  (Ignored if SILIKO_PACKAGE is false)" On)

	set(SILIKO_MSI_GUID  "00000000-0000-0000-0000-000000000000"  CACHE STRING
		"The Upgrade GUID used for MSI Installers (Ignored if SILIKO_PACKAGE or SILIKO_PACKAGE_MSI is false)"
	)
	option(SILIKO_OFFICIAL_RELEASE  "Build an official release of Siliko." Off)
	if(SILIKO_OFFICIAL_RELEASE)
	     set(VSV_PRERELEASE_FLAG 0)
	else()
	     set(VSV_PRERELEASE_FLAG VS_FF_PRERELEASE)
	endif()


	set(TMP_INSTALL_CMAKEDIR "cmake")
	else()
	set(TMP_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/Siliko")
endif()

# We use TMP_INSTALL_CMAKEDIR here to avoid a duplicate call to set()
# so there's a single place to edit the docstring.
set(SILIKO_INSTALL_CMAKEDIR "${TMP_INSTALL_CMAKEDIR}" CACHE STRING
		"Locaton to install CMake files (ignored if SILIKO_BUILD_FRAMEWORK is on)"
)


###########
# Targets #
###########

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(siliko)
if(SILIKO_BUILD_CLI)
	add_subdirectory(silikilo)
endif()
if(SILIKO_BUILD_GUI)
	if(APPLE)
		add_subdirectory(silikujo-macos)
	elseif(UNIX)
		add_subdirectory(silikujo-unix)
	elseif(WIN32)
		add_subdirectory(silikujo-windows)
	else()
		message(FATAL_ERROR "A GUI program is not available for this system.")
	endif()
endif()


if (SILIKO_PACKAGE)
	add_subdirectory(packaging)
endif()
