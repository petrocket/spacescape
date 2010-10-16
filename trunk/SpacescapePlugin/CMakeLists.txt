INCLUDE_DIRECTORIES(include/)

if(POCO_INCLUDE_DIR)
	INCLUDE_DIRECTORIES(${POCO_INCLUDE_DIR})
endif(POCO_INCLUDE_DIR)

if(POCO_DIR)
	LINK_DIRECTORIES(${POCO_DIR}/lib)
endif(POCO_DIR)

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

ADD_LIBRARY(SpacescapePlugin SHARED ${SPCPLG_SRCS} ${SPCPLG_HDRS})
IF(WIN32)
	SET_TARGET_PROPERTIES(SpacescapePlugin PROPERTIES OUTPUT_NAME "Plugin_Spacescape" PREFIX "" COMPILE_DEFINITIONS "OGRE_SPACESCAPEPLUGIN_EXPORTS")
	target_link_libraries(SpacescapePlugin debug ${OGRE_LIBRARY_DBG} optimized ${OGRE_LIBRARY_REL})
ELSE(WIN32)
	SET_TARGET_PROPERTIES(SpacescapePlugin PROPERTIES OUTPUT_NAME "Plugin_Spacescape" PREFIX "")
ENDIF(WIN32)

#TODO If OGRE uses a threading library like boost or POCO then we'll probably need to add it to the includes directory
