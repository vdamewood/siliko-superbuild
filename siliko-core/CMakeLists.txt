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

# These are specific to this project, and are not a standard practice.
# These values are appended to the version number in the
# versioning API and used by the version resource file for Windows
# binaries. They are set here to keep version information in as close to
# one location as possible.

# SILIKO_CORE_PRERELEASE_SUFFIX is set by project maintainers.
# SILIKO_CORE_PACKAGE_SUFFIX is set by package maintainers at
#  packaging time.

# Values for SILIKO_CORE_PRERELEASE_SUFFIX:
# "indev" is used for SCM commits that aren't releases of any sort.
# "beta." followed by an unsigned integer (eg "beta.2") is used for beta
#     releases.
# "rc." followed by an unsigned integer (eg "rc.1") is used for release
#     candidates
# The empty string ("") is used for full releases.
set(SILIKO_CORE_PRERELEASE_SUFFIX "indev" CACHE INTERNAL
	"Suffix to append to prerelease versions")

# Follow your package repository's guidelines when setting this value.
set(SILIKO_CORE_PACKAGE_SUFFIX "" CACHE STRING
	"Suffix to append to packaged versions by package maintainers"
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

if(SILIKO_CORE_BUILD_FRAMEWORK)
	set(CMAKEDIR "${SILIKO_CORE_INSTALL_FRAMEWORKDIR}/SilikoCore.framework/Resources")
	set(RUNTIME_COMPONENT "SilikoCore")
	set(DEVELOPMENT_COMPONENT "SilikoCore")
else()
	set(CMAKEDIR "${SILIKO_CORE_INSTALL_CMAKEDIR}")
	set(RUNTIME_COMPONENT "SilikoCore_Runtime")
	set(DEVELOPMENT_COMPONENT "SilikoCore_Development")
endif()


find_library(LIBM m)

add_library(SilikoCore)
add_library(Siliko::SilikoCore ALIAS SilikoCore)
target_sources(SilikoCore
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
			include
			${CMAKE_CURRENT_BINARY_DIR}/include
		FILES
			include/SilikoCore/Value.h
			include/SilikoCore/FunctionCaller.h
			include/SilikoCore/SyntaxTree.h
			include/SilikoCore/DataSource.h
			include/SilikoCore/StringSource.h
			include/SilikoCore/Token.h
			include/SilikoCore/Lexer.h
			include/SilikoCore/InfixParser.h
			${CMAKE_CURRENT_BINARY_DIR}/include/SilikoCore/Api.h
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
if(LIBM)
	target_link_libraries(SilikoCore PRIVATE m)
endif()

target_compile_features(SilikoCore PRIVATE c_std_11)
get_target_property(PUBLIC_HEADERS SilikoCore HEADER_SET_HEADERS)
set_target_properties(SilikoCore PROPERTIES
	PUBLIC_HEADER             "${PUBLIC_HEADERS}"
	C_VISIBILITY_PRESET       hidden
	VISIBILITY_INLINES_HIDDEN On
	FRAMEWORK                 "${SILIKO_CORE_BUILD_FRAMEWORK}"
	FRAMEWORK_VERSION         ${PROJECT_VERSION}
	VERSION                   ${PROJECT_VERSION}
	SOVERSION                 ${PROJECT_VERSION_MAJOR}
)

if(WIN32)
	if(SILIKO_CORE_PRERELEASE_SUFFIX STREQUAL "")
		set(VSV_PRERELEASE_FLAG 0)
	else()
		set(VSV_PRERELEASE_FLAG VS_FF_PRERELEASE)
	endif()

	set(VSV_DEBUG_FLAG 0)
	set(VSV_FILENAME "SilikoCore.dll")
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-release.rc
		@ONLY
  	)
	target_sources(SilikoCore PRIVATE $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}/version-release.rc>)

	set(VSV_DEBUG_FLAG VS_FF_DEBUG)
	set(VSV_FILENAME "SilikoCore.dll")
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc
		@ONLY
  	)
	target_sources(SilikoCore PRIVATE $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc>)
endif()

include(GenerateExportHeader)
generate_export_header(SilikoCore
	EXPORT_FILE_NAME include/SilikoCore/Api.h
)

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
	COMPONENT             "${RUNTIME_COMPONENT}"
ARCHIVE
	DESTINATION           "${CMAKE_INSTALL_LIBDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
LIBRARY
	DESTINATION           "${CMAKE_INSTALL_LIBDIR}"
	COMPONENT             "${RUNTIME_COMPONENT}"
	NAMELINK_COMPONENT    "${DEVELOPMENT_COMPONENT}"
FRAMEWORK
	DESTINATION           "${SILIKO_CORE_INSTALL_FRAMEWORKDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
FILE_SET HEADERS
	DESTINATION           "${CMAKE_INSTALL_INCLUDEDIR}/SilikoCore"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(EXPORT SilikoCore
	NAMESPACE             Siliko::
	FILE                  SilikoCoreTarget.cmake
	DESTINATION           "${CMAKEDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SilikoCoreConfigVersion.cmake"
		"SilikoCoreConfig.cmake"
	DESTINATION           "${CMAKEDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(FILES
		"${PROJECT_SOURCE_DIR}/Readme.md"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/SilikoCore"
	COMPONENT "${RUNTIME_COMPONENT}"
)

install(FILES
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
