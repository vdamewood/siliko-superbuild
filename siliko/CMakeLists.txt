# CMakeLists.txt: Build Script
# Copyright 2012-2024 Vincent Damewood
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
	DESCRIPTION   "Mathematical expression parser library written in C"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/siliko"
	LANGUAGES     C
)

include(GNUInstallDirs)

if(APPLE)
	option(SILIKO_BUILD_FRAMEWORK   "Build Siliko as a framework." On)
	set(SILIKO_INSTALL_FRAMEWORKDIR  Frameworks  CACHE STRING
		"Locaton to install framework (ignored if SILIKO_BUILD_FRAMEWORK is off)"
	)
endif()
if(WIN32)
	option(SILIKO_OFFICIAL_RELEASE  "Build an official release of Siliko." Off)
endif()
set(SILIKO_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/Siliko" CACHE STRING
		"Locaton to install CMake files (ignored if SILIKO_BUILD_FRAMEWORK is on)"
)

if(SILIKO_BUILD_FRAMEWORK)
	set(CMAKEDIR "${SILIKO_INSTALL_FRAMEWORKDIR}/Siliko.framework/Resources")
	set(RUNTIME_COMPONENT "Siliko")
	set(DEVELOPMENT_COMPONENT "Siliko")
else()
	set(CMAKEDIR "${SILIKO_INSTALL_CMAKEDIR}")
	set(RUNTIME_COMPONENT "Siliko_Runtime")
	set(DEVELOPMENT_COMPONENT "Siliko_Development")
endif()


find_library(LIBM m)

add_library(Siliko)
add_library(Siliko::Siliko ALIAS Siliko)
set(PUBLIC_HEADERS
	include/Siliko/Value.h
	include/Siliko/FunctionCaller.h
	include/Siliko/SyntaxTree.h
	include/Siliko/DataSource.h
	include/Siliko/StringSource.h
	include/Siliko/Token.h
	include/Siliko/Lexer.h
	include/Siliko/InfixParser.h
	${CMAKE_CURRENT_BINARY_DIR}/include/Siliko/Api.h
)
target_sources(Siliko
	PRIVATE
		${PUBLIC_HEADERS}
		src/Functions.h
		src/Hash.h
		src/Value.c
		src/Hash.c
		src/Functions.c
		src/FunctionCaller.c
		src/SyntaxTree.c
		src/DataSource.c
		src/StringSource.c
		src/Lexer.c
		src/InfixParser.c
)
if(LIBM)
	target_link_libraries(Siliko PRIVATE m)
endif()
target_include_directories(Siliko
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
if (NOT SILIKO_BUILD_FRAMEWORK)
	target_include_directories(Siliko
	INTERFACE
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endif()
target_compile_features(Siliko PRIVATE c_std_11)
set_target_properties(Siliko PROPERTIES
	PUBLIC_HEADER             "${PUBLIC_HEADERS}"
	C_VISIBILITY_PRESET       hidden
	VISIBILITY_INLINES_HIDDEN On
	FRAMEWORK                 "${SILIKO_BUILD_FRAMEWORK}"
	FRAMEWORK_VERSION         ${PROJECT_VERSION}
	VERSION                   ${PROJECT_VERSION}
	SOVERSION                 ${PROJECT_VERSION_MAJOR}
)
if(WIN32)
	if(SILIKO_OFFICIAL_RELEASE)
		set(VSV_PRERELEASE_FLAG 0)
	else()
		set(VSV_PRERELEASE_FLAG VS_FF_PRERELEASE)
	endif()

	set(VSV_DEBUG_FLAG 0)
	set(VSV_FILENAME "Siliko.dll")
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-release.rc
		@ONLY
  	)
	target_sources(Siliko PRIVATE $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}/version-release.rc>)

	set(VSV_DEBUG_FLAG VS_FF_DEBUG)
	set(VSV_FILENAME "Siliko.dll")
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc
		@ONLY
  	)
	target_sources(Siliko PRIVATE $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc>)
endif()

include(GenerateExportHeader)
generate_export_header(Siliko
	EXPORT_FILE_NAME include/Siliko/Api.h
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	SilikoConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(TARGETS Siliko
	EXPORT Siliko
RUNTIME
	DESTINATION           "${CMAKE_INSTALL_BINDIR}"
	COMPONENT             "${RUNTIME_COMPONENT}"
ARCHIVE
	DESTINATION           "${CMAKE_INSTALL_LIBDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
LIBRARY
	DESTINATION           "${CMAKE_INSTALL_LIBDIR}"
	COMPONENT             "${RUNTIME_COMPONENT}"
	NAMELINK_COMPONENT    "${DEVELOPMENT_COMPONENT}"
FRAMEWORK
	DESTINATION           "${SILIKO_INSTALL_FRAMEWORKDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
PUBLIC_HEADER
	DESTINATION           "${CMAKE_INSTALL_INCLUDEDIR}/Siliko"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(EXPORT Siliko
	NAMESPACE             Siliko::
	FILE                  SilikoTarget.cmake
	DESTINATION           "${CMAKEDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SilikoConfigVersion.cmake"
		"SilikoConfig.cmake"
	DESTINATION           "${CMAKEDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(FILES
		"${PROJECT_SOURCE_DIR}/Readme.md"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/Siliko"
	COMPONENT "${RUNTIME_COMPONENT}"
)

install(FILES
		"${PROJECT_SOURCE_DIR}/License.txt"
		"${PROJECT_SOURCE_DIR}/License.Gpl.txt"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/Siliko"
	COMPONENT "${RUNTIME_COMPONENT}"
)

function(siliko_fixup_bundle TGT)
	if(APPLE AND TARGET ${TGT})
		get_target_property(TGT_IS_BUNDLE ${TGT} MACOSX_BUNDLE)
	else()
		set(TGT_IS_BUNDLE No)
	endif()
	get_target_property(SILIKO_TYPE Siliko::Siliko TYPE)
	get_target_property(SILIKO_IS_FRAMEWORK Siliko::Siliko FRAMEWORK)

	if (${SILIKO_TYPE} STREQUAL SHARED_LIBRARY AND TGT_IS_BUNDLE)
		get_target_property(TGT_RPATH ${TGT} INSTALL_RPATH)
		if(NOT "@executable_path/../Frameworks" IN_LIST TGT_RPATH)
			list(APPEND TGT_RPATH "@executable_path/../Frameworks")
			set_target_properties(${TGT} PROPERTIES
				INSTALL_RPATH  ${TGT_RPATH})
		endif()
		set_target_properties(${TGT} PROPERTIES
			BUILD_WITH_INSTALL_RPATH On)

		if(SILIKO_IS_FRAMEWORK)
			add_custom_command(TARGET ${TGT} POST_BUILD
				COMMAND "cmake" "-E" "make_directory"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::Siliko>.framework/Versions/$<TARGET_PROPERTY:Siliko::Siliko,FRAMEWORK_VERSION>/Resources"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE_DIR:Siliko::Siliko>/Resources/Info.plist"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::Siliko>.framework/Versions/$<TARGET_PROPERTY:Siliko::Siliko,FRAMEWORK_VERSION>/Resources/"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE:Siliko::Siliko>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::Siliko>.framework/Versions/$<TARGET_PROPERTY:Siliko::Siliko,FRAMEWORK_VERSION>/"
				COMMAND "cmake" "-E" "create_symlink"
					"$<TARGET_PROPERTY:Siliko::Siliko,FRAMEWORK_VERSION>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::Siliko>.framework/Versions/Current"
				COMMAND "cmake" "-E" "create_symlink"
					"Versions/Current/Resources"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::Siliko>.framework/Resources"
				COMMAND "cmake" "-E" "create_symlink"
					"Versions/Current/$<TARGET_FILE_NAME:Siliko::Siliko>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::Siliko>.framework/$<TARGET_FILE_NAME:Siliko::Siliko>"
				COMMENT "Installing Siliko framework into ${TGT} bundle"
			)
		else()
			add_custom_command(TARGET ${TGT} POST_BUILD
				COMMAND "cmake" "-E" "make_directory"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE:Siliko::Siliko>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/"
				COMMAND "cmake" "-E" "create_symlink"
					"$<TARGET_FILE_NAME:Siliko::Siliko>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_SONAME_FILE_NAME:Siliko::Siliko>"
				COMMENT "Installing Siliko dylib into ${TGT} bundle"
			)
		endif()
	endif()
endfunction()