# Ogre include path
macx{
    INCLUDEPATH += ../../../../ogre/1.8.x/src/OgreMain/include/
} else {
    INCLUDEPATH += ../../../../ogre/OgreMain/include/
}

# Spacescape includes
INCLUDEPATH += ../include/
INCLUDEPATH += ../external/QtPropertyBrowser/
INCLUDEPATH += ../../SpacescapePlugin/include/

# Spacescape resources (images)
RESOURCES += Spacescape.qrc

TARGET = Spacescape
TEMPLATE = app
QT += core gui widgets

message("CONFIG : " $$CONFIG)

CONFIG(release, debug|release) {
    message("--- Release Setting ---")
    win32 {
        DESTDIR = ../app/win/release
    }
    macx {
        DESTDIR = ../app/mac/release
    }
    LIBS += ../lib/OgreMain.lib
    LIBS += ../lib/Plugin_Spacescape.lib
    LIBS += ../lib/QtSolutions_PropertyBrowser-2.5.lib    
}
else {
    message("--- Debug Setting ---")
    DESTDIR = ../app/win/debug    
    LIBS += ../lib/OgreMain_d.lib
    LIBS += ../lib/Plugin_Spacescape_d.lib
    LIBS += ../lib/QtSolutions_PropertyBrowser-2.5d.lib
    win32:CONFIG += console
    DEFINES += _DEBUG
}

SOURCES += ../src/Main.cpp \
    ../src/QtSpacescapeMainWindow.cpp \
    ../src/QtOgreWidgetOSX.cpp \
    ../src/QtSpacescapeWidget.cpp \
    ../src/QtSpacescapeExportFileDialog.cpp \
    ../src/QtSpacescapeAboutDialog.cpp
HEADERS += ../include/QtSpacescapeMainWindow.h \
    ../include/QtOgreWidgetOSX.h \
    ../include/QtSpacescapeUI.h \
    ../include/QtSpacescapeWidget.h \
    ../include/QtSpacescapeExportFileDialog.h \
    ../include/QtSpacescapeAboutDialog.h \
    ../include/QtSpacescapeAboutDialogUI.h
