echo 'Fixing IDs'

cd /Users/alex/Documents/workspace/spacescape/spacescape-git/trunk/Spacescape/scripts

# Fix IDs
install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../lib/QtGui.framework/QtGui
install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../lib/QtCore.framework/QtCore
install_name_tool -id @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ../lib/QtWidgets.framework/QtWidgets
install_name_tool -id @executable_path/../Frameworks/libQtSolutions_PropertyBrowser-head.1.0.0.dylib ../external/QtPropertyBrowser5/lib/libQtSolutions_PropertyBrowser-head.1.0.0.dylib
install_name_tool -id @executable_path/../Plugins/platforms/libqcocoa.dylib ../lib/libqcocoa.dylib
install_name_tool -id @executable_path/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport ../lib/QtPrintSupport.framework/QtPrintSupport

echo 'Fixing Framework paths'

# Fix libqcocoa Paths
# DEBUG
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../app/mac/Debug/Spacescape.app/Contents/PlugIns/platforms/libqcocoa.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ../app/mac/Debug/Spacescape.app/Contents/PlugIns/platforms/libqcocoa.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../app/mac/Debug/Spacescape.app/Contents/PlugIns/platforms/libqcocoa.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport ../app/mac/Debug/Spacescape.app/Contents/PlugIns/platforms/libqcocoa.dylib

# LIB
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../lib/libqcocoa.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ../lib/libqcocoa.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../lib/libqcocoa.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport ../lib/libqcocoa.dylib

# Fix PrintSupport Paths
# DEBUG
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../app/mac/Debug/Spacescape.app/Contents/Frameworks/QtPrintSupport.framework/QtPrintSupport
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ../app/mac/Debug/Spacescape.app/Contents/Frameworks/QtPrintSupport.framework/QtPrintSupport
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../app/mac/Debug/Spacescape.app/Contents/Frameworks/QtPrintSupport.framework/QtPrintSupport

# LIB
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../lib/QtPrintSupport.framework/QtPrintSupport
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ../lib/QtPrintSupport.framework/QtPrintSupport
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../lib/QtPrintSupport.framework/QtPrintSupport



# Fix QtGui Path
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../app/mac/Debug/Spacescape.app/Contents/Frameworks/QtGui.framework/QtGui
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../lib/QtGui.framework/QtGui

# Fix QtWidgets Path
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../app/mac/Debug/Spacescape.app/Contents/Frameworks/QtWidgets.framework/QtWidgets
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../app/mac/Debug/Spacescape.app/Contents/Frameworks/QtWidgets.framework/QtWidgets
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../lib/QtWidgets.framework/QtWidgets
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../lib/QtWidgets.framework/QtWidgets

# Fix QtPropertyBrowser Path
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../app/mac/Debug/Spacescape.app/Contents/Frameworks/libQtSolutions_PropertyBrowser-head.1.0.0.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ../app/mac/Debug/Spacescape.app/Contents/Frameworks/libQtSolutions_PropertyBrowser-head.1.0.0.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../app/mac/Debug/Spacescape.app/Contents/Frameworks/libQtSolutions_PropertyBrowser-head.1.0.0.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../external/QtPropertyBrowser5/lib/libQtSolutions_PropertyBrowser-head.1.0.0.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ../external/QtPropertyBrowser5/lib/libQtSolutions_PropertyBrowser-head.1.0.0.dylib
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../external/QtPropertyBrowser5/lib/libQtSolutions_PropertyBrowser-head.1.0.0.dylib

echo 'Fixing Spacescape paths'

# Fix Spacescape Paths  - Release
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../app/mac/Release/Spacescape.app/Contents/MacOS/Spacescape
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../app/mac/Release/Spacescape.app/Contents/MacOS/Spacescape
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ../app/mac/Release/Spacescape.app/Contents/MacOS/Spacescape
install_name_tool -change /Users/alex/Documents/workspace/spacescape/spacescape-git/trunk/Spacescape/external/QtPropertyBrowser5/lib/libQtSolutions_PropertyBrowser-head.1.dylib @executable_path/../Frameworks/libQtSolutions_PropertyBrowser-head.1.0.0.dylib ../app/mac/Release/Spacescape.app/Contents/MacOS/Spacescape

# Fix Spacescape Paths  - Debug
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ../app/mac/Debug/Spacescape.app/Contents/MacOS/Spacescape
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ../app/mac/Debug/Spacescape.app/Contents/MacOS/Spacescape
install_name_tool -change /Users/alex/Qt/5.3/clang_64/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ../app/mac/Debug/Spacescape.app/Contents/MacOS/Spacescape
install_name_tool -change /Users/alex/Documents/workspace/spacescape/spacescape-git/trunk/Spacescape/external/QtPropertyBrowser5/lib/libQtSolutions_PropertyBrowser-head.1.dylib @executable_path/../Frameworks/libQtSolutions_PropertyBrowser-head.1.0.0.dylib ../app/mac/Debug/Spacescape.app/Contents/MacOS/Spacescape

# Revert
#install_name_tool -id /Users/alex/Documents/workspace/spacescape/spacescape-git/trunk/Spacescape/external/QtPropertyBrowser5/lib/libQtSolutions_PropertyBrowser-head.1.dylib ../external/QtPropertyBrowser5/lib/libQtSolutions_PropertyBrowser-head.1.0.0.dylib
#install_name_tool -change  @executable_path/../Frameworks/libQtSolutions_PropertyBrowser-head.1.0.0.dylib /Users/alex/Documents/workspace/spacescape/spacescape-git/trunk/Spacescape/external/QtPropertyBrowser5/lib/libQtSolutions_PropertyBrowser-head.1.dylib ../app/mac/Debug/Spacescape.app/Contents/MacOS/Spacescape


#otool -L ../app/mac/Release/Spacescape.app/Contents/MacOS/Spacescape
otool -L ../app/mac/Debug/Spacescape.app/Contents/MacOS/Spacescape

echo 'All done'
