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
#ifndef QtSpacescapeWidget_H
#define QtSpacescapeWidget_H

#include "QtOgreWidget.h"
#include "SpacescapePlugin.h"
#include "SpacescapeLayer.h"

/** QtSpacescapeWidget is a subclass of QtOgreWidget and it interacts
with the Spacescape Ogre Plugin and Ogre.
*/
class QtSpacescapeWidget : public QtOgreWidget {
	Q_OBJECT
public:
    /** Constructor
    */
	QtSpacescapeWidget(QWidget* parent);

    /** Destructor
    */
	~QtSpacescapeWidget(void);
    
    /** Add a SpacescapeLayer
    @param type Layer type (0 - points, 1 billboards, 2 noise)
    @param params Layer params
    @return layerId or -1 on failure
    */
    int addLayer(int type, const Ogre::NameValuePairList& params);

    /** Clear all SpacescapeLayers
    */
    void clearLayers();

    /** Copy a SpacescapeLayer
    @param layerID Layer ID of the layer to copy
    @return layerId of the new layer or -1 on failure
    */
    int copyLayer(unsigned int layerID);

    /** Delete a SpacescapeLayer
    @param layerID Layer ID of the layer to delete
    @return true on success
    */
    bool deleteLayer(unsigned int layerID);

    /** Write the current spacescape to a skybox
    @param filename Name of the file (with path)
    @param imageSize The size of the image in pixels
    @param cubeMap Whether to write a single cubemap or not
    @return true on success
    */
    bool exportSkybox(const QString& filename, unsigned int imageSize = 1024, bool cubeMap = false);

    /** Get current SpacescapeLayers list
    @return current SpacescapeLayers list
    */
    std::vector<Ogre::SpacescapeLayer *> getLayers();

    /** Move a SpacescapeLayer down in the list
    @param layerID The layer ID of the layer to move
    @return true on success
    */
    bool moveLayerDown(unsigned int layerID);

    /** Move a SpacescapeLayer up in the list
    @param layerID The layer ID of the layer to move
    @return true on success
    */
    bool moveLayerUp(unsigned int layerID);

    /** Open a Spacescape .xml file
    @param filename The name of the file to open (with path)
    @return true on success
    */
    bool open(const QString& filename);

    /** Save a Spacescape .xml file
    @param filename The name of the file to save (with path)
    @return true on success
    */
    bool save(const QString& filename);

    /** Change the visibility of a SpacescapeLayer
    @param layerID The layer ID of the layer to show/hide
    @param visible Visibility flag
    */
    void setLayerVisible(unsigned int layerID, bool visible);

    /** Set the progress listener
    @param listener The new listener object
    */
    void setProgressListener(Ogre::SpacescapeProgressListener* listener) { mProgressListener = listener; }

    /** Update the params or a SpacescapeLayer
    @param layerID The layer ID of the layer to move
    @param params The new params
    @return true on success
    */
    bool updateLayer(unsigned int layerID, const Ogre::NameValuePairList& params);

protected:
    /** Create Ogre scene
    */
	void createScene(void);

    /** The user moved the mouse, if tracking process it
    @param e The event data
    */
	void mouseMoveEvent(QMouseEvent *e);

    /** The user pressed a mouse button, start tracking
    @param e The event data
    */
	void mousePressEvent(QMouseEvent *e);

    /** The user released a mouse button, stop tracking
    @param e The event data
    */
	void mouseReleaseEvent(QMouseEvent *e);

    /** Handle a paint event (just render again, if needed create render window)
    @param e The event data
    */
    void paintEvent(QPaintEvent *e);

    /** Handle a timer event - calls update()
    @param e The event data
    */
    void timerEvent(QTimerEvent *e);

    /** Handle a resize event (pass it along to the render window)
    @param e The event data
    */
	void resizeEvent(QResizeEvent *e);

    /** Set up Ogre resources
    */
    void setupResources(void);

    /** Setup the scene
    */
    void setupScene(void);

    // mouse pressed flag
	bool mMousePressed;

    // mouse pressed position
	QPoint mMousePressPos;

    // camera orientation when mouse was last pressed
	Ogre::Quaternion mLastCamOrientation;

    // Ogre camera node
	Ogre::SceneNode *mCameraNode;

    // Ogre scene manager
	Ogre::SceneManager *mSceneMgr;

    // Ogre camera
	Ogre::Camera *mCamera;

    // Ogre viewport
	Ogre::Viewport *mViewPort;

    // the radius used for rotating
	static const float mRADIUS;

private:
    /** Utility function for getting a pointer to 
    the Spacescape Ogre plugin
    @return the spacescape plugin or NULL
    */
    Ogre::SpacescapePlugin* getPlugin();

    // Our spacescape progress listener
    Ogre::SpacescapeProgressListener* mProgressListener;
};

#endif
