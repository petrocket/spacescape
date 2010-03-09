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
#ifndef __SPACESCAPEPREREQUISITES_H__
#define __SPACESCAPEPREREQUISITES_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
}

#if (OGRE_PLATFORM == OGRE_PLATFORM_WIN32 ) && !defined(OGRE_STATIC_LIB)
#   ifdef OGRE_SPACESCAPEPLUGIN_EXPORTS
#       define _SpacescapePluginExport __declspec(dllexport)
#   else
#       if defined( __MINGW32__ )
#           define _SpacescapePluginExport
#       else
#    		define _SpacescapePluginExport __declspec(dllimport)
#       endif
#   endif
#elif defined ( OGRE_GCC_VISIBILITY )
#    define _SpacescapePluginExport  __attribute__ ((visibility("default")))
#else
#   define _SpacescapePluginExport
#endif

#endif

