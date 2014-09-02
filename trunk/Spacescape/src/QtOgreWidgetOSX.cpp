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
#include "QtOgreWidgetOSX.h"
#include <QApplication>
#include <QWidget>
#if defined(Q_WS_MAC)
#include "macUtils.h"
#elif defined(Q_WS_X11)
#include <QX11Info>
#endif

Ogre::Root * QtOgreWidget::mOgreRoot = NULL;

/** Constructor
 @param parent
 */
QtOgreWidget::QtOgreWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f | Qt::MSWindowsOwnDC) {
#ifndef Q_WS_MAC
	setAttribute(Qt::WA_PaintOnScreen);
#endif
	setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NativeWindow);
	
	mRenderWindow = NULL;
	mOgreRoot = NULL;
}

/** Destructor
 */
QtOgreWidget::~QtOgreWidget(void) {
}

/** Configure Ogre
 */
void QtOgreWidget::configure(void) {
	if (mOgreRoot)
		return;
	
#ifdef WIN32
	// set the current working directory to the path where the
	// executable resides
	QString path = QApplication::applicationDirPath();
    
	if(!path.contains("app/win",Qt::CaseInsensitive)) {
		path += "/app/win/release";
	}
    
	//MessageBox(NULL, path.toStdString().c_str(), "App Path", MB_OK);
    
	SetCurrentDirectory(path.toStdString().c_str());
#endif
    
#if defined(Q_WS_X11)
    mOgreRoot = new Ogre::Root("plugins.cfg", "app.cfg", "app.log");
#else
    #if defined(Q_WS_MAC)
    Ogre::String resourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
    mOgreRoot = new Ogre::Root(
                               resourcePath + "plugins.cfg",
                               resourcePath + "app.cfg",
                               "app.log"
                               );
    
    #else
        #ifdef _DEBUG
            mOgreRoot = new Ogre::Root(
                                       QString("../plugins_d.cfg").toStdString(), 
                                       QString("../app.cfg").toStdString(), 
                                       QString("../app.log").toStdString()
                                       );
        #else
            mOgreRoot = new Ogre::Root(
                                       QString("../plugins.cfg").toStdString(), 
                                       QString("../app.cfg").toStdString(), 
                                       QString("../app.log").toStdString()
                                       );
        #endif
    #endif
#endif
	if (!mOgreRoot->restoreConfig()) {
		// setup a renderer
		Ogre::RenderSystemList renderers = mOgreRoot->getAvailableRenderers();
		assert(!renderers.empty()); // we need at least one renderer to do anything useful
		
		//Ogre::RenderSystem *renderSystem = chooseRenderer(renderers);
		// TODO: does this work right?
		Ogre::RenderSystem *renderSystem = *renderers.begin();
		assert(renderSystem); // user might pass back a null renderer, which would be bad!
		
		mOgreRoot->setRenderSystem(renderSystem);
		QString dimensions = QString("%1x%2").arg(width()).arg(height());
		renderSystem->setConfigOption("Video Mode", dimensions.toStdString());
		
		// initialize without creating window
		mOgreRoot->getRenderSystem()->setConfigOption("Full Screen", "No");
		mOgreRoot->saveConfig();
	}
	mOgreRoot->initialise(false);

}

/** Create the Ogre render window
 */
void QtOgreWidget::createRenderWindow(void) {
	Ogre::NameValuePairList params;
	
#ifndef Q_WS_MAC
    setAttribute(Qt::WA_PaintOnScreen);
#endif
    setAttribute(Qt::WA_NoSystemBackground);
    
	if (mRenderWindow)
		return;
	if (!mOgreRoot)
		configure();
    
    params["useNVPerfHUD"] = "true";
    
    //Accept input focus
    setFocusPolicy(Qt::StrongFocus);    
	
#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
    WId wId = winId();
	params["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)(wId));
#else
    QX11Info info = x11Info();
	Ogre::String winHandle;
	winHandle  = Ogre::StringConverter::toString((unsigned long)(info.display()));
	winHandle += ":";
	winHandle += Ogre::StringConverter::toString((unsigned int)(info.screen()));
	winHandle += ":";
	winHandle += Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
	params["parentWindowHandle"] = winHandle;
    
#endif
#if defined(Q_WS_MAC)
    params["macAPI"] = "cocoa";
    params["macAPICocoaUseNSView"] = "true";
#endif
    mRenderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("OgreWindow", width(), height(), false, &params);
	
#if defined(Q_WS_MAC)
	resizeRenderWindow();
#endif
	
	// take over ogre window
#if !defined(Q_WS_MAC) && !defined(Q_WS_WIN)
	WId ogreWinId = 0x0;
	mRenderWindow->getCustomAttribute("WINDOW", &ogreWinId);
	assert(ogreWinId);
	create(ogreWinId);
#endif
}

/** Give the minimum size for this widget
 @return the minimum size for this widget
 */
QSize QtOgreWidget::minimumSizeHint(void) const {
	return QSize(50, 50);
}

/** Handle a paint event (just render again, if needed create render window)
 @param e The event data
 */
void QtOgreWidget::paintEvent(QPaintEvent *) {
	if (!mRenderWindow)
		createRenderWindow();
	
	update();
}

/** Handle a resize event (pass it along to the render window)
 @param e The event data
 */
void QtOgreWidget::resizeEvent(QResizeEvent *e) {
	if (mRenderWindow)
		resizeRenderWindow();
}

/** Resize the render window (when the widget was resized)
 */
void QtOgreWidget::resizeRenderWindow(void) {
	if (!mRenderWindow)
		return;
    
    if(mRenderWindow)
    {
        mRenderWindow->resize(width(), height());
        mRenderWindow->windowMovedOrResized();
        
        for(int ct = 0; ct < mRenderWindow->getNumViewports(); ++ct)
        {
            Ogre::Viewport* pViewport = mRenderWindow->getViewport(ct);
            Ogre::Camera* pCamera = pViewport->getCamera();
            pCamera->setAspectRatio(static_cast<Ogre::Real>(pViewport->getActualWidth()) / static_cast<Ogre::Real>(pViewport->getActualHeight()));
        }
    }
}

/** Update the Ogre render window
 */
void QtOgreWidget::update(void) {
	if (mRenderWindow && this->isEnabled()) {        
        mOgreRoot->_fireFrameStarted();
        mRenderWindow->update();
        mOgreRoot->_fireFrameRenderingQueued();
        mOgreRoot->_fireFrameEnded();        
	}
}
