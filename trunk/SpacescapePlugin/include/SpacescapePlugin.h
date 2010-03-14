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
#ifndef __SPACESCAPEPLUGIN_H__
#define __SPACESCAPEPLUGIN_H__

#include "SpacescapePrerequisites.h"
#include "OgrePlugin.h"
#include "OgreCommon.h"
#include "OgreDataStream.h"
#include "OgreTexture.h"
#include "SpacescapeProgressListener.h"

namespace Ogre
{
    // forward declaration
    class SpacescapeLayer;

    /** The SpacescapePlugin class is an Ogre Plugin.  It creates 
    and manages SpacescapeLayer objects and can open and save .xml 
    files and export skyboxes, or export a skybox to an Ogre::Material.
    */
	class _SpacescapePluginExport SpacescapePlugin : public Plugin
	{
	public:
		SpacescapePlugin();

        // Valid SpacescapeLayer types
        enum SpacescapeLayerType
        {
            SLT_POINTS = 0,
            SLT_BILLBOARDS,
            SLT_NOISE
        };

        typedef std::vector<SpacescapeLayer*> SpacescapeLayerList;

        /** Add a layer with the given params
        @param type The layer type (see the SpacescapeLayerType enum)
        @param params The list of name & value pairs that are parameters for the layer type
        @return the layer id of the created layer or -1 on error
        */
        int addLayer(int type, const NameValuePairList& params);

        /** Add a progress listener for receiving progress updates
        @param listener The listener to add
        */
        void addProgressListener(SpacescapeProgressListener* listener);

        /** Clear all layers
        @return true on success
        */
        bool clear();

        /** Delete a layer
        @return true on success
        */
        bool deleteLayer(unsigned int layerId);

        /** Duplicate a layer
        @param layerId The layer to duplicate
        @return the layer id of the created layer or -1 on error
        */
        int duplicateLayer(unsigned int layerId);

        /** Get copy of layers list
        @return List of layers
        */
        SpacescapeLayerList getLayers() { return mLayers; }

		/// @copydoc Plugin::getName
		const String& getName() const;

        /** Get the root scene node for all the layers
        @return the SceneNode pointer for all the layers
        */
        SceneNode* getSceneNode() { return mSceneNode; }

        /** Get a unique id - used by layers for unique material names etc.
        @return a unique id
        */
        unsigned long long getUniqueId() { return mUniqueId++; }

		/// @copydoc Plugin::install
		void install();

		/// @copydoc Plugin::initialise
		void initialise();

        /** Load a config file
        @param stream The stream of the file to read
        @return true on success, false on error
        */
        bool loadConfigFile(DataStreamPtr& stream);

        /** Load a config file
        @param filename The filename of the config file to load
        @return true on success, false on error
        */
        bool loadConfigFile(const String& filename);

       /** Get the power of 2 for this value. Credit to :
        http://www.southwindsgames.com/blog/2009/01/19/fast-integer-log2-function-in-cc/
        @param x the value to check
        */
        static inline unsigned int _log2(register unsigned int x)
        {
            register unsigned int l=0;
            if(x >= 1<<16) { x>>=16; l|=16; }
            if(x >= 1<<8) { x>>=8; l|=8; }
            if(x >= 1<<4) { x>>=4; l|=4; }
            if(x >= 1<<2) { x>>=2; l|=2; }
            if(x >= 1<<1) l|=1;
            return l;
        }

        /** Move a layer up or down by the specified amount
        @param layerId The layer id of the layer to move
        @return true on success, false on error
        */
        bool moveLayer(unsigned int layerId, int amount);

        /** Remove a progress listener
        @param listener The listener to remove
        */
        void removeProgressListener(SpacescapeProgressListener* listener);

        /** Save a config file
        @param filename The filename of the config file to save
        @return true on success, false on error
        */
        bool saveConfigFile(const String& filename);

        /** Show or hide a layer
        @param layerId the layer to hide/show
        @param visible true to show, false to hide
        */
        void setLayerVisible(unsigned int layerId, bool visible);

		/// @copydoc Plugin::shutdown
		void shutdown();

        /** Toggle the visibility of a layer
        @param layerId the layer to hide/show
        */
        void toggleLayerVisible(unsigned int layerId);

		/// @copydoc Plugin::uninstall
		void uninstall();

        /** Update a layer with new params
        @param layerId The layer id of the layer to update
        @param params The params to update the layer with
        @return true on success, false on error
        */
        bool updateLayer(unsigned int layerId, const NameValuePairList& params);

        /** Write the skybox to a file
        @param filename The filename (and path) of the file to write (i.e. "../skyboxes/myskybox.png")
        @param type The filetype.  TEX_TYPE_2D will be written as 6 
        separate images, while TEX_TYPE_3D files will be written as a single
        file (currently only supports .dds 3d file types)
        */
        void writeToFile(const String& filename, unsigned int size = 1024, TextureType type = TEX_TYPE_2D);

        /** Write the skybox to a material
        @param materialName The name of the material to write the skybox to
        @param the skybox size
        @remarks if the material specified doesn't exist it will be created.
        */
        void writeToMaterial(const String& materialName, unsigned int size = 1024);

        /** Write the skybox to a material
        @param mat The material to write the skybox to
        @param the skybox size
        */
        void writeToMaterial(MaterialPtr& mat, unsigned int size = 1024);

        /** For internal use only - layers use this render to texture function
        to render to texture
        @param texture The texture to rtt to - must be cubic
        @param numMipMaps Number of mip maps to create - use -1 for auto and 0 for none
        */
        bool _rtt(TexturePtr& texture, int numMipMaps);
 
    private:
        /** Utility function to send progress events to all listeners
        @param percentComplete Percent complete
        @param msg Task status message
        */
        void updateProgress(unsigned int percentComplete, const String& msg);

        /** Utility function to update the render to texture surface & mipmaps
        for all 6 faces of the skybox with the current settings
        @param size The size / resolution of the skybox image
        @return whether the update succeeded or not
        */
        bool updateRTT(unsigned int size);

        // layers list
        SpacescapeLayerList mLayers;

        // progress listeners
        std::vector<SpacescapeProgressListener* > mProgressListeners;

        // scene node all layers are attached to
        SceneNode* mSceneNode;

        // a unique id used for getting unique material/texture names
        unsigned int mUniqueId;
	};
}

#endif
