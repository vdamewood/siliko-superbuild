include("${CMAKE_CURRENT_LIST_DIR}/SilikoCoreTarget.cmake")

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
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::SilikoCore>.framework/Versions/${SilikoCore_VERSION}/Resources"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE_DIR:Siliko::SilikoCore>/Resources/Info.plist"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::SilikoCore>.framework/Versions/${SilikoCore_VERSION}/Resources/"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE:Siliko::SilikoCore>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Siliko::SilikoCore>.framework/Versions/${SilikoCore_VERSION}/"
				COMMAND "cmake" "-E" "create_symlink"
					"${SilikoCore_VERSION}"
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
