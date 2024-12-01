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

project(Silikilo
	VERSION       0.0.0
	DESCRIPTION   "Command-line frontend for Siliko"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/siliko"
	LANGUAGES     C
)
set(PRERELEASE_SUFFIX "indev")

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
if(NOT TARGET Siliko::SilikoCore)
	find_package(SilikoCore REQUIRED)
endif()
find_package(Readline)

add_executable(Silikilo)
target_sources(Silikilo
	PRIVATE
		main.c
)
set_target_properties(Silikilo PROPERTIES
	OUTPUT_NAME               silikilo
)
target_compile_features(Silikilo PRIVATE c_std_11)
target_link_libraries(Silikilo PRIVATE Siliko::SilikoCore)

if (UNIX)
	target_compile_definitions(Silikilo PRIVATE USE_UNIX=1)
endif()

if(Readline_FOUND)
	target_link_libraries(Silikilo PRIVATE Readline::Readline)
	target_compile_definitions(Silikilo PRIVATE HAVE_READLINE=1)
else()
	target_sources(Silikilo PRIVATE rl.c)
	target_compile_definitions(Silikilo PRIVATE HAVE_READLINE=0)
endif()

if(APPLE AND SILIKO_BUILD_SHARED_LIBRARY)
	if (SILIKO_BUILD_FRAMEWORK)
		set_target_properties(Silikilo PROPERTIES
			INSTALL_RPATH                   "@executable_path/../Frameworks"
		)
	else()
		set_target_properties(Silikilo PROPERTIES
			INSTALL_RPATH                   "@executable_path/../lib"
		)
	endif()
endif()

if(WIN32)
	if(PRERELEASE_SUFFIX STREQUAL "")
		set(VSV_PRERELEASE_FLAG 0)
	else()
		set(VSV_PRERELEASE_FLAG VS_FF_PRERELEASE)
	endif()

	set(VSV_DEBUG_FLAG 0)
	set(VSV_FILENAME "silikilo.exe")
	configure_file(
		version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-release.rc
		@ONLY
	)
	target_sources(Silikilo PRIVATE $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}/version-release.rc>)

	set(VSV_DEBUG_FLAG VS_FF_DEBUG)
	set(VSV_FILENAME "silikilo.exe")
	configure_file(
		version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc
		@ONLY
  	)
	target_sources(Silikilo PRIVATE $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc>)
endif()

install(TARGETS Silikilo
RUNTIME
	DESTINATION  ${CMAKE_INSTALL_BINDIR}
	COMPONENT    Silikilo
)
