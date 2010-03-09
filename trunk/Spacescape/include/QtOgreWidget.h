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
#ifndef QtOgreWidget_H
#define QtOgreWidget_H

#include <QWidget>
#if defined(Q_WS_WIN)
// disable some warning
#pragma warning( push ) 
#pragma warning( disable : 4100 )
#include <Ogre.h>
#pragma warning( pop )
#else
#include <OGRE/Ogre.h>
#endif

#if defined(Q_WS_MAC)
// mac specific context
typedef struct __AGLContextRec  *AGLContext;
#endif

/** QtOgreWidget is a class for embedding an Ogre
render window in a QtWidget. Thank you QtOgre!!
*/
class QtOgreWidget : public QWidget {
	Q_OBJECT

public:
    /** Constructor
    @param parent
    */
	QtOgreWidget(QWidget* parent);

    /** Destructor
    */
	~QtOgreWidget(void);

    /** Update the Ogre render window
    */
    virtual void update(void);

    /** Give the minimum size for this widget
    @return the minimum size for this widget
    */
	QSize minimumSizeHint(void) const;

protected:
    /** Configure Ogre
    */
	virtual void configure(void);

    /** Create the Ogre render window
    */
	void createRenderWindow(void);

    /** Create Ogre scene - pure virtual
    */
    virtual void createScene(void) = 0;

    /** Handle a paint event (just render again, if needed create render window)
    @param e The event data
    */
    void paintEvent(QPaintEvent *e);

    /** Handle a resize event (pass it along to the render window)
    @param e The event data
    */
	void resizeEvent(QResizeEvent *e);

    /** Resize the render window (when the widget was resized)
    */
    void resizeRenderWindow(void);

    /** Set up Ogre resources - pure virtual
    */
	virtual void setupResources(void) = 0;

    // Ogre renderwindow
	Ogre::RenderWindow *mRenderWindow;

    // Ogre root
	static Ogre::Root *mOgreRoot;

#if defined(Q_WS_MAC)
    // mac specific context
	AGLContext mAglContext;
#endif
};

#endif