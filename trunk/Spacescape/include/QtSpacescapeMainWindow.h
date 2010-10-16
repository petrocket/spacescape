/* 
This source file is part of Spacescape
For the latest info, see http://alexcpeterson.com/spacescape

"He determines the number of the stars and calls them each by name. "
Psalm 147:4

The MIT License

Copyright (c) 2010 Alex Peterson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef __QTSPACESCAPEMAINWINDOW_H__
#define __QTSPACESCAPEMAINWINDOW_H__

#include <QMainWindow>
#include <QtProperty>
#include <QtVariantPropertyManager>
#if defined(Q_WS_WIN)
#pragma warning( push ) 
#pragma warning( disable : 4100 )
#endif
#include "OgrePrerequisites.h"
#include "SpacescapeLayer.h"
#include "SpacescapeProgressListener.h"
#if defined(Q_WS_WIN)
#pragma warning( pop )
#endif

namespace Ui {
    class QtSpacescapeUI;
}

/** QtSpacescapeMainWindow is the main UI window for Spacescape.
*/
class QtSpacescapeMainWindow : public QMainWindow, Ogre::SpacescapeProgressListener {
    Q_OBJECT
public:
    /** Constructor
    @param parent
    */
    QtSpacescapeMainWindow(QWidget *parent = 0);

    /** Destructor
    */
    ~QtSpacescapeMainWindow();

    /** Function used to update the progress bar
    @param percentComplete The percentage complete 0 - 100
    @param msg The current task message
    */
    void updateProgressBar(unsigned int percentComplete, const Ogre::String& msg);

    // property manager
    QtVariantPropertyManager* mPropertyManager;

protected:
    /** A change event was received
    @param e The event parameters
    */
    void changeEvent(QEvent *e);

public Q_SLOTS:
    /** Current selected layer property has changed so update the status bar
    with this property's tip if it has one.
    @param item The selected item
    */
    void currentItemChanged(QtBrowserItem *item);

    /** The about action was clicked
    */
    void onAbout();

    /** The copy layer button was clicked
    */
    void onCopyLayerClicked();

    /** The delete layer button was clicked
    */
    void onDeleteLayerClicked();

    /** The export action was clicked
    */
    void onExport();

    /** The move down button was clicked
    */
    void onMoveLayerDown();

    /** The move up button was clicked
    */
    void onMoveLayerUp();

    /** The new file action was clicked
    */
    void onNewFile();

    /** The new layer button was clicked
    */
    void onNewLayerClicked();

    /* The open action was clicked
    */
    void onOpen();

    /** The save action was clicked
    */
    void onSave();

    /** The save as action was clicked
    */
    void onSaveAs();

    /** A layer property was changed in the UI. Update the layer and 
    make any necessary UI changes.
    @param property The changed property name
    @param value The new value.
    */
    void valueChanged(QtProperty *property, const QVariant &value);

private:
    /** Utility function for creating a property from a key/value pair
    @param key The key
    @param value The value
    @return the created QtProperty
    */
    QtVariantProperty* createProperty(const Ogre::String& key, const Ogre::String& value);

    /** Utility function to convert a blend mode string to int
    @param param blend mode string like "one" or "dest_colour"
    */
    int getBlendMode(const QString& param);

    /** Utility function to convert an ogre color to a string
    @param color String color with values between 0 and 1
    */
    QColor getColor(const Ogre::String& color);

    /** Utility function to convert an ogre color to a string
    @param color String color with values between 0 and 255
    */
    Ogre::ColourValue getColor(const QString& color);

    /** Utility function to convert an QColor color to a Ogre string
    @param color String color with values between 0 and 1
    */
    Ogre::String getColor(QColor color);

    /** Utility function to convert between string types and property names and titles
    For example you provide Dest Blend Factor and get destBlendFactor
    @param prop - the property to get the xml var for for
    */
    Ogre::String getProperty(const QString& prop);

    /** Utility function to get the status tip for a property type
    @param prop - the property to get the status tip for
    */
    QLatin1String getPropertyStatusTip(const Ogre::String& prop);

    /** Utility function to convert between string types and property names and titles
    For example you provied destBlendFactor and get Dest Blend Factor
    @param prop - the property to get the title for
    */
    QString getPropertyTitle(const Ogre::String& prop);

    /** Utility function to get the property type by name
    @param name The name of the property i.e. seed or maskEnabled etc.
    @return the QVariant type
    */
    int getPropertyType(const Ogre::String& name);

    /** Utiltiy function for retrieving the selected layer id
    @return int The id of the selected layer or -1
    */
    int getSelectedLayerId();

    /** Utility function for creating and inserting layer properties
    @param layer Spacescape layer
    @param insertAfter Property to insert after
    @param minimize Minimize this layer
    @return The created / inserted property
    */
    QtProperty* insertLayerProperties(Ogre::SpacescapeLayer* layer, QtProperty *insertAfter = NULL, bool minimize = true);

    /** Utility function for refreshing the entire layer property tree
    */
    void refreshProperties();

    // the user interface
    Ui::QtSpacescapeUI *ui;

    // ui refreshing flag
    bool mRefreshing;

    // open filename
    QString mFilename;

    // last export directory path
    QString mLastExportDir;

    // last open directory path
    QString mLastOpenDir;

    // last save directory path
    QString mLastSaveDir;

    // mapping of ogre property names to readable titles
    std::map<Ogre::String, QString> mPropertyTitles;
};

#endif // __QTSPACESCAPEMAINWINDOW_H__
