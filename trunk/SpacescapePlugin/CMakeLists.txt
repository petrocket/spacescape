if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW) # remove if CMake >= 2.8.11 required
endif()

INCLUDE_DIRECTORIES(include/)

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

SET(SPCPLG_SRCS
	src/SpacescapeDll.cpp
	src/SpacescapeLayerBillboards.cpp
	src/SpacescapeLayer.cpp
	src/SpacescapeLayerNoise.cpp
	src/SpacescapeLayerPoints.cpp
	src/SpacescapeNoiseMaterial.cpp
	src/SpacescapePlugin.cpp
	src/ticpp.cpp
	src/tinystr.cpp
	src/tinyxml.cpp
	src/tinyxmlerror.cpp
	src/tinyxmlparser.cpp
)

SET(SPCPLG_HDRS
	include/SpacescapePrerequisites.h
	include/SpacescapeProgressListener.h
	include/SpacescapeLayerBillboards.h
	include/SpacescapeLayer.h
	include/SpacescapeLayerNoise.h
	include/SpacescapeLayerPoints.h
	include/SpacescapeNoiseMaterial.h
	include/SpacescapePlugin.h
	include/ticpp.h
	include/ticpprc.h
	include/tinystr.h
	include/tinyxml.h
)

AUX_SOURCE_DIRECTORY(src/ SPCPLG_SRCS)


IF(APPLE)
    ADD_LIBRARY(SpacescapePlugin SHARED ${SPCPLG_SRCS} ${SPCPLG_HDRS})
ELSE(APPLE)
    ADD_LIBRARY(SpacescapePlugin SHARED ${SPCPLG_SRCS} ${SPCPLG_HDRS})
ENDIF(APPLE)

IF(WIN32)
	SET_TARGET_PROPERTIES(SpacescapePlugin PROPERTIES OUTPUT_NAME "Plugin_Spacescape" PREFIX "" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/SpacescapePlugin"  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/SpacescapePlugin" COMPILE_DEFINITIONS "OGRE_SPACESCAPEPLUGIN_EXPORTS")
	target_link_libraries(SpacescapePlugin debug ${OGRE_LIBRARY_DBG} optimized ${OGRE_LIBRARY_REL})

	ADD_CUSTOM_COMMAND(TARGET SpacescapePlugin POST_BUILD COMMAND copy "\"$(TargetPath)\"" "\"${CMAKE_SOURCE_DIR}\\Spacescape\\app\\win\\$(OutDir)\\\"" COMMENT "Copying Spacescape_Plugin dll to Spacescape app directory")
ELSEIF(APPLE)
    SET_TARGET_PROPERTIES(SpacescapePlugin PROPERTIES OUTPUT_NAME "Plugin_Spacescape")
    SET_TARGET_PROPERTIES(SpacescapePlugin PROPERTIES VERSION ver1)
    SET_TARGET_PROPERTIES(SpacescapePlugin PROPERTIES FRAMEWORK TRUE)
	TARGET_LINK_LIBRARIES(SpacescapePlugin "${OGRE_LIBRARIES}")

    # Add the path where the Ogre framework was found
    if(NOT ${OGRE_FRAMEWORK_PATH} STREQUAL "")
      set_target_properties(SpacescapePlugin PROPERTIES
        COMPILE_FLAGS "-F${OGRE_FRAMEWORK_PATH}"
        LINK_FLAGS "-F${OGRE_FRAMEWORK_PATH}"
      )
    endif()

ELSE(WIN32)
	SET_TARGET_PROPERTIES(SpacescapePlugin PROPERTIES OUTPUT_NAME "Plugin_Spacescape" PREFIX "")
ENDIF(WIN32)

if (OGRE_THREAD_PROVIDER EQUAL 2)
	TARGET_LINK_LIBRARIES(SpacescapePlugin "${POCO_LIBRARY_REL}")
endif()
#TODO If OGRE uses a threading library like boost or POCO then we'll probably need to add it to the includes directory
