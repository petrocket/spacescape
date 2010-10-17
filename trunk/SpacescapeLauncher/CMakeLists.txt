INCLUDE_DIRECTORIES(include/)

SET(SPC_SRCS
	src/SpacescapeLauncher.cpp
	src/stdafx.cpp
	resources/SpacescapeLauncher.ico
	resources/SpacescapeLauncher.rc
)

SET(SPC_HDRS
	include/Resource.h
	include/SpacescapeLauncher.h
	include/stdafx.h
	include/targetver.h
)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/SpacescapeLauncher)

ADD_EXECUTABLE(SpacescapeLauncher WIN32 ${SPC_SRCS} ${SPC_HDRS})

ADD_CUSTOM_COMMAND(TARGET SpacescapeLauncher POST_BUILD COMMAND copy "\"$(TargetPath)\"" "\"$(TargetDir)..\\..\\Spacescape\\Spacescape.exe\"" COMMENT "Copying launcher .exe")


