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
#ifndef __SPACESCAPEPROGRESSLISTENER_H__
#define __SPACESCAPEPROGRESSLISTENER_H__

namespace Ogre
{
    /** This listener class is used to send progress updates regarding
    spacescape plugin tasks like opening a file or exporting or anything
    that might take a while.
    */
	class _SpacescapePluginExport SpacescapeProgressListener
	{
    public:
        /** Destructor
        */
		virtual ~SpacescapeProgressListener() {}

        /** pure virtual function used to update the progress bar
        @param percentComplete The percentage complete 0 - 100
        @param msg The current task message
        */
        virtual void updateProgressBar(unsigned int percentComplete, const String& msg) = 0;
    };
}

#endif