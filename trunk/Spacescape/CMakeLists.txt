INCLUDE(${QT_USE_FILE})
ADD_SUBDIRECTORY(external/QtPropertyBrowser)

INCLUDE_DIRECTORIES(include/)
INCLUDE_DIRECTORIES(external/QtPropertyBrowser/src/)
INCLUDE_DIRECTORIES(../SpacescapePlugin/include/)

LINK_DIRECTORIES("${OGRE_LIBRARY_DIR_DBG}" "${OGRE_LIBRARY_DIR_REL}")

if(POCO_INCLUDE_DIR)
	INCLUDE_DIRECTORIES(${POCO_INCLUDE_DIR})
endif(POCO_INCLUDE_DIR)

if(POCO_DIR)
	LINK_DIRECTORIES(${POCO_DIR}/lib)
endif(POCO_DIR)

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

QT4_WRAP_CPP(SPC_MOC_SRCS ${SPC_MOC_HDRS})

IF(UNIX)
  ADD_EXECUTABLE(Spacescape ${SPC_SRCS} ${SPC_MOC_SRCS} ${SPC_MOC_HDRS})
ELSEIF(APPLE)
  ADD_EXECUTABLE(Spacescape MACOSX_BUNDLE ${SPC_SRCS} ${SPC_MOC_SRCS} ${SPC_MOC_HDRS})
ELSEIF(WIN32)
  ADD_EXECUTABLE(Spacescape WIN32 ${SPC_SRCS} ${SPC_MOC_SRCS} ${SPC_MOC_HDRS})
ENDIF()

TARGET_LINK_LIBRARIES(Spacescape SpacescapePlugin ${OGRE_LIBRARIES} ${QT_LIBRARIES} debug ${QtPropertyBrowser_Library_DBG} optimized ${QtPropertyBrowser_Library_REL})

if(WIN32)
   set_target_properties(Spacescape PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(Spacescape PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   set_target_properties(Spacescape PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(Spacescape PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(Spacescape PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(Spacescape PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
else(WIN32)
	SET_TARGET_PROPERTIES(Spacescape PROPERTIES SUFFIX ".bin")
endif(WIN32)