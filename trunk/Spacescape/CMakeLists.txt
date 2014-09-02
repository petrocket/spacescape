# Also includes the include dirs for dependencies QtGui & QtCore
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDES})

# Needed for Qt 5
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})

ADD_SUBDIRECTORY(external/QtPropertyBrowser5)

INCLUDE_DIRECTORIES(include/)
INCLUDE_DIRECTORIES(external/QtPropertyBrowser5/src/)
INCLUDE_DIRECTORIES(../SpacescapePlugin/include/)

LINK_DIRECTORIES("${OGRE_LIBRARY_DIR_DBG}" "${OGRE_LIBRARY_DIR_REL}")

if(POCO_INCLUDE_DIR)
	INCLUDE_DIRECTORIES(${POCO_INCLUDE_DIR})
endif(POCO_INCLUDE_DIR)

if(POCO_LIBRARY_DBG)
	get_filename_component(POCO_LIBRARY_DIR ${POCO_LIBRARY_DBG} PATH)
	LINK_DIRECTORIES(${POCO_LIBRARY_DIR})
elseif(POCO_LIBRARY_REL)
	get_filename_component(POCO_LIBRARY_DIR ${POCO_LIBRARY_REL} PATH)
	LINK_DIRECTORIES(${POCO_LIBRARY_DIR})
endif(POCO_LIBRARY_DBG)

IF(APPLE)
    # don't glob, we don't want all .h files - some are windows specific
    SET(SPC_SRCS
        src/Main.cpp
        src/QtOgreWidgetOSX.cpp
        src/QtSpacescapeAboutDialog.cpp
        src/QtSpacescapeExportFileDialog.cpp
        src/QtSpacescapeMainWindow.cpp
        src/QtSpacescapeWidget.cpp
        src/QtFilePathProperty.cpp
    )
    SET(SPC_MOC_HDRS
        include/QtOgreWidgetOSX.h
        include/QtSpacescapeAboutDialog.h
        include/QtSpacescapeAboutDialogUI.h
        include/QtSpacescapeExportFileDialog.h
        include/QtSpacescapeMainWindow.h
        include/QtSpacescapeUI.h
        include/QtSpacescapeWidget.h
        include/QtFilePathProperty.h
    )

    # FILE(GLOB_RECURSE SPC_SRCS src/*.cpp)

    # FILE(GLOB_RECURSE SPC_MOC_HDRS include/*.h)

    # Locate all resourses
    FILE(GLOB COPY_RESOURCES app/mac/*.cfg)

    # MESSAGE("OGRE FRAMEWORK PATH = ${OGRE_FRAMEWORK_PATH}")

    # Find all frameworks
    SET(COPY_FRAMEWORKS  
        ${OGRE_RenderSystem_GL_LIBRARY_REL} 
        ${OGRE_FRAMEWORK_PATH}
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}Release/Plugin_Spacescape.framework"
        )
    SET_SOURCE_FILES_PROPERTIES(
        ${COPY_FRAMEWORKS}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Frameworks
        )
        
    # MESSAGE("OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}Debug/")

    FILE(GLOB_RECURSE MEDIA media/*)
    
    SET_SOURCE_FILES_PROPERTIES(
        ${MEDIA}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources/media/materials/textures
        )


ELSE(APPLE)
    SET(SPC_SRCS
        src/Main.cpp
        src/QtOgreWidget.cpp
        src/QtSpacescapeAboutDialog.cpp
        src/QtSpacescapeExportFileDialog.cpp
        src/QtSpacescapeMainWindow.cpp
        src/QtSpacescapeWidget.cpp
    )
    SET(SPC_MOC_HDRS
        include/QtOgreWidget.h
        include/QtSpacescapeAboutDialog.h
        include/QtSpacescapeAboutDialogUI.h
        include/QtSpacescapeExportFileDialog.h
        include/QtSpacescapeMainWindow.h
        include/QtSpacescapeUI.h
        include/QtSpacescapeWidget.h
    )
ENDIF(APPLE)

SET(SPC_RESOURCES 
	scripts/Spacescape.qrc
)

QT5_ADD_RESOURCES(SPC_RESOURCES_RCC ${SPC_RESOURCES})


IF(APPLE)
  SET(MACOSX_BUNDLE_BUNDLE_NAME Spacescape)
  ADD_EXECUTABLE(Spacescape MACOSX_BUNDLE ${SPC_MOC_HDRS} ${SPC_SRCS} ${SPC_MOC_SRCS} ${SPC_RESOURCES_RCC} ${COPY_RESOURCES} ${COPY_FRAMEWORKS} ${MEDIA})
  SOURCE_GROUP(Frameworks FILES ${COPY_FRAMEWORKS})
  SOURCE_GROUP("media/materials/textures" FILES ${MEDIA})

ELSEIF(UNIX)
  ADD_EXECUTABLE(Spacescape ${SPC_SRCS} ${SPC_MOC_SRCS} ${SPC_MOC_HDRS} ${SPC_RESOURCES_RCC})
ELSEIF(WIN32)
  ADD_EXECUTABLE(Spacescape WIN32 ${SPC_SRCS} ${SPC_MOC_SRCS} ${SPC_MOC_HDRS} ${SPC_RESOURCES_RCC})
ENDIF()

TARGET_LINK_LIBRARIES(Spacescape SpacescapePlugin "${OGRE_LIBRARIES}" Qt5::Widgets debug ${QtPropertyBrowser_Library_DBG} optimized ${QtPropertyBrowser_Library_REL})

IF(WIN32)
   set_target_properties(Spacescape PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(Spacescape PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   set_target_properties(Spacescape PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(Spacescape PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(Spacescape PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(Spacescape PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
ELSEIF(APPLE)
    # Add the path where the Ogre framework was found
    if(NOT ${OGRE_FRAMEWORK_PATH} STREQUAL "")
      set_target_properties(Spacescape PROPERTIES
        COMPILE_FLAGS "-F${OGRE_FRAMEWORK_PATH}"
        LINK_FLAGS "-F${OGRE_FRAMEWORK_PATH}"
      )
    endif()
    
    SET_TARGET_PROPERTIES(Spacescape PROPERTIES MACOSX_BUNDLE TRUE)

    add_custom_command(TARGET Spacescape PRE_BUILD
            COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Spacescape.app/Contents/Resources COMMENT "Creating Resources directory"
            )

    # MESSAGE( "COPY_RESOURCES = ${COPY_RESOURCES}")

    # Add resources to application bundle target
    SET_TARGET_PROPERTIES(Spacescape PROPERTIES RESOURCE "${COPY_RESOURCES}")

    SET_SOURCE_FILES_PROPERTIES(
        "${COPY_RESOURCES}"
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
        )
        
    SET_TARGET_PROPERTIES(Spacescape PROPERTIES XCODE_ATTRIBUTE_OGRE_SDK_ROOT "${OGRE_FRAMEWORK_PATH}/../../../../")

ELSE(WIN32)
	SET_TARGET_PROPERTIES(Spacescape PROPERTIES SUFFIX ".bin")
endif(WIN32)
