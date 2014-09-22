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
#include "QtSpacescapeMainWindow.h"
#include "QtSpacescapeUI.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "QFileDialog"
#include <QProgressDialog>
#include "QtSpacescapeExportFileDialog.h"
#include "QtSpacescapeAboutDialog.h"
#include <QSettings>
#include "QtFilePathProperty.h"

//#include "OGRE/Ogre.h"
#include <Ogre.h>

/** Constructor
@param parent
*/
QtSpacescapeMainWindow::QtSpacescapeMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtSpacescapeUI),
    mFilename(""),
    mRefreshing(false)
{
    setAttribute(Qt::WA_NativeWindow);
    
	QCoreApplication::setOrganizationName("Spacescape");
	QCoreApplication::setOrganizationDomain("alexcpeterson.com");
	QCoreApplication::setApplicationName("Spacescape");

	// we store last settings used in registry/settings
	QSettings settings;

    mLastExportDir = settings.value("LastExportDir","../../../export/").toString();
	mLastOpenDir = settings.value("LastOpenDir","../../../save/").toString();
	mLastSaveDir = settings.value("LastSaveDir","../../../save/").toString();

    // setup the ui
    ui->setupUi(this);

    // set the property manager to a variant type (changable/editable - not read only)
    mPropertyManager = new QtVariantPropertyManager();
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
    ui->layerProperties->setFactoryForManager(mPropertyManager, variantFactory);
    ui->layerProperties->setPropertiesWithoutValueMarked(true);

    // we want the little arrows on root properties to indicate they're expandable
    ui->layerProperties->setRootIsDecorated(true);

    // set us as a progress listener
    ui->ogreWindow->setProgressListener(this);

    // add a signal for when properties are changed
    connect(mPropertyManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(valueChanged(QtProperty *, const QVariant &)));

    // add a signal to detect when layer items change so we can update status bar
    // with a tip
    connect(ui->layerProperties, SIGNAL(currentItemChanged(QtBrowserItem*)),
            this,SLOT(currentItemChanged(QtBrowserItem*)));

    // add a signal for when the open menu item is selected
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(onOpen()));

    // add a signal for when the save menu item is selected
    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(onSave()));

    // add a signal for when the save menu item is selected
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(onSaveAs()));

    // add a signal for when the about menu item is selected
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onAbout()));

    
    mPropertyTitles["destBlendFactor"] = QString("Dest Blend Factor");
    mPropertyTitles["ditherAmount"] = QString("Dither Amount");
    mPropertyTitles["farColor"] = QString("Far Color");
    mPropertyTitles["gain"] = QString("Gain");
    mPropertyTitles["innerColor"] = QString("Inner Color");
    mPropertyTitles["lacunarity"] = QString("Lacunarity");
    // in non-hdr mode color fades linearly based on distance
    // but in hdr mode we may want other options
    mPropertyTitles["hdrPower"] = QString("HDR Power");
    
    // option multiplier to use in HDR mode
    mPropertyTitles["hdrMultiplier"] = QString("HDR Multiplier");
    mPropertyTitles["maskEnabled"] = QString("Mask Enabled");
    mPropertyTitles["maskGain"] = QString("Mask Gain");
    mPropertyTitles["maskInnerColor"] = QString("Mask Inner Color");
    mPropertyTitles["maskLacunarity"] = QString("Mask Lacunarity");
    mPropertyTitles["maskNoiseType"] = QString("Mask Noise Type");
    mPropertyTitles["maskOffset"] = QString("Mask Offset");
    mPropertyTitles["maskOctaves"] = QString("Mask Octaves");
    mPropertyTitles["maskOuterColor"] = QString("Mask Outer Color");
    mPropertyTitles["maskPower"] = QString("Mask Power");
    mPropertyTitles["maskScale"] = QString("Mask Noise Scale");
    mPropertyTitles["maskSeed"] = QString("Mask Random Seed");
    mPropertyTitles["maskThreshold"] = QString("Mask Threshold");
    mPropertyTitles["maxSize"] = QString("Near Billboard Size");
    mPropertyTitles["minSize"] = QString("Far Billboard Size");
    mPropertyTitles["name"] = QString("Layer Name");
    mPropertyTitles["nearColor"] = QString("Near Color");
    mPropertyTitles["noiseType"] = QString("Noise Type");
    mPropertyTitles["numBillboards"] = QString("Number of Billboards");
    mPropertyTitles["dataFile"] = QString("Data File");
    mPropertyTitles["numPoints"] = QString("Number of Points");
    mPropertyTitles["octaves"] = QString("Octaves");
    mPropertyTitles["offset"] = QString("Noise Offset");
    mPropertyTitles["outerColor"] = QString("Outer Color");
    mPropertyTitles["pointSize"] = QString("Point Size");
    mPropertyTitles["powerAmount"] = QString("Power");
    mPropertyTitles["previewTextureSize"] = QString("Preview Texture Size");
    mPropertyTitles["texture"] = QString("Billboard Texture");
    mPropertyTitles["type"] = QString("Layer Type");
    mPropertyTitles["scale"] = QString("Noise Scale");
    mPropertyTitles["seed"] = QString("Random Seed");
    mPropertyTitles["shelfAmount"] = QString("Threshold");
    mPropertyTitles["sourceBlendFactor"] = QString("Source Blend Factor");
    mPropertyTitles["visible"] = QString("Layer Visible");
    
    mDebugLayerLoaded = false;
}

/** Destructor
*/
QtSpacescapeMainWindow::~QtSpacescapeMainWindow()
{
    delete ui;
}

void QtSpacescapeMainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    if(!mDebugLayerLoaded &&
       ui->ogreWindow->pluginReady()) {
#ifdef _DEBUG
         Ogre::NameValuePairList params;
         ui->ogreWindow->addLayer(1,params);
         
         // insert the new layer
         insertLayerProperties(ui->ogreWindow->getLayers().back());
#endif
        mDebugLayerLoaded = true;
    }
}


/** A change event was received
@param e The event parameters
*/
void QtSpacescapeMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/** Utility function for creating a property from a key/value pair
@param key The key
@param value The value
@return the created QtProperty
*/
QtVariantProperty* QtSpacescapeMainWindow::createProperty(const Ogre::String& key, const Ogre::String& value)
{
    QStringList noiseTypes, layerTypes, blendTypes, textureSizes, billboardTextures;
    noiseTypes << "fbm" << "ridged";
    layerTypes << "points" << "billboards" << "noise";
    blendTypes << "one" << "zero" << "dest_colour" << "src_colour" 
        << "one_minus_dest_colour" << "one_minus_src_colour" 
        << "dest_alpha" << "src_alpha" << "one_minus_dest_alpha" 
        << "one_minus_src_alpha";
    textureSizes << "256" << "512" << "1024" << "2048" << "4096";
    if(ui->ogreWindow->isHDREnabled()) {
        billboardTextures << "hdr-flare-blue.exr" << "hdr-flare-red.exr" << "hdr-flare-yellow.exr" << "hdr-flare-purple.exr" << "hdr-flare-white.exr"  << "hdr-flare-white2.exr";
        billboardTextures << "hdr-flare-blue-2spires.exr" << "hdr-flare-blue-4spires.exr" << "hdr-flare-blue1.exr" << "hdr-flare-bluepurple-4spire.exr" << "hdr-flare-bluepurple-multispire.exr" << "hdr-flare-pink1.exr" << "hdr-flare-red-2spires.exr" << "hdr-flare-redpurple-multispire.exr";
    }
    else {
        billboardTextures << "default.png" << "flare-blue-purple1.png" << "flare-blue-purple2.png" << "flare-blue-purple3.png" << "flare-blue-spikey1.png" << "flare-green1.png" << "flare-inverted-blue-purple3.png" << "flare-red-yellow1.png" << "flare-red1.png" << "flare-white-small1.png" << "sun.png";
    }
    int propertyType = getPropertyType(key);
    QtVariantProperty* property;

    property = mPropertyManager->addProperty(propertyType, getPropertyTitle(key));
    property->setStatusTip(getPropertyStatusTip(key));
    property->setToolTip(getPropertyStatusTip(key));

    if(propertyType == QVariant::Int) {
        property->setValue(Ogre::StringConverter::parseInt(value));
        property->setAttribute(QLatin1String("minimum"), 0);
        property->setAttribute(QLatin1String("singleStep"), 1);
    }
    else if(propertyType == QVariant::Bool) {
        property->setValue(Ogre::StringConverter::parseBool(value));
    }
    else if(propertyType == QtVariantPropertyManager::enumTypeId()) {
        QStringList *enumList = NULL;
        if(key == "destBlendFactor" || key == "sourceBlendFactor") {
            enumList = &blendTypes;
        }
        else if(key == "type") {
            enumList = &layerTypes;
        }
        else if(key == "noiseType" || key == "maskNoiseType") {
            enumList = &noiseTypes;
        }
        else if(key == "previewTextureSize") {
            enumList = &textureSizes;    
        }
#ifdef Q_WS_MAC
        else if(key == "texture") {
            enumList = &billboardTextures;
        }
#endif

        property->setAttribute(QLatin1String("enumNames"), *enumList);

        int valueIndex = 0;

        // find the selected value
        for(int i = 0; i < enumList->size(); i++) {
            if((*enumList)[i] == QString(value.c_str())) {
                valueIndex = i;
                break;
            }
        }
        
        property->setValue(valueIndex);
    }
    else if(propertyType == QVariant::Double) {
		property->setValue(Ogre::StringConverter::parseReal(value));
        property->setAttribute(QLatin1String("singleStep"), 0.01);
        property->setAttribute(QLatin1String("decimals"), 3);
    }
    else if(propertyType == QVariant::Color) {
       property->setValue(getColor(value));
    }
    else {
        // assume string
        property->setValue(value.c_str());
    }

    return property;
}

/** Current selected layer property has changed so update the status bar
with this property's tip if it has one.
@param item The selected item
*/
void QtSpacescapeMainWindow::currentItemChanged(QtBrowserItem *item)
{
    // a hack to prevent invalid item checking when moving layers around
    if(mRefreshing) return;

    if(item && item->property() && !item->property()->statusTip().isNull()) {
        ui->statusBar->showMessage(item->property()->statusTip());
    }
}


/** Utility function to convert a blend mode string to int
@param param blend mode string like "one" or "dest_colour"
*/
int QtSpacescapeMainWindow::getBlendMode(const QString& param)
{
    if (param == "one")
        return 0;
    else if (param == "zero")
        return 1;
    else if (param == "dest_colour")
        return 2;
    else if (param == "src_colour")
        return 3;
    else if (param == "one_minus_dest_colour")
        return 4;
    else if (param == "one_minus_src_colour")
        return 5;
    else if (param == "dest_alpha")
        return 6;
    else if (param == "src_alpha")
        return 7;
    else if (param == "one_minus_dest_alpha")
        return 8;
    else if (param == "one_minus_src_alpha")
        return 9;
    else
        return 0;
}

/** Utility function to convert an ogre color to a string
@param color String color with values between 0 and 1
*/
QColor QtSpacescapeMainWindow::getColor(const Ogre::String& color)
{
    // first convert to a colourvalue
    Ogre::ColourValue c = Ogre::StringConverter::parseColourValue(color);
    return QColor(
        (int)floor(c.r * 255.0),
        (int)floor(c.g * 255.0),
        (int)floor(c.b * 255.0),
        (int)floor(c.a * 255.0)
    );
}

/** Utility function to convert an ogre color to a string
@param color String color with values between 0 and 255
*/
#include <string>
Ogre::ColourValue QtSpacescapeMainWindow::getColor(const QString& color)
{
	Ogre::StringVector vec = Ogre::StringUtil::split(color.toStdString());
    double scale = 1.0 / 255.0;

    return Ogre::ColourValue(
        floor(Ogre::StringConverter::parseReal(vec[0]) * scale),
        floor(Ogre::StringConverter::parseReal(vec[1]) * scale),
        floor(Ogre::StringConverter::parseReal(vec[2]) * scale),
        floor(Ogre::StringConverter::parseReal(vec[3]) * scale)
    );
}

/** Utility function to convert an QColor color to a Ogre string
@param color String color with values between 0 and 1
*/
Ogre::String QtSpacescapeMainWindow::getColor(QColor color)
{
    float scale = (float)1.0 / (float)255.0;
    return Ogre::StringConverter::toString(color.red() * scale) + " " +
        Ogre::StringConverter::toString(color.green() * scale) + " " +
        Ogre::StringConverter::toString(color.blue() * scale) + " " +
        Ogre::StringConverter::toString(color.alpha() * scale);
}

/** Utility function to get the status tip for a property type
@param prop - the property to get the status tip for
*/
QLatin1String QtSpacescapeMainWindow::getPropertyStatusTip(const Ogre::String& prop)
{
    if(prop == "name") {
        return QLatin1String("Layer name. What? Spacescapelayer0 isn't descriptive enough for you?!");
    }
    else if(prop == "destBlendFactor") {
        return QLatin1String("Destination blend factor.");
    }
    else if(prop == "ditherAmount") {
        return QLatin1String("Add additional fine grain noise to help with banding in the noise gradients - most noticable in dark colors.");
    }
    else if(prop == "gain" || prop == "maskGain") {
        return QLatin1String("A multiplier that determines how quickly the amplitudes diminish for each successive octave.");
    }
    else if(prop == "lacunarity" || prop == "maskLacunarity") {
        return QLatin1String("A multiplier that determines how quickly the frequency increases for each successive octave.");
    }
    else if(prop == "hdrPower") {
        return QLatin1String("How distance affects the transition between near and far colours.");
    }
    else if(prop == "hdrMultiplier") {
        return QLatin1String("Multiply the final by this value.");
    }
    else if(prop == "dataFile") {
        return QLatin1String("A CSV file with x,y,z,distance (in parsecs), mag (magnitude), BV fields.");
    }
    else if(prop == "octaves" || prop == "maskOctaves") {
        return QLatin1String("Number of noise functions in a series of noise functions that are added together.");
    }
    else if(prop == "noiseType" || prop == "maskNoiseType") {
        return QLatin1String("Noise type can be either FBM noise (smooth) or Ridged FBM noise.");
    }
    else if(prop == "offset" || prop == "maskOffset") {
        return QLatin1String("I honestly don't know exactly how this effects ridged noise, but it is only used for ridged noise. Have fun!");
    }
    else if(prop == "scale" || prop == "maskScale") {
        return QLatin1String("Multiplier applied to initial noise coordinates.");
    }
    else if(prop == "power" || prop == "maskPower") {
        return QLatin1String("The final noise value is raised to this power.  Useful for changing the noise gradient slope.");
    }
    else if(prop == "seed") {
        return QLatin1String("This number is used as the basis for the random number generator.");
    }
    else if(prop == "maskEnabled") {
        return QLatin1String("Makes points or billboards appear in greater density where noise values are higher.");
    }
    else if(prop == "maskSeed") {
        return QLatin1String("This number is used as the basis for the random number generator for the noise mask.");
    }
    else if(prop == "sourceBlendFactor") {
        return QLatin1String("Source blend factor.");
    }
    else if(prop == "type") {
        return QLatin1String("Layer type: Point, Billboards or Noise.");
    }
    else if(prop == "visible") {
        return QLatin1String("Show or hide this layer.");
    }
    else {
		// made this a space because if you just return "" then the value
		// does not get updated because the default value is ""
        return QLatin1String(" ");
    }
}

/** Utility function to convert between string types and property names and titles
For example you provied destBlendFactor and get Dest Blend Factor
@param prop - the property to get the title for
*/
QString QtSpacescapeMainWindow::getPropertyTitle(const Ogre::String& prop)
{
    if(mPropertyTitles[prop].isEmpty()) {
        return QString(prop.c_str());
    }
    else {
        return mPropertyTitles[prop];
    }
}

/** Utility function to convert between string types and property names and titles
For example you provide Dest Blend Factor and get destBlendFactor
@param prop - the property to get the xml var for for
*/
Ogre::String QtSpacescapeMainWindow::getProperty(const QString& prop)
{
    std::map<Ogre::String,QString>::iterator ii;
    for(ii = mPropertyTitles.begin(); ii != mPropertyTitles.end(); ii++) {
        if(ii->second == prop) {
            return ii->first;
        }
    }

    return Ogre::String(QString(prop).toStdString());
}

/** Utility function to get the property type by name
@param name The name of the property i.e. seed or maskEnabled etc.
@return the QVariant type
*/
int QtSpacescapeMainWindow::getPropertyType(const Ogre::String& name)
{
    if(name == "seed" || 
        name == "octaves" ||
        name == "maskSeed" ||
        name == "maskOctaves" ||
        name == "numBillboards" ||
        name == "numPoints" ||
        name == "pointSize" ) {
            return QVariant::Int;
    }
    else if(name == "visible" ||
        name == "maskEnabled"
        ) {
            return QVariant::Bool;
    }
    else if(name == "type" ||
        name == "destBlendFactor" ||
        name == "sourceBlendFactor" ||
        name == "noiseType" ||
        name == "previewTextureSize" ||
#ifdef Q_WS_MAC
        name == "texture" ||
#endif
        name == "maskNoiseType") {
            return QtVariantPropertyManager::enumTypeId();
    }
    else if(name == "name" ||
            name == "texture") {
        return QVariant::String;
    }
    else if(name == "dataFile") {
        return QVariant::String;
    }
    else if(name == "innerColor" ||
        name == "outerColor" ||
        name == "farColor" ||
        name == "nearColor") {
            return QVariant::Color;
    }
    else {
        // assume double
        return QVariant::Double;
    }
}


/** Utiltiy function for retrieving the selected layer id
@return int The id of the selected layer or -1
*/
int QtSpacescapeMainWindow::getSelectedLayerId()
{
    // get the current selected item
    QtBrowserItem* bi = ui->layerProperties->currentItem();
    if(!bi) {
        return -1;
    }

    // get the top level layer this item belongs to
    if(bi->parent()) {
        while(bi->parent()) {
            bi = bi->parent();
        }
    }

    int index = ui->layerProperties->topLevelItems().indexOf(bi);
    if(index > -1) {
        return ui->layerProperties->topLevelItems().size() - 1 - index;
    }

    return -1;
}

/** Utility function for creating and inserting layer properties
@param layer Spacescape layer
@param insertAfter Property to insert after
@param minimize Minimize this layer
@return The created / inserted property
*/
QtProperty* QtSpacescapeMainWindow::insertLayerProperties(Ogre::SpacescapeLayer* layer, QtProperty *insertAfter, bool minimize)
{
    // turn refreshing flag on so we don't process valueChanged events
    mRefreshing = true;

    // get layer params
    Ogre::NameValuePairList params = layer->getParams();

    // create the layer properties object
    QtProperty *layerProperties = mPropertyManager->addProperty(
                QtVariantPropertyManager::groupTypeId(),
                QLatin1String(layer->getName().c_str())
    );

    // insert it into the property tree early so we can minize items inside
    ui->layerProperties->insertProperty(layerProperties, insertAfter);

    // minimize the layer - speeds things up!
    if(minimize) {
        ui->layerProperties->setExpanded(ui->layerProperties->topLevelItem(layerProperties), false);
    }

    // add the common layer params to the subproperties first
    layerProperties->addSubProperty(createProperty( "name", layer->getName()));
    layerProperties->addSubProperty(createProperty( "type", layer->getLayerTypeName()));
    layerProperties->addSubProperty(createProperty( "visible", "true"));
    layerProperties->addSubProperty(createProperty( "seed", params["seed"]));

    // now add all the remaining layer params to the subproperties
    Ogre::NameValuePairList::iterator pl;
    for(pl = params.begin(); pl != params.end(); pl++) {
        // skip common params that come first
        if(pl->first == "name" || pl->first == "type" || 
            pl->first == "visible" || pl->first == "seed") {
            continue;
        }
        
        // skip hdr stuff if not enabled
        if(pl->first == "hdrMultiplier" || pl->first == "hdrPower") {
            if(!ui->ogreWindow->isHDREnabled())
                continue;
        }

        if(pl->first == "dataFile") {
            QtFilePathManager *mgr = new QtFilePathManager;
            QtProperty *pathProperty = mgr->addProperty("Data File");
            mgr->setValue(pathProperty, "");
            mgr->setFilter(pathProperty, "Source files (*.csv)");
            QtFileEditFactory *fact = new QtFileEditFactory;
            ui->layerProperties->setFactoryForManager(mgr, fact);
            layerProperties->addSubProperty(pathProperty);
            
            // add a signal for when properties are changed
            connect(mgr, SIGNAL(valueChanged(QtProperty *, const QString &)),
                    this, SLOT(valueChanged(QtProperty *, const QString &)));
        }
        else {
            // create the sub property
            QtVariantProperty* subProperty = createProperty(pl->first, pl->second);
            if(!subProperty) {
                continue;
            }

            // add this sub property parameter
            layerProperties->addSubProperty(subProperty);

            // special auto hide for color types
            if(getPropertyType(pl->first) == QVariant::Color) {
                QList<QtBrowserItem *> bi = ui->layerProperties->items(subProperty);
                ui->layerProperties->setExpanded(bi.first(),false);
            }
        }
    }

    // done adding properties
    mRefreshing = false;

    return layerProperties;
}

/** The about action was clicked
*/
void QtSpacescapeMainWindow::onAbout()
{
    QtSpacescapeAboutDialog d;
    d.exec();
}

/** The copy layer button was clicked
*/
void QtSpacescapeMainWindow::onCopyLayerClicked()
{
    // copy this layer
    int layerId = getSelectedLayerId();
    if(layerId != -1) {
        int newLayerId = ui->ogreWindow->copyLayer(layerId);

        // insert the new layer
        QList<QtBrowserItem *> bl = ui->layerProperties->topLevelItems();
        if((bl.size() - layerId - 1) == 0) {
            // insert at beginning of properties list
            insertLayerProperties(ui->ogreWindow->getLayers()[newLayerId]);
        }
        else {
            // insert before the copied property
            insertLayerProperties(ui->ogreWindow->getLayers()[newLayerId],
                                  ui->layerProperties->properties()[bl.size() - layerId - 2]);
        }
    }
}

/** The delete layer button was clicked
*/
void QtSpacescapeMainWindow::onDeleteLayerClicked()
{   
    // get the selected layer
    int layerId = getSelectedLayerId();
    if(layerId != -1) {
        ui->ogreWindow->deleteLayer(layerId);
        QList<QtBrowserItem *> bl = ui->layerProperties->topLevelItems();
        ui->layerProperties->removeProperty(bl[bl.size() - layerId - 1]->property());
    }
}

/** The enable hdr action was clicked
 */
void QtSpacescapeMainWindow::onEnableHDR()
{
    static bool hdrEnabled = false;
    hdrEnabled = !hdrEnabled;

    ui->ogreWindow->setHDREnabled(hdrEnabled);
    if(hdrEnabled) {
        ui->actionEnableHDR->setText(QApplication::translate("MainWindow", "Disable HDR", 0));
    }
    else {
        ui->actionEnableHDR->setText(QApplication::translate("MainWindow", "Enable HDR", 0));
    }
    
    refreshProperties();
}


/** The export action was clicked
*/
void QtSpacescapeMainWindow::onExport()
{
    QString imageSize;
    QString selectedFilter;
    QString outputTypes;
    QSettings settings;
    QString orientation;
    
    if(ui->ogreWindow->isHDREnabled()) {
        outputTypes = QLatin1String("6 EXR files(*.exr);;Single DDS Cube Map(*.dds)");
    }
    else {
        outputTypes = QLatin1String("6 PNG files(*.png);;6 JPG files(*.jpg);;6 TGA files(*.tga);;Single DDS Cube Map(*.dds)");
    }
    
    if(!settings.value("LastExportDir").isNull()) {
        mLastExportDir = settings.value("LastExportDir").toString();
    }
    if(!settings.value("selectedFilter").isNull()) {
        selectedFilter = settings.value("selectedFilter").toString();
    }
    if(!settings.value("imageSize").isNull()) {
        imageSize = settings.value("imageSize").toString();
    }
    if(!settings.value("orientation").isNull()) {
        orientation = settings.value("orientation").toString();
    }
    
	QString filename = QtSpacescapeExportFileDialog::getExportFileName(
		this,
		"Export Skybox",
		mLastExportDir,
		outputTypes,
        &selectedFilter,
        0,
        &imageSize,
        &orientation
    );

    settings.setValue("selectedFilter", selectedFilter);
    settings.setValue("imageSize", imageSize);
    settings.setValue("orientation", orientation);
    
    // disable ogre window till done exporting to prevent crashes
    ui->ogreWindow->setDisabled(true);

    if(!filename.isNull() && !filename.isEmpty()) {
        // make sure we have an extension on the filename
        QFileInfo fi(filename);
        if(fi.completeSuffix().isNull() || fi.completeSuffix().isEmpty()) {
            if(selectedFilter == "6 EXR files(*.exr)") {
                filename += ".exr";
            }
			else if (selectedFilter == "Single DDS Cube Map(*.dds)") {
				filename += ".dds";
            }
			else if (selectedFilter == "6 JPG files(*.jpg)") {
                filename += ".jpg";
            }
            else if (selectedFilter == "6 TGA files(*.tga)") {
                filename += ".tga";
            }
            else {
                filename += ".png";
            }
        }
        
        mLastExportDir = fi.absolutePath();

		// save the last export dir in the registry
		settings.setValue("LastExportDir",mLastExportDir);

        ui->statusBar->showMessage("Exporting skybox " + filename);

        bool cubeMap = selectedFilter == "Single DDS Cube Map(*.dds)" ;
        
        int skyboxOrientation = 0;
        if(orientation == "UNREAL") {
            skyboxOrientation = 1;
        }
        else if(orientation == "UNITY") {
            skyboxOrientation = 2;
        }
        else if(orientation == "SOURCE") {
            skyboxOrientation = 3;
        }
        
        // ogre can't export dds files doh!
		ui->ogreWindow->exportSkybox(filename,
                                     imageSize.toUInt(),
                                     cubeMap,
                                     skyboxOrientation);

        ui->statusBar->showMessage("Exported skybox " + filename,3000);
    }

    ui->ogreWindow->setDisabled(false);

}

/** The move down button was clicked
*/
void QtSpacescapeMainWindow::onMoveLayerDown()
{
    // get the selected layer
    int layerId = getSelectedLayerId();
    if(layerId > -1) {
        if(ui->ogreWindow->moveLayerDown(layerId)) {
            QList<QtBrowserItem *> bl = ui->layerProperties->topLevelItems();
            unsigned int index = bl.size() - layerId - 1;
            QtProperty *p = bl[index]->property();

            // save expanded settings
            bool expanded = ui->layerProperties->isExpanded(bl[index]);

            // remove and re-insert at the new location
            ui->layerProperties->removeProperty(p);
            ui->layerProperties->insertProperty(p,bl[index + 1]->property());

            // re-apply property tree visiblity settings
            bl = ui->layerProperties->topLevelItems();
            ui->layerProperties->setExpanded(bl[index + 1],expanded);

            // un-expand the color items
            QList<QtProperty *> sl = p->subProperties();
            for(int i = 0; i < sl.size(); i++) {
                if(((QtVariantProperty*)sl[i])->propertyType() == QVariant::Color) {
                    QList<QtBrowserItem *> bi = ui->layerProperties->items(sl[i]);
                    ui->layerProperties->setExpanded(bi.first(),false);
                }
            }

            // re-select the item
            ui->layerProperties->setFocus();
            bl = ui->layerProperties->topLevelItems();
            ui->layerProperties->setCurrentItem(bl[index + 1]);
        }
    }
}

/** The move up button was clicked
*/
void QtSpacescapeMainWindow::onMoveLayerUp()
{
    // get the selected layer
    int layerId = getSelectedLayerId();
    if(layerId > -1) {
        if(ui->ogreWindow->moveLayerUp(layerId)) {
            QList<QtBrowserItem *> bl = ui->layerProperties->topLevelItems();
            unsigned int index = bl.size() - layerId - 1;
            QtProperty *p = bl[index]->property();

            // save expanded settings
            bool expanded = ui->layerProperties->isExpanded(bl[index]);

            // remove and re-insert at the new location
            ui->layerProperties->removeProperty(p);
            if(index == 1) {
                ui->layerProperties->insertProperty(p, NULL);
            }
            else {
                ui->layerProperties->insertProperty(p,bl[index - 2]->property());
            }

            // re-apply property tree visiblity settings
            bl = ui->layerProperties->topLevelItems();
            ui->layerProperties->setExpanded(bl[index - 1],expanded);

            // un-expand the color items
            QList<QtProperty *> sl = p->subProperties();
            for(int i = 0; i < sl.size(); i++) {
                if(((QtVariantProperty*)sl[i])->propertyType() == QVariant::Color) {
                    QList<QtBrowserItem *> bi = ui->layerProperties->items(sl[i]);
                    ui->layerProperties->setExpanded(bi.first(),false);
                }
            }

            // re-select the item
            ui->layerProperties->setFocus();
            ui->layerProperties->setCurrentItem(bl[index - 1]);
        }
    }
}

/** The new file action was clicked
*/
void QtSpacescapeMainWindow::onNewFile()
{
    // TODO: prompt to save first
    ui->ogreWindow->clearLayers();
    refreshProperties();
}

/** The new layer button was clicked
*/
void QtSpacescapeMainWindow::onNewLayerClicked()
{
    ui->statusBar->showMessage("Creating new layer...");

    Ogre::NameValuePairList params;
    ui->ogreWindow->addLayer(0,params);

    ui->statusBar->showMessage("Creating new layer properties list...");

    // insert the new layer
    insertLayerProperties(ui->ogreWindow->getLayers().back());

    ui->statusBar->showMessage("New layer created",3000);
}

/* The open action was clicked
*/
void QtSpacescapeMainWindow::onOpen()
{
    QString filename = QFileDialog::getOpenFileName(
         this,
         QLatin1String("Open Spacescape .xml file"), 
         mLastOpenDir,
         QLatin1String("XML Files (*.xml)")
    );

    // disable ogre window till done opening to prevent crashes
    ui->ogreWindow->setDisabled(true);

    if(!filename.isNull() && !filename.isEmpty()) {
        QFileInfo fi(filename);
        mLastOpenDir = fi.absolutePath();
        mLastSaveDir = mLastOpenDir;

		// save in the registry
		QSettings settings;
		settings.setValue("LastOpenDir",mLastOpenDir);
		settings.setValue("LastSaveDir",mLastSaveDir);

        ui->statusBar->showMessage("Opening " + filename + " ...");

        // open the progress dialog
        ui->mProgressDialog->setValue(0);
        ui->mProgressDialog->show();

        if(ui->ogreWindow->open(filename)) {
            mFilename = filename;

            QString title = "Spacescape - " + fi.completeBaseName() + "." + fi.completeSuffix();
            setWindowTitle(QApplication::translate("MainWindow", title.toStdString().c_str(), 0));

            // file opened successfully so clear property list and start over
            refreshProperties();

            ui->statusBar->showMessage("Opened " + filename,3000);
        }
        else {
            ui->statusBar->showMessage("Failed to load " + filename,3000);
        }
    }

    ui->ogreWindow->setDisabled(false);
}

/** The save action was clicked
*/
void QtSpacescapeMainWindow::onSave()
{
    if(mFilename.isEmpty() || mFilename.isNull()) {
        mFilename = QFileDialog::getSaveFileName(
            this,
            "Save As...",
            mLastSaveDir,
            QLatin1String("XML Files(*.xml)")
        );
    }

    if(!mFilename.isNull()) {
        if(mFilename.isEmpty()) {
            // pop up an error - no empty filenames allowed
        }
        else {
            QFileInfo fi(mFilename);
            mLastSaveDir = fi.absolutePath();

			// save the last save dir in the registry
			QSettings settings;
			settings.setValue("LastSaveDir",mLastSaveDir);

            ui->statusBar->showMessage("Saving " + mFilename);

            // save this file!
            if(ui->ogreWindow->save(mFilename)) {
                QString title = "Spacescape - " + fi.completeBaseName() + "." + fi.completeSuffix();
                setWindowTitle(QApplication::translate("MainWindow", title.toStdString().c_str(), 0));

                ui->statusBar->showMessage("Saved " + mFilename,3000);
            }
            else {
                ui->statusBar->showMessage("Failed to save " + mFilename,3000);
            }
        }
    }
}

/** The save as action was clicked
*/
void QtSpacescapeMainWindow::onSaveAs()
{
    mFilename = QFileDialog::getSaveFileName(
        this,
        "Save As...",
        mLastSaveDir,
        QLatin1String("XML Files(*.xml)")
    );

    if(!mFilename.isNull()) {
        if(mFilename.isEmpty()) {
            // pop up an error - no empty filenames allowed
        }
        else {
            QFileInfo fi(mFilename);
            mLastSaveDir = fi.absolutePath();

			// save the last save dir in the registry
			QSettings settings;
			settings.setValue("LastSaveDir",mLastSaveDir);

            // save this file!
            if(ui->ogreWindow->save(mFilename)) {
                QString title = "Spacescape - " + fi.completeBaseName() + "." + fi.completeSuffix();
                setWindowTitle(QApplication::translate("MainWindow", title.toStdString().c_str(), 0));
            }
        }
    }
}
    
/** The show debug box was clicked
 */
void QtSpacescapeMainWindow::onShowDebugBox()
{
    static bool debugVisible = false;
    debugVisible = !debugVisible;
    ui->ogreWindow->setDebugBoxVisible(debugVisible);
    if(debugVisible) {
        ui->actionShowDebugBox->setText(QApplication::translate("MainWindow", "Hide Debug Box", 0));
    }
    else {
        ui->actionShowDebugBox->setText(QApplication::translate("MainWindow", "Show Debug Box", 0));
    }
}

    
/** Utility function for refreshing the entire layer property tree
*/
void QtSpacescapeMainWindow::refreshProperties()
{
    // set refreshing properties flag so we don't process valueChanged events
    mRefreshing = true;

    // remove existing properties
    mPropertyManager->clear();

    // get all layers and add them to the property list in reverse order
    std::vector<Ogre::SpacescapeLayer *> layers = ui->ogreWindow->getLayers();
    if(!layers.empty()) {
        QtProperty *layer = NULL;

        QString numLayers;
        numLayers.setNum(layers.size());
        for(int i = layers.size() - 1; i >= 0; i--) {
            QString layerNum;
            layerNum.setNum(layers.size() - i);
            //ui->statusBar->showMessage("Updating layer properties " + layerNum + " of " + numLayers);
            layer = insertLayerProperties(layers[i], layer);
        }
    }

    mRefreshing = false;
}

/** Function used to update the progress bar
@param percentComplete The percentage complete 0 - 100
@param msg The current task message
*/
void QtSpacescapeMainWindow::updateProgressBar(unsigned int percentComplete, const Ogre::String& msg)
{
    ui->mProgressDialog->setValue(percentComplete);
    ui->mProgressDialog->setLabelText(QString(msg.c_str()));
    qApp->processEvents();
}

/** A layer property was changed in the UI. Update the layer and 
make any necessary UI changes.
@param property The changed property name
@param value The new value.
*/
void QtSpacescapeMainWindow::valueChanged(QtProperty *property, const QVariant &value)
{
    // don't update if we're refreshing
    if(mRefreshing) return;

    QList<QtProperty *> l = ui->layerProperties->properties();

    int topLevelIndex = 0;
    int layerId = -1;

    // find the layer that contains this property
    for(; topLevelIndex < l.size(); topLevelIndex++) {
        if(l[topLevelIndex] == property) {
            layerId = l.size() - 1 - topLevelIndex;
            break;
        }

        bool found = false;

        QList<QtProperty *> sl = l[topLevelIndex]->subProperties();
        if(!sl.empty()) {
            for(int id = 0; id < sl.size(); id++) {
                if(sl[id] == property) {
                    layerId = l.size() - 1 - topLevelIndex;
                    found = true;
                    break;
                }
            }
        }

        if(found) break;
    }

    // did we find a valid layer id?
    std::vector<Ogre::SpacescapeLayer *> layers = ui->ogreWindow->getLayers();
    if(layerId > -1 && layerId < (int)l.size() && layerId < (int)layers.size()) {
        Ogre::NameValuePairList params;
        Ogre::String propertyStr = getProperty(property->propertyName());
        bool refresh = false;

        if(property->propertyName().indexOf("color") != -1 ||
            property->propertyName().indexOf("Color") != -1) {
            params[propertyStr] = getColor(value.value<QColor>());
        }
        else if(propertyStr == "noiseType" || propertyStr == "maskNoiseType") {
            params[propertyStr] = value == "0" ? "fbm" : "ridged";
        }
#ifdef Q_WS_MAC
        else if(propertyStr == "texture") {
            QStringList billboardTextures;
            if(ui->ogreWindow->isHDREnabled()) {
                billboardTextures << "hdr-flare-blue.exr" << "hdr-flare-red.exr" << "hdr-flare-yellow.exr" << "hdr-flare-purple.exr" << "hdr-flare-white.exr" << "hdr-flare-white2.exr";
                billboardTextures << "hdr-flare-blue-2spires.exr" << "hdr-flare-blue-4spires.exr" << "hdr-flare-blue1.exr" << "hdr-flare-bluepurple-4spire.exr" << "hdr-flare-bluepurple-multispire.exr" << "hdr-flare-pink1.exr" << "hdr-flare-red-2spires.exr" << "hdr-flare-redpurple-multispire.exr";
            }
            else {
                billboardTextures << "default.png" << "flare-blue-purple1.png" << "flare-blue-purple2.png" << "flare-blue-purple3.png" << "flare-blue-spikey1.png" << "flare-green1.png" << "flare-inverted-blue-purple3.png" << "flare-red-yellow1.png" << "flare-red1.png" << "flare-white-small1.png" << "sun.png";
            }
            params[propertyStr] = Ogre::String(billboardTextures[value.toUInt()].toStdString());
        }
#endif
        else if(propertyStr == "type") {
            QStringList layerTypes;
            layerTypes << "points" << "billboards" << "noise";
            params[propertyStr] = Ogre::String(layerTypes[value.toUInt()].toStdString());
            refresh = true;
        }
        else if(propertyStr == "previewTextureSize") {
            QStringList textureSizes;
            textureSizes << "64" << "128" << "256" << "512" << "1024" << "2048" << "4096" << "8192";
            params[propertyStr] = Ogre::String(textureSizes[value.toUInt()].toStdString());
        }
        else if(propertyStr == "name") {
            // set the layer name property
            ui->layerProperties->topLevelItems()[topLevelIndex]->property()->setPropertyName(value.toString());
            params[propertyStr] = Ogre::String(value.toString().toStdString());
        }
        else if(propertyStr == "destBlendFactor" || propertyStr == "sourceBlendFactor") {
            QStringList blendTypes;
            
            blendTypes << "one" << "zero" << "dest_colour" << "src_colour" 
            << "one_minus_dest_colour" << "one_minus_src_colour" 
            << "dest_alpha" << "src_alpha" << "one_minus_dest_alpha" 
            << "one_minus_src_alpha";

            params[propertyStr] = Ogre::String(blendTypes[value.toUInt()].toStdString());
        }
        else if(propertyStr == "visible") {
            ui->ogreWindow->setLayerVisible(layerId, value.toBool());
        }
        else {
            params[propertyStr] = Ogre::String(value.toString().toStdString());
        }

        // update the layer with new parameter settings
        if(!params.empty()) {
            ui->ogreWindow->updateLayer(layerId,params);
        }

        // refresh layer properties if necessary
        if(refresh) {
            // remove this layer's properties
            QList<QtBrowserItem *> bl = ui->layerProperties->topLevelItems();
            ui->layerProperties->removeProperty(bl[bl.size() - layerId - 1]->property());
            
            bl = ui->layerProperties->topLevelItems();

            // re-insert new properties
            if(!bl.empty() && layerId != bl.size()) {
                insertLayerProperties(ui->ogreWindow->getLayers()[layerId],bl[bl.size() - layerId - 1]->property(),false);
            }
            else {
                insertLayerProperties(ui->ogreWindow->getLayers()[layerId],0,false);
            }
        }
    }
}
    
void QtSpacescapeMainWindow::valueChanged(QtProperty *property, const QString &value)
{
    // don't update if we're refreshing
    if(mRefreshing) return;
    
    QList<QtProperty *> l = ui->layerProperties->properties();
    
    int topLevelIndex = 0;
    int layerId = -1;
    
    // find the layer that contains this property
    for(; topLevelIndex < l.size(); topLevelIndex++) {
        if(l[topLevelIndex] == property) {
            layerId = l.size() - 1 - topLevelIndex;
            break;
        }
        
        bool found = false;
        
        QList<QtProperty *> sl = l[topLevelIndex]->subProperties();
        if(!sl.empty()) {
            for(int id = 0; id < sl.size(); id++) {
                if(sl[id] == property) {
                    layerId = l.size() - 1 - topLevelIndex;
                    found = true;
                    break;
                }
            }
        }
        
        if(found) break;
    }
    
    // did we find a valid layer id?
    std::vector<Ogre::SpacescapeLayer *> layers = ui->ogreWindow->getLayers();
    if(layerId > -1 && layerId < (int)l.size() && layerId < (int)layers.size()) {
        Ogre::NameValuePairList params;
        Ogre::String propertyStr = getProperty(property->propertyName());
        params[propertyStr] = Ogre::String(value.toStdString());
        ui->ogreWindow->updateLayer(layerId,params);
    }
}

