INCLUDE(${QT_USE_FILE})
ADD_SUBDIRECTORY(external/QtPropertyBrowser)

INCLUDE_DIRECTORIES(include/)
INCLUDE_DIRECTORIES(external/QtPropertyBrowser/src/)
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
SET(SPC_SRCS
	src/Main.cpp
	src/QtOgreWidgetOSX.cpp
	src/QtSpacescapeAboutDialog.cpp
	src/QtSpacescapeExportFileDialog.cpp
	src/QtSpacescapeMainWindow.cpp
	src/QtSpacescapeWidget.cpp
)
SET(SPC_MOC_HDRS
	include/QtOgreWidgetOSX.h
	include/QtSpacescapeAboutDialog.h
	include/QtSpacescapeAboutDialogUI.h
	include/QtSpacescapeExportFileDialog.h
	include/QtSpacescapeMainWindow.h
	include/QtSpacescapeUI.h
	include/QtSpacescapeWidget.h
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

SET(QT_USE_QTGUI 1)
SET(QT_USE_QTSCRIPT 1)
SET(QT_USE_QTXML 1)

INCLUDE(${QT_USE_FILE})

QT4_WRAP_CPP(SPC_MOC_SRCS ${SPC_MOC_HDRS})
QT4_ADD_RESOURCES(SPC_RESOURCES_RCC ${SPC_RESOURCES})

IF(UNIX)
  ADD_EXECUTABLE(Spacescape ${SPC_SRCS} ${SPC_MOC_SRCS} ${SPC_MOC_HDRS} ${SPC_RESOURCES_RCC})
ELSEIF(APPLE)
  ADD_EXECUTABLE(Spacescape MACOSX_BUNDLE ${SPC_SRCS} ${SPC_MOC_SRCS} ${SPC_MOC_HDRS} ${SPC_RESOURCES_RCC} )
ELSEIF(WIN32)
  ADD_EXECUTABLE(Spacescape WIN32 ${SPC_SRCS} ${SPC_MOC_SRCS} ${SPC_MOC_HDRS} ${SPC_RESOURCES_RCC})
ENDIF()

TARGET_LINK_LIBRARIES(Spacescape SpacescapePlugin "${OGRE_LIBRARIES}" ${QT_LIBRARIES} debug ${QtPropertyBrowser_Library_DBG} optimized ${QtPropertyBrowser_Library_REL})

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
ELSE(WIN32)
	SET_TARGET_PROPERTIES(Spacescape PROPERTIES SUFFIX ".bin")
endif(WIN32)