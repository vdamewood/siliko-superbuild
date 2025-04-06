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

project(SilikujoForWindows
	VERSION       0.0.0
	DESCRIPTION   "GUI frontend for Siliko for Windows"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/siliko"
	LANGUAGES     C
)
set(PRERELEASE_SUFFIX "indev")

include(GNUInstallDirs)

if(NOT TARGET Siliko::SilikoCore)
	find_package(SilikoCore REQUIRED)
endif()

add_executable(SilikujoForWindows WIN32)
target_sources(SilikujoForWindows
	PRIVATE
		src/EvalWindow.h
		src/About.h
		src/Id.h
		src/WinMain.c
		src/EvalWindow.c
		src/EvalWindow.rc
		src/About.c
		src/About.rc
		src/Icon.rc
)
set_target_properties(SilikujoForWindows PROPERTIES
	OUTPUT_NAME               Silikujo
)
if(PRERELEASE_SUFFIX STREQUAL "")
	set(VSV_PRERELEASE_FLAG 0)
else()
	set(VSV_PRERELEASE_FLAG VS_FF_PRERELEASE)
endif()
set(VSV_DEBUG_FLAG 0)
set(VSV_FILENAME "Silikujo.exe")
configure_file(
	src/Version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/Version.Release.rc
	@ONLY
)
target_sources(SilikujoForWindows PRIVATE $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}/Version.Release.rc>)

set(VSV_DEBUG_FLAG VS_FF_DEBUG)
set(VSV_FILENAME "Silikujo.exe")
configure_file(
	src/Version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/Version.Debug.rc
	@ONLY
)
target_sources(SilikujoForWindows PRIVATE $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/Version.Debug.rc>)
target_compile_features(SilikujoForWindows PRIVATE c_std_11)
target_link_libraries(SilikujoForWindows PRIVATE Siliko::SilikoCore)

install(TARGETS SilikujoForWindows
RUNTIME
	DESTINATION  ${CMAKE_INSTALL_BINDIR}
	COMPONENT    Silikujo
)
