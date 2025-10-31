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

project(SilikoCore
	VERSION       0.0.0
	DESCRIPTION   "Mathematical expression parser library written in C"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/siliko"
	LANGUAGES     C
)

set(SILIKO_CORE_COPYRIGHT "Copyright 2012-2025 Vincent Damewood")
#[=[
* `indev`: SCM commits
* `beta.<uint>` (eg "beta.2") beta test releases
* "rc.<uint>" (eg "rc.1") release candidates
* "": official releases
#]=]
set(SILIKO_CORE_PRERELEASE_SUFFIX "indev")

# Follow your package repository's guidelines when setting this value.
set(SILIKO_CORE_PACKAGE_SUFFIX "" CACHE STRING
	"Suffix to append to packaged versions of the Siliko Core library by package maintainers"
)

include(GNUInstallDirs)

if(APPLE)
	option(SILIKO_CORE_BUILD_FRAMEWORK   "Build Siliko Core as a framework." On)
	set(SILIKO_CORE_INSTALL_FRAMEWORKDIR  Frameworks  CACHE STRING
		"Locaton to install framework (ignored if SILIKO_CORE_BUILD_FRAMEWORK is off)"
	)
endif()
set(SILIKO_CORE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/SilikoCore" CACHE STRING
		"Locaton to install CMake files (ignored if SILIKO_CORE_BUILD_FRAMEWORK is on)"
)

add_library(SilikoCore)
add_library(Siliko::SilikoCore ALIAS SilikoCore)
target_sources(SilikoCore
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
			include
		FILES
			include/SilikoCore/Value.h
			include/SilikoCore/FunctionCaller.h
			include/SilikoCore/SyntaxTree.h
			include/SilikoCore/DataSource.h
			include/SilikoCore/StringSource.h
			include/SilikoCore/Token.h
			include/SilikoCore/Lexer.h
			include/SilikoCore/InfixParser.h
			include/SilikoCore/Version.h
	PRIVATE
		src/Functions.h
		src/Functions.c
		src/FunctionCaller.c
		src/SyntaxTree.c
		src/DataSource.c
		src/StringSource.c
		src/Lexer.c
		src/InfixParser.c
)
target_compile_features(SilikoCore PRIVATE c_std_11)
set_target_properties(SilikoCore PROPERTIES
	C_VISIBILITY_PRESET       hidden
	VISIBILITY_INLINES_HIDDEN On
	VERSION                   ${PROJECT_VERSION}
	SOVERSION                 ${PROJECT_VERSION_MAJOR}
)

# Some systems provide math.h functions in a seperate library called
# libm. This checks to see if math.h functions work and if they need
# libm.
set(SIN_PROGRAM [[
#include <math.h>

int main(int argc, char *argv[])
{
	return sin(argc*0.017453292519943295) * 255;
}
]]
)
include(CMakePushCheckState)
include(CheckSourceCompiles)
cmake_push_check_state(RESET)
check_source_compiles(C "${SIN_PROGRAM}" CAN_USE_SIN_WITHOUT_LIBM)
if(NOT CAN_USE_SIN_WITHOUT_LIBM)
	cmake_reset_check_state()
	find_library(LIBM m)
	if (NOT LIBM)
		message(FATAL_ERROR "Can't use sin() without libm and can't find libm.")
	endif()
	set(CMAKE_REQUIRED_LIBRARIES ${LIBM})
	check_source_compiles(C "${SIN_PROGRAM}" CAN_USE_SIN_WITH_LIBM)
	if(NOT CAN_USE_SIN_WITH_LIBM)
		message(FATAL_ERROR "Can't use sin() with or without libm().")
	endif()
	target_link_libraries(SilikoCore PRIVATE "${LIBM}")
endif()
cmake_pop_check_state()

configure_file(
	src/Version.c.in
	${CMAKE_CURRENT_BINARY_DIR}/Version.c
	@ONLY
)
target_sources(SilikoCore
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/Version.c
)
if(WIN32)
	if(SILIKO_CORE_PRERELEASE_SUFFIX STREQUAL "")
		set(PRERELEASE_FLAG 0)
	else()
		set(PRERELEASE_FLAG VS_FF_PRERELEASE)
	endif()
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version.rc
		@ONLY
  	)
	target_compile_definitions(SilikoCore
		PRIVATE
			"SILIKO_CORE_ORIGINAL_FILENAME=\"$<TARGET_FILE_NAME:SilikoCore>\""
	)
	target_sources(SilikoCore
		PRIVATE
			"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	)
endif()

include(GenerateExportHeader)
generate_export_header(SilikoCore
	EXPORT_FILE_NAME include/SilikoCore/Api.h
)
target_sources(SilikoCore
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
			${CMAKE_CURRENT_BINARY_DIR}/include
		FILES
			${CMAKE_CURRENT_BINARY_DIR}/include/SilikoCore/Api.h
)

#[[
Ideally, an if statement here wouldn't be needed, but since
PUBLIC_HEADER is redundant with FILE_SET HEADERS on non-framework
builds, it can cause conflicts when installing.
#]]
if(SILIKO_CORE_BUILD_FRAMEWORK)
	get_target_property(PUBLIC_HEADERS SilikoCore HEADER_SET_HEADERS)
	set_target_properties(SilikoCore PROPERTIES
		FRAMEWORK                 On
		PUBLIC_HEADER             "${PUBLIC_HEADERS}"
		FRAMEWORK_VERSION         ${PROJECT_VERSION}
	)

	set(CMAKECONFIG_DIR        "${SILIKO_CORE_INSTALL_FRAMEWORKDIR}/SilikoCore.framework/Resources")
	set(CMAKECONFIG_COMPONENT  "SilikoCore_Framework")
	set(DOC_DIR                "${SILIKO_CORE_INSTALL_FRAMEWORKDIR}/SilikoCore.framework/Resources/Documentation")
	set(DOC_COMPONENT          "SilikoCore_Framework")
else()
	set(CMAKECONFIG_DIR        "${SILIKO_CORE_INSTALL_CMAKEDIR}")
	set(CMAKECONFIG_COMPONENT  "SilikoCore_Development")
	set(DOC_DIR                "${CMAKE_INSTALL_DATADIR}/doc/SilikoCore")
	set(DOC_COMPONENT          "SilikoCore_Runtime")
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	SilikoCoreConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(TARGETS SilikoCore
	EXPORT SilikoCore
RUNTIME
	DESTINATION           "${CMAKE_INSTALL_BINDIR}"
	COMPONENT             SilikoCore_Runtime
ARCHIVE
	DESTINATION           "${CMAKE_INSTALL_LIBDIR}"
	COMPONENT             SilikoCore_Development
LIBRARY
	DESTINATION           "${CMAKE_INSTALL_LIBDIR}"
	COMPONENT             SilikoCore_Runtime
	NAMELINK_COMPONENT    SilikoCore_Development
FRAMEWORK
	DESTINATION           "${SILIKO_CORE_INSTALL_FRAMEWORKDIR}"
	COMPONENT             SilikoCore_Framework
FILE_SET HEADERS
	DESTINATION           "${CMAKE_INSTALL_INCLUDEDIR}"
	COMPONENT             SilikoCore_Development
)
install(EXPORT SilikoCore
	NAMESPACE             Siliko::
	FILE                  SilikoCoreTarget.cmake
	DESTINATION           "${CMAKECONFIG_DIR}"
	COMPONENT             "${CMAKECONFIG_COMPONENT}"
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SilikoCoreConfigVersion.cmake"
		"SilikoCoreConfig.cmake"
	DESTINATION           "${CMAKECONFIG_DIR}"
	COMPONENT             "${CMAKECONFIG_COMPONENT}"
)

install(FILES
		"${PROJECT_SOURCE_DIR}/Readme.md"
		"${PROJECT_SOURCE_DIR}/License.md"
		"${PROJECT_SOURCE_DIR}/License.Gpl.md"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/SilikoCore"
	COMPONENT "${RUNTIME_COMPONENT}"
)

function(siliko_core_fixup_bundle TGT)
	if(APPLE AND TARGET ${TGT})
		get_target_property(TGT_IS_BUNDLE ${TGT} MACOSX_BUNDLE)
	else()
		set(TGT_IS_BUNDLE No)
	endif()
	get_target_property(SILIKO_CORE_TYPE Siliko::SilikoCore TYPE)
	get_target_property(SILIKO_CORE_IS_FRAMEWORK Siliko::SilikoCore FRAMEWORK)

	if (${SILIKO_CORE_TYPE} STREQUAL SHARED_LIBRARY AND TGT_IS_BUNDLE)
		get_target_property(TGT_RPATH ${TGT} INSTALL_RPATH)
		if(NOT "@executable_path/../Frameworks" IN_LIST TGT_RPATH)
			list(APPEND TGT_RPATH "@executable_path/../Frameworks")
			set_target_properties(${TGT} PROPERTIES
				INSTALL_RPATH  ${TGT_RPATH})
		endif()
		set_target_properties(${TGT} PROPERTIES
			BUILD_WITH_INSTALL_RPATH On)

		if(SILIKO_CORE_IS_FRAMEWORK)
			add_custom_command(TARGET ${TGT} POST_BUILD
				COMMAND "cmake" "-E" "make_directory"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::SilikoCore>.framework/Versions/$<TARGET_PROPERTY:Siliko::SilikoCore,FRAMEWORK_VERSION>/Resources"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE_DIR:Siliko::SilikoCore>/Resources/Info.plist"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::SilikoCore>.framework/Versions/$<TARGET_PROPERTY:Siliko::SilikoCore,FRAMEWORK_VERSION>/Resources/"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE:Siliko::SilikoCore>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::SilikoCore>.framework/Versions/$<TARGET_PROPERTY:Siliko::SilikoCore,FRAMEWORK_VERSION>/"
				COMMAND "cmake" "-E" "create_symlink"
					"$<TARGET_PROPERTY:Siliko::SilikoCore,FRAMEWORK_VERSION>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::SilikoCore>.framework/Versions/Current"
				COMMAND "cmake" "-E" "create_symlink"
					"Versions/Current/Resources"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::SilikoCore>.framework/Resources"
				COMMAND "cmake" "-E" "create_symlink"
					"Versions/Current/$<TARGET_FILE_NAME:Siliko::SilikoCore>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::SilikoCore>.framework/$<TARGET_FILE_NAME:Siliko::SilikoCore>"
				COMMENT "Installing Siliko Core framework into ${TGT} bundle"
			)
		else()
			add_custom_command(TARGET ${TGT} POST_BUILD
				COMMAND "cmake" "-E" "make_directory"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE:Siliko::SilikoCore>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/"
				COMMAND "cmake" "-E" "create_symlink"
					"$<TARGET_FILE_NAME:Siliko::SilikoCore>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_SONAME_FILE_NAME:Siliko::SilikoCore>"
				COMMENT "Installing Siliko Core dylib into ${TGT} bundle"
			)
		endif()
	endif()
endfunction()
