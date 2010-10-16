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
#include "QtSpacescapeWidget.h"
#include <QMouseEvent>

const float QtSpacescapeWidget::mRADIUS = (float)0.8;

/** Constructor
*/
QtSpacescapeWidget::QtSpacescapeWidget(QWidget *parent) : QtOgreWidget(parent),
    mProgressListener(0) {
	mSceneMgr = NULL;
	mViewPort = NULL;
	mMousePressed = false;
}

/** Destructor
*/
QtSpacescapeWidget::~QtSpacescapeWidget(void) {
}

/** Add a SpacescapeLayer
@param type Layer type (0 - points, 1 billboards, 2 noise)
@param params Layer params
@return layerId or -1 on failure
*/
int QtSpacescapeWidget::addLayer(int type, const Ogre::NameValuePairList& params)
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        return plugin->addLayer(type, params);
    }

    return -1;
}

/** Clear all SpacescapeLayers
*/
void QtSpacescapeWidget::clearLayers()
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        plugin->clear();
    }
}

/** Copy a SpacescapeLayer
@param layerID Layer ID of the layer to copy
@return layerId of the new layer or -1 on failure
*/
int QtSpacescapeWidget::copyLayer(unsigned int layerID)
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        return plugin->duplicateLayer(layerID);
    }

    return -1;
}

/** Create Ogre scene
*/
void QtSpacescapeWidget::createScene(void) {
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0,0,0,0));
}

/** Delete a SpacescapeLayer
@param layerID Layer ID of the layer to delete
@return true on success
*/
bool QtSpacescapeWidget::deleteLayer(unsigned int layerID)
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        return plugin->deleteLayer(layerID);
    }

    return false;
}

/** Write the current spacescape to a skybox
@param filename Name of the file (with path)
@param imageSize The size of the image in pixels
@param cubeMap Whether to write a single cubemap or not
@return true on success
*/
bool QtSpacescapeWidget::exportSkybox(const QString& filename, unsigned int imageSize, bool cubeMap)
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        plugin->writeToFile(
            Ogre::String(filename.toStdString()),
            imageSize,
            cubeMap ? Ogre::TEX_TYPE_CUBE_MAP : Ogre::TEX_TYPE_2D
        );
        return true;
    }

    return false;
}

/** Get current SpacescapeLayers list
@return current SpacescapeLayers list
*/
std::vector<Ogre::SpacescapeLayer *> QtSpacescapeWidget::getLayers()
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        return plugin->getLayers();
    }

    std::vector<Ogre::SpacescapeLayer *> layers;

    return layers;
}

/** Utility function for getting a pointer to 
the Spacescape Ogre plugin
@return the spacescape plugin or NULL
*/
Ogre::SpacescapePlugin* QtSpacescapeWidget::getPlugin()
{
    //std::vector<Ogre::Plugin*> pl = Ogre::Root::getSingleton().getInstalledPlugins();
	Ogre::Root::PluginInstanceList pl = Ogre::Root::getSingleton().getInstalledPlugins();
	Ogre::Root::PluginInstanceList::iterator ii = pl.begin();
    for(ii = pl.begin(); ii != pl.end(); ii++) {
        if((*ii)->getName() == "Spacescape") {
            return (Ogre::SpacescapePlugin*)(*ii);
        }
    }

    return NULL;
}

/** Handle a paint event (just render again, if needed create render window)
@param e The event data
*/
void QtSpacescapeWidget::paintEvent(QPaintEvent *) {
	if(!mRenderWindow) {
		createRenderWindow();
		setupResources();
		setupScene();
	}

	update();
}

/** The user moved the mouse, if tracking process it
@param e The event data
*/
void QtSpacescapeWidget::mouseMoveEvent(QMouseEvent *e) {
    if (mMousePressed) {
        QPoint curPos = e->pos();
		
        double w = width();
        double h = height();
		
        double curX = (curPos.x() * 2. - w) / w;
        double curY = (h - curPos.y() * 2.) / h;
        double x0 = (mMousePressPos.x() * 2. - w) / w;
        double y0 = (h - mMousePressPos.y() * 2.) / h;
		
        Ogre::Vector3 v1(x0, y0, 0);
        Ogre::Vector3 v2(curX, curY, 0);
		
        double radiusSqr = mRADIUS * mRADIUS;
        double cutoff = radiusSqr * 0.5;
        double Rho = v1[0] * v1[0] + v1[1] * v1[1];
        v1[2] = (Rho < cutoff) ? sqrt(radiusSqr - Rho) : (cutoff / sqrt(Rho));
		
        Rho = v2[0] * v2[0] + v2[1] * v2[1];
        v2[2] = (Rho < cutoff) ? sqrt(radiusSqr - Rho) : (cutoff / sqrt(Rho));
		
        // v_cross is the normal of rotating plane
        Ogre::Vector3 cross = v2.crossProduct(v1);
        cross.normalise();

        // compute the angle
        v1.normalise();
        v2.normalise();
        double cosAngle = v1.dotProduct(v2);
        if (cosAngle < -1.0)
            cosAngle = -1.0;
        else if(cosAngle > 1.0)
            cosAngle = 1.0;
        double angle = acos(cosAngle);
		
        mCameraNode->rotate(cross, Ogre::Radian(angle));
		
        mMousePressPos = curPos;
        mLastCamOrientation = mCameraNode->getOrientation();

        update();
    }
}

/** The user pressed a mouse button, start tracking
@param e The event data
*/
void QtSpacescapeWidget::mousePressEvent(QMouseEvent *e) {
    mMousePressPos = e->pos();
	if (mCameraNode)
		mLastCamOrientation = mCameraNode->getOrientation();
    mMousePressed = true;
}

/** The user released a mouse button, stop tracking
@param e The event data
*/
void QtSpacescapeWidget::mouseReleaseEvent(QMouseEvent *) {
    mMousePressed = false;
}

/** Move a SpacescapeLayer down in the list
@param layerID The layer ID of the layer to move
@return true on success
*/
bool QtSpacescapeWidget::moveLayerDown(unsigned int layerID)
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        return plugin->moveLayer(layerID,-1);
    }

    return false;
}

/** Move a SpacescapeLayer up in the list
@param layerID The layer ID of the layer to move
@return true on success
*/
bool QtSpacescapeWidget::moveLayerUp(unsigned int layerID)
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        return plugin->moveLayer(layerID,1);
    }

    return false;
}

/** Open a Spacescape .xml file
@param filename The name of the file to open (with path)
@return true on success
*/
bool QtSpacescapeWidget::open(const QString& filename)
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        if(mProgressListener) {
            plugin->addProgressListener(mProgressListener);
        }
        return plugin->loadConfigFile(filename.toStdString());
    }

    return false;
}

/** Handle a resize event (pass it along to the render window)
@param e The event data
*/
void QtSpacescapeWidget::resizeEvent(QResizeEvent *e) {
	QtOgreWidget::resizeEvent(e);
	
	if (mRenderWindow) {
		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio(Ogre::Real(width()) / Ogre::Real(height()));
		mViewPort->update();
	}
}

/** Save a Spacescape .xml file
@param filename The name of the file to save (with path)
@return true on success
*/
bool QtSpacescapeWidget::save(const QString& filename)
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        return plugin->saveConfigFile(filename.toStdString());
    }

    return false;
}

/** Change the visibility of a SpacescapeLayer
@param layerID The layer ID of the layer to show/hide
@param visible Visibility flag
*/
void QtSpacescapeWidget::setLayerVisible(unsigned int layerID, bool visible)
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        plugin->setLayerVisible(layerID,visible);
    }
}

/** Set up Ogre resources
*/
void QtSpacescapeWidget::setupResources(void) {
	// Load resource paths from config file
	Ogre::ConfigFile config;
        config.load("../resources.cfg");
	
	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator it = config.getSectionIterator();
	
	Ogre::String secName, typeName, archName;
	while (it.hasMoreElements()) {
		secName = it.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = it.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		
		for (i = settings->begin(); i != settings->end(); ++i) {
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
    }
}

/** Setup the scene
*/
void QtSpacescapeWidget::setupScene(void) {
	mSceneMgr = mOgreRoot->createSceneManager(Ogre::ST_GENERIC);
	
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0, 0, 0));
    mCamera->setNearClipDistance(0.1f);
    mCamera->setFarClipDistance(10000.0f);
	
	// Create one viewport, entire window
	mViewPort = mRenderWindow->addViewport(mCamera);
	mViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mViewPort->setClearEveryFrame(true);
	
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	createScene();
	
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_BILINEAR);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(1);
	
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(mViewPort->getActualWidth()) / Ogre::Real(mViewPort->getActualHeight()));
    mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mCameraNode->attachObject(mCamera);

    startTimer(20);
}

/** Handle a timer event - calls update()
@param e The event data
*/
void QtSpacescapeWidget::timerEvent(QTimerEvent *) {
    update();
}

/** Update the params or a SpacescapeLayer
@param layerID The layer ID of the layer to move
@param params The new params
@return true on success
*/
bool QtSpacescapeWidget::updateLayer(unsigned int layerID, const Ogre::NameValuePairList& params)
{
    Ogre::SpacescapePlugin* plugin = getPlugin();
    if(plugin) {
        return plugin->updateLayer(layerID,params);
    }

    return false;
}
