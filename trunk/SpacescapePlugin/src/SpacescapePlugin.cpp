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
#include "SpacescapePlugin.h"
#include "SpacescapeLayerBillboards.h"
#include "SpacescapeLayerNoise.h"
#include "SpacescapeLayerPoints.h"
#include "OgreRoot.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreRenderTarget.h"
#include "OgreRenderTexture.h"
#include "OgreSceneNode.h"
#include <iostream>
#include "ticpp.h"

namespace Ogre 
{
	const String sPluginName = "Spacescape";

    SpacescapePlugin::SpacescapePlugin() :
        mSceneNode(0),
        mUniqueId(0)
	{
        mProgressListeners.clear();
	}

    /** Add a layer with the given params
    @param type The layer type (see the SpacescapeLayerType enum)
    @param params The list of name & value pairs that are parameters for the layer type
    @return the layer id of the created layer or -1 on error
    */
    int SpacescapePlugin::addLayer(int type, const NameValuePairList& params)
    {
        // get the next layer id
        unsigned int layerId = (unsigned int)mLayers.size();
        String layerName = "SpacescapeLayer" + StringConverter::toString(layerId);

        // create the scene node if it doesn't already exist
        if(!mSceneNode) {
            // get the default scene manager
            if(!Ogre::Root::getSingleton().getSceneManagerIterator().hasMoreElements()) {
                Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
                    "No scene manager found in SpacescapePlugin::addLayer().  You can't add a layer before creating a scene manager.";
            }
            SceneManager* sceneMgr = Root::getSingleton().getSceneManagerIterator().peekNextValue();
            mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode("SpacescapeNode");
        }

        if(type == SLT_BILLBOARDS) {
            Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
                "Creating Billboards SpacescapeLayer";

            // create the layer
            mLayers.push_back(OGRE_NEW SpacescapeLayerBillboards(layerName,this));
        }
        else if(type == SLT_NOISE) {
            Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
                "Creating Noise SpacescapeLayer";
            
            // create the layer
            mLayers.push_back(OGRE_NEW SpacescapeLayerNoise(layerName,this));
        }

        else if(type == SLT_POINTS) {
            Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
                "Creating Points SpacescapeLayer";

            // create the layer
            mLayers.push_back(OGRE_NEW SpacescapeLayerPoints(layerName,this));
        }
        else {
            // unknown type
            Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
                "Unkown SpacescapeLayerType: " << type;
            return -1;
        }

        // set layer id and render queue
        mLayers[layerId]->setLayerID(layerId);

        // init with the given params
        mLayers[layerId]->init(params);
        mLayers[layerId]->getMovableObject()->setRenderQueueGroup(RENDER_QUEUE_SKIES_EARLY + layerId);

        // attach the layer to the scene so it can be displayed
        mSceneNode->createChildSceneNode(layerName)->attachObject(mLayers[layerId]->getMovableObject());

        return layerId;
    }

    /** Add a progress listener for receiving progress updates
    @param listener The listener to add
    */
    void SpacescapePlugin::addProgressListener(SpacescapeProgressListener* listener)
    {
        // make sure this listener isn't already in the list
        for(unsigned int i = 0; i < mProgressListeners.size(); i++) {
            if(mProgressListeners[i] == listener) {
                // already in the list
                return;
            }
        }

        // add to the list
        mProgressListeners.push_back(listener);
    }

    /** Clear all layers
    @return true on success
    */
    bool SpacescapePlugin::clear()
    {
        // check if the scene manager still exists
        if(!Ogre::Root::getSingleton().getSceneManagerIterator().hasMoreElements()) {
            Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
                "SpacescapePlugin::clear() called with no scenemanagers.";
        }
        else {
            SceneManager* sceneMgr = Root::getSingleton().getSceneManagerIterator().peekNextValue();

            // it is possible that this function is called after the plugin has been shutdown etc.
            // so the scene node pointer may be invalid
            if(!sceneMgr->hasSceneNode("SpacescapeNode")) {
                mSceneNode = NULL;
            }
            else if(mSceneNode) {
                // detach all layer nodes and their objects
                mSceneNode->removeAndDestroyAllChildren();
            }
        }

        if(mLayers.empty()) {
            return false;
        }

        // delete all allocated layers
        for(unsigned int i = 0; i < mLayers.size(); ++i) {
            OGRE_DELETE mLayers[i];
        }

        mLayers.clear();

        return true;
    }


    /** Delete a layer
    @return true on success
    */
    bool SpacescapePlugin::deleteLayer(unsigned int layerId)
    {
        if(layerId < mLayers.size() && !mLayers.empty()) {
            OGRE_DELETE mLayers[layerId];
            mLayers.erase(mLayers.begin() + layerId);

            // detach all layer nodes and their objects
            if(mSceneNode) {
                mSceneNode->removeAndDestroyAllChildren();
            }

            // update the layer ids for all the other layers
            // and re-attach all layers to the scene
            for(unsigned int i = 0; i < mLayers.size(); ++i) {
                mLayers[i]->setLayerID(i);
                if(mSceneNode) {
                    String layerName = "SpacescapeLayer" + StringConverter::toString(i);
                    mLayers[i]->getMovableObject()->setRenderQueueGroup(RENDER_QUEUE_SKIES_EARLY + i);
                    mSceneNode->createChildSceneNode(layerName)->attachObject(mLayers[i]->getMovableObject());
                }
            }
            return true;
        }

        return false;
    }

    /** Duplicate a layer
    @param layerId The layer to duplicate
    @return the layer id of the created layer or -1 on error
    */
    int SpacescapePlugin::duplicateLayer(unsigned int layerId)
    {
        if(layerId < mLayers.size() && !mLayers.empty()) {
            // duplicate!
            int newLayerId = addLayer(mLayers[layerId]->getLayerType(),mLayers[layerId]->getParams());
            if(newLayerId) {
                // move the copied layer to be just on top of the original layer
                int distance = newLayerId - layerId - 1;
                if(distance) {
                    moveLayer(newLayerId, -distance);
                }

                // append the word "copy" to the layer name
                NameValuePairList newParams;
                newParams["name"] = mLayers[layerId]->getParams()["name"] + " copy";
                mLayers[layerId + 1]->init(newParams);
            }

            return layerId + 1;
        }

        return -1;
    }

    const String& SpacescapePlugin::getName() const
	{
		return sPluginName;
	}

    void SpacescapePlugin::install()
	{
	}

    void SpacescapePlugin::initialise()
	{
	}

    /** Load a config file
    @param stream The stream of the file to read
    @return true on success, false on error
    */
    bool SpacescapePlugin::loadConfigFile(DataStreamPtr& stream)
    {
        ticpp::Document config;
        ticpp::Iterator<ticpp::Element> child;
        unsigned int numLayers = 0;
        unsigned int currentLayer = 1;
        unsigned int progressAmount = 0;

        // update progress
        updateProgress(progressAmount, "Loading .xml");

        try {
            config.Parse(stream->getAsString());
        }
        catch(ticpp::Exception& e) {
            LogManager::getSingleton().getDefaultLog()->logMessage("Invalid XML file " + String(e.what()));
            return false;
        }

        // update progress amount
        progressAmount += 10;

        // clear the current scene first
        clear();

        // get number of layers to add
        for(child = child.begin(config.FirstChildElement()); child != child.end(); child++) {
            std::string key;

            // top level items should be layers
            child->GetValue(&key);

            if(key == "layer") {
                numLayers++;
            }
        }

        for(child = child.begin(config.FirstChildElement()); child != child.end(); child++) {
            std::string key, value;

            // top level items are layers
            child->GetValue(&key);

            if(key == "layer") {
                // default layer type is points
                int layerType = SLT_POINTS;

                // params to pass to addLayer function()
                NameValuePairList params;

                // go through all the layer parameters
                ticpp::Iterator<ticpp::Element> subchild;
                for(subchild = subchild.begin(child->ToElement()); subchild != subchild.end(); subchild++) {
                    subchild->GetValue(&key);

                    // get the layer type
                    if(key == "type") {
                        subchild->GetTextOrDefault(&value,"points");
                        if(value == "billboards") {
                            layerType = SLT_BILLBOARDS;
                        }
                        else if(value == "noise") {
                            layerType = SLT_NOISE;
                        }
                        else {
                            layerType = SLT_POINTS;
                        }
                    }
                    else {
                        // add this to the params list
                        subchild->GetTextOrDefault(&params[key],"");
                    }
                }

                // update progress
                if(params["name"].empty()) {
                    updateProgress(progressAmount, "Creating layer " + 
                        StringConverter::toString(currentLayer) + " of " +
                        StringConverter::toString(numLayers));
                }
                else {
                    updateProgress(progressAmount, "Creating layer " + params["name"] + " (" + 
                        StringConverter::toString(currentLayer) + " of " +
                        StringConverter::toString(numLayers) + ")");
                }

                // add the layer
                addLayer(layerType, params);

                // update progress amount
                progressAmount += (unsigned int)(90.0 / (Real)numLayers);
                progressAmount = std::min<unsigned int>(progressAmount, 100);
                currentLayer++;
            }
        }

        // update progress
        updateProgress(100, "Layers created");

        return true;
    }

    /** Load a config file
    @param filename The filename of the config file to load
    @return true on success, false on error
    */
    bool SpacescapePlugin::loadConfigFile(const String& filename)
    {
        LogManager::getSingleton().getDefaultLog()->logMessage("Loading " + filename);

		// try to open in the current folder first
		std::ifstream fs;
		fs.open(filename.c_str(), std::ios::in | std::ios::binary);
		if (fs) {
			// Wrap as a stream
            DataStreamPtr stream(OGRE_NEW FileStreamDataStream(filename, &fs, false));
            bool result = loadConfigFile(stream);
            stream.setNull();
            fs.close();

            return result;
		}
		else
		{
			// otherwise try resource system
			DataStreamPtr stream = 
				ResourceGroupManager::getSingleton().openResource(filename, 
					ResourceGroupManager::getSingleton().getWorldResourceGroupName());
            if(stream.isNull()) {
                LogManager::getSingleton().getDefaultLog()->logMessage("Empty config file: " + filename);
                return false;
            }

			return loadConfigFile(stream);
		}
    }

    /** Move a layer up or down by the specified amount
    @param layerId The layer id of the layer to move
    @return true on success, false on error
    */
    bool SpacescapePlugin::moveLayer(unsigned int layerId, int amount)
    {
        if(layerId >= mLayers.size() || amount == 0) {
            return false;
        }

        SpacescapeLayer* l = mLayers[layerId];

        if(amount < 0) {
            amount = std::max<int>(-(int)layerId,amount);
            mLayers.insert(mLayers.begin() + layerId + amount,l);
            mLayers.erase(mLayers.begin() + layerId + 1);
        }
        else {
            amount = std::min<int>((int)mLayers.size() - layerId - 1,amount);
            mLayers.insert(mLayers.begin() + layerId + amount + 1,l);
            mLayers.erase(mLayers.begin() + layerId);
        }

        if(amount != 0) {
           // detach all layer nodes and their objects
            if(mSceneNode) {
                mSceneNode->removeAndDestroyAllChildren();
            }

            // update the layer ids for all the other layers
            // and re-attach all layers to the scene
            for(unsigned int i = 0; i < mLayers.size(); ++i) {
                mLayers[i]->setLayerID(i);
                mLayers[i]->getMovableObject()->setRenderQueueGroup(RENDER_QUEUE_SKIES_EARLY + i);
                if(mSceneNode) {
                    String layerName = "SpacescapeLayer" + StringConverter::toString(i);
                    mSceneNode->createChildSceneNode(layerName)->attachObject(mLayers[i]->getMovableObject());
                }
            }
        }

        return amount != 0;
    }

    /** Remove a progress listener
    @param listener The listener to remove
    */
    void SpacescapePlugin::removeProgressListener(SpacescapeProgressListener* listener)
    {
        // find and remove from our list of listeners
        std::vector<SpacescapeProgressListener* >::iterator ii;
        for( ii = mProgressListeners.begin(); ii != mProgressListeners.end(); ii++) {
            if(*ii == listener) {
                mProgressListeners.erase(ii);
            }
        }
    }

    /** For internal use only - layers use this render to texture function
    to render to texture
    @param texture The texture to rtt to - must be cubic
    @param numMipMaps Number of mip maps to create - use -1 for auto and 0 for none
    */
    bool SpacescapePlugin::_rtt(TexturePtr& texture, int numMipMaps)
    {
        if(!mSceneNode) {
            return false;
        }

        SceneManager* mgr = mSceneNode->getCreator();
        
        // get the rtt camera
        Camera* rttCam;
        if(!mgr->hasCamera("RTTCam")) {
            rttCam = mgr->createCamera("RttCam");

            // initialize the camera
            rttCam->lookAt(Vector3::UNIT_Z);
            rttCam->setAspectRatio(1.0);
            rttCam->setFOVy(Radian(Degree(90.0))); // 90 degree fov
            rttCam->setNearClipDistance(0.1f);
            rttCam->setFarClipDistance(1000);
        }
        else {
            rttCam = mgr->getCamera("RttCam");
        }

         // attach rtt cam to scene
        mSceneNode->attachObject(rttCam);
 
        int numMips = (numMipMaps == -1) ? SpacescapePlugin::_log2((uint)texture->getWidth()) : numMipMaps;
        
        // be sure to not go negative
        numMips = std::max<int>(0,numMips);

        // point the camera in six different directions and rtt
        Vector3 forward,up,right;
        for(int i = 0; i < 6; i++) {
		    switch(i)
		    {
		    case 0:
                // right
                forward = Vector3::NEGATIVE_UNIT_X;
			    up = Vector3::UNIT_Y ;
			    right = Vector3::UNIT_Z;
			    break;
		    case 1:
                // left??
                forward = Vector3::UNIT_X;
			    up = Vector3::UNIT_Y ;
			    right = Vector3::NEGATIVE_UNIT_Z;
			    break;
		    case 2:
                // top
                forward = Vector3::NEGATIVE_UNIT_Y ;
			    up = Vector3::UNIT_Z ;
			    right = Vector3::UNIT_X;
			    break;
		    case 3:
                // down?
                forward = Vector3::UNIT_Y ;
			    up = Vector3::NEGATIVE_UNIT_Z ;
			    right = Vector3::UNIT_X;
			    break;
		    case 4:
                // correct (forward)
                forward = Vector3::UNIT_Y;
			    up = Vector3::UNIT_Z;
			    right = Vector3::UNIT_X;
			    break;
		    case 5:
                // correct (opposite of 4) back
                forward = Vector3::NEGATIVE_UNIT_Z;
			    up = Vector3::UNIT_Y ;
			    right = Vector3::NEGATIVE_UNIT_X ;
			    break;
		    }

            Quaternion q;
            q.FromAxes(right,up,forward);
            rttCam->setOrientation(q);

            for(int j = 0; j <= numMips; ++j) {
                // get render target for mipmap
                RenderTarget* target = (RenderTarget*)texture->getBuffer(i,j)->getRenderTarget();
                target->setAutoUpdated(false);
                target->setActive(true);

                // add viewport to RTT texture
                Viewport* v = target->addViewport(rttCam);
                v->setOverlaysEnabled(false);

                // setting either of these messes things up
                v->setBackgroundColour(ColourValue::Black); 
                v->setSkiesEnabled(false);
                v->setClearEveryFrame(true);
                v->update();

                // remove the viewports we created
                target->removeAllViewports();
            }
        }
        texture->load();

        mSceneNode->detachObject(rttCam);
        mgr->destroyCamera(rttCam);

        return true;
    }

    /** Save a config file
    @param filename The filename of the config file to save
    @return true on success, false on error
    */
    bool SpacescapePlugin::saveConfigFile(const String& filename)
    {
        if(mLayers.empty()) {
            return false;
        }

        try {
            // create an xml file from layer params
            ticpp::Document config;
            ticpp::Declaration *decl = new ticpp::Declaration ("1.0", "utf-8", "");
            config.LinkEndChild(decl);

            ticpp::Element elem("spacescapelayers");
            ticpp::Element *n = config.InsertEndChild(elem)->ToElement();
            for(unsigned int i = 0; i < mLayers.size(); i++) {
                // create the layer node
				ticpp::Element temp_element = ticpp::Element("layer");
                ticpp::Element *layerNode = n->InsertEndChild(temp_element)->ToElement();

                // create all the layer data nodes
                NameValuePairList pl = mLayers[i]->getParams();
                NameValuePairList::iterator pli;
                for(pli = pl.begin(); pli != pl.end(); pli++) {
                    ticpp::Element layerElem;
                    layerElem.SetValue(pli->first.c_str());
                    layerElem.SetText(pli->second.c_str());

                    // append this node
                    layerNode->InsertEndChild(layerElem);   
                }
            }
            // save the xml file
            config.SaveFile(filename.c_str());

            delete decl;
        }
        catch(ticpp::Exception& e) {
            LogManager::getSingleton().getDefaultLog()->logMessage("saveConfigFile()  " + String(e.what()));
            return false;
        }

        return true;
    }

    /** Show or hide a layer
    @param layerId the layer to hide/show
    @param visible true to show, false to hide
    */
    void SpacescapePlugin::setLayerVisible(unsigned int layerId, bool visible)
    {
        if(!mSceneNode || layerId >= mLayers.size()) {
            return;
        }

        String layerName = "SpacescapeLayer" + StringConverter::toString(layerId);

        SceneNode* n = (SceneNode*)mSceneNode->getChild(layerName);
        if(n) {
            n->setVisible(visible);
        }
    }

    void SpacescapePlugin::shutdown()
	{
        clear();
	}

    /** Toggle the visibility of a layer
    @param layerId the layer to hide/show
    */
    void SpacescapePlugin::toggleLayerVisible(unsigned int layerId)
    {
        if(!mSceneNode || layerId >= mLayers.size()) {
            return;
        }

        String layerName = "SpacescapeLayer" + StringConverter::toString(layerId);

        SceneNode* n = (SceneNode*)mSceneNode->getChild(layerName);
        if(n) {
            n->flipVisibility();
        }
    }

    void SpacescapePlugin::uninstall()
	{
        clear();
	}

    /** Update a layer with new params
    @param layerId The layer id of the layer to update
    @param params The params to update the layer with
    @return true on success, false on error
    */
    bool SpacescapePlugin::updateLayer(unsigned int layerId, const NameValuePairList& params)
    {
        if(layerId >= mLayers.size()) {
            return false;
        }

        // has the layer type changed?
        String layerType = params.find("type") == params.end() ? "" : params.find("type")->second;

        if(layerType != "" && layerType != mLayers[layerId]->getLayerTypeName()) {
            // destroy this layer and re-create in the same location with the new type

            String layerName = mLayers[layerId]->getName();

            // we need a writable version of params to copy some old params like the seed
            NameValuePairList newParams = params;
            NameValuePairList oldParams = mLayers[layerId]->getParams();

            // detach old object from the layer scene node
            SceneNode* n =(SceneNode*)mSceneNode->getChild("SpacescapeLayer" + StringConverter::toString(layerId));
            n->detachAllObjects();

            // delete the layer
            OGRE_DELETE mLayers[layerId];

            // create the new layer
            if(layerType == "billboards") {
                mLayers[layerId] = OGRE_NEW SpacescapeLayerBillboards(layerName,this);
            }
            else if(layerType == "noise") {
                mLayers[layerId] = OGRE_NEW SpacescapeLayerNoise(layerName,this);
            }
            else {
                mLayers[layerId] = OGRE_NEW SpacescapeLayerPoints(layerName,this);
            }

            // set layer id
            mLayers[layerId]->setLayerID(layerId);

            // copy the seed if not specified already
            if(newParams.find("seed") == newParams.end()) {
                newParams["seed"] = oldParams["seed"];
            }

            // init with the given params and set render queue
            mLayers[layerId]->init(newParams);
            mLayers[layerId]->getMovableObject()->setRenderQueueGroup(RENDER_QUEUE_SKIES_EARLY + layerId);

            // attach the layer to the scene so it can be displayed
            n->attachObject(mLayers[layerId]->getMovableObject());
        }
        else {
            // run initialization again with given params
            mLayers[layerId]->init(params);
        }

        return true;
    }

   /** Utility function to send progress events to all listeners
    @param percentComplete Percent complete
    @param msg Task status message
    */
    void SpacescapePlugin::updateProgress(unsigned int percentComplete, const String& msg)
    {
        if(mProgressListeners.empty()) {
            return;
        }

        // send to all listeners
        for(unsigned int i = 0; i < mProgressListeners.size(); ++i) {
            mProgressListeners[i]->updateProgressBar(percentComplete, msg);
        }
    }

    /** Utility function to update the render to texture surface & mipmaps
    for all 6 faces of the skybox with the current settings
    @param size The size / resolution of the skybox image
    @return whether the update succeeded or not
    */
    bool SpacescapePlugin::updateRTT(unsigned int size)
    {
        if(!mSceneNode || mLayers.empty()) {
            return false;
        }

        // don't do mip maps  because some ATI cards x1950 pro have issues and
        // draw the mipmaps on the same surface
        int numMips = 0;//SpacescapePlugin::_log2(size);

        bool createTexture = false;

        TexturePtr rtt = TextureManager::getSingleton().getByName("SpacescapeRTT");
        if(rtt.isNull()) {
            createTexture = true;
        }
        else {
            // check if the size has changed or num mipmaps
            if(rtt->getWidth() != size || rtt->getHeight() != size || 
                rtt->getNumMipmaps() != numMips) {
                TextureManager::getSingleton().remove(rtt->getHandle());
                createTexture = true;
            }
        }

        if(createTexture) {
            // create the rtt texture
            rtt = TextureManager::getSingleton().createManual(
                "SpacescapeRTT",
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                TEX_TYPE_CUBE_MAP,
                size, size,
                1,
                numMips,
                PF_BYTE_RGB,
                TU_RENDERTARGET
            );
        }

        // tell all the layers to display high res versions
        for(unsigned int i = 0; i < mLayers.size(); i++) {
            mLayers[i]->setDisplayHighRes(true);
        }

        // render to texture
        bool result = _rtt(rtt, numMips);

        // tell all the layers to go back to low res displays
        for(unsigned int i = 0; i < mLayers.size(); i++) {
            mLayers[i]->setDisplayHighRes(false);
        }

        return result;
    }

    /** Write the skybox to a file
    @param filename The filename (and path) of the file to write
    @param type The filetype.  TEX_TYPE_2D will be written as 6 
    separate images, while TEX_TYPE_3D files will be written as a single
    file (currently only supports .dds 3d file types)
    */
    void SpacescapePlugin::writeToFile(const String& filename, unsigned int size, TextureType type)
    {
        unsigned int progressAmount = 0;

        Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
            "Writing to file " << filename << " size: " << StringConverter::toString(size);

        // update progress
        updateProgress(progressAmount,"Updating RTT");

        Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
            "Updating RTT";

        // first update the rtt texture
        updateRTT(size);

        // update progress
        progressAmount+= 40;

        // get number of mip maps - disabled for now because ATI x1950 pro draws them funny
        // and it doesn't look like we need them
        //int numMips = SpacescapePlugin::_log2(size);
        int numMips = 0;

        // get the render to texture object
        TexturePtr rtt = TextureManager::getSingleton().getByName("SpacescapeRTT");

        if(type == TEX_TYPE_2D) {
            String suffixes[6] = {
                "right1",
                "left2",
                "top3",
                "bottom4",
                "front5",
                "back6"
            };

            // default extension/type is png
            String ext = ".png";
            String basename = filename;
            if(filename.length() > 4) {
                // use the extension provided (assume 4 characters in length)
                ext = filename.substr(filename.length() - 4, 4);
                basename = filename.substr(0,filename.length() - 4);
            }
            
            // write out six textures
            for(int i = 0; i < 6; ++i) {
                Image* img = OGRE_NEW Image();

                Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
                    "Saving image " << basename << "_" << suffixes[i] << ext;

                // update progress
                updateProgress(progressAmount,"Exporting " + suffixes[i]);

                // allocate room for this image and its mip maps
                size_t numBytes = img->calculateSize(numMips,1,size,size,1,PF_BYTE_RGB);
                uchar* data = OGRE_ALLOC_T(uchar,numBytes,MEMCATEGORY_GENERAL);

                // load all the data into the image
                img->loadDynamicImage(data,size,size,1,PF_BYTE_RGB,false,1,numMips);
                for(int j = 0; j <= numMips; ++j) {
                    rtt->getBuffer(i,j)->getRenderTarget()->copyContentsToMemory(
                        img->getPixelBox(0,j),
                        RenderTarget::FB_FRONT
                    );
                }

                // tell the image to save out in the requested format
                // this internal Ogre function will handle format issues
                // filename is basename with our suffix and the original extension
                img->save(basename + "_" + suffixes[i] + ext);

                OGRE_FREE(data,MEMCATEGORY_GENERAL);
                OGRE_DELETE img;

                // update progress
                progressAmount += 10;
            }
        }
        else {
            // assume cubic/3d .dds texture
            Image* img = OGRE_NEW Image();
            size_t numBytes = img->calculateSize(numMips,6,size,size,1,PF_BYTE_RGB);

            // allocate room for this image and its mip maps
            uchar* data = OGRE_ALLOC_T(uchar,numBytes,MEMCATEGORY_GENERAL);

            // load all the data into the image
            img->loadDynamicImage(data,size,size,1,PF_BYTE_RGB,false,6,numMips);

            // combine the six textures into one image with 6 faces
            for(int i = 0; i < 6; ++i) {
                for(int j = 0; j <= numMips; ++j) {
                    rtt->getBuffer(i,j)->getRenderTarget()->copyContentsToMemory(
                        img->getPixelBox(i,j),
                        RenderTarget::FB_FRONT
                    );
                }
            }

            // tell the image to save out in the requested format
            // this internal Ogre function will handle format issues
            img->save(filename);

            OGRE_FREE(data,MEMCATEGORY_GENERAL);
            OGRE_DELETE img;
        }

        updateProgress(100, "Export complete");

        Ogre::LogManager::getSingleton().getDefaultLog()->stream() << 
            "Export complete";

        // unload the rtt texture
        //TextureManager::getSingletonPtr()->unload(rtt->getHandle());
    }

    /** Write the skybox to a material
    @param materialName The name of the material to write the skybox to
    @remarks if the material specified doesn't exist it will be created.
    */
    void SpacescapePlugin::writeToMaterial(const String& materialName, unsigned int size)
    {
        // create the material if it doesn't exist already
        MaterialPtr m = MaterialManager::getSingleton().getByName(materialName);
        if(m.isNull()) {
            m = MaterialManager::getSingleton().create(materialName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        }

        writeToMaterial(m,size);
    }

    /** Write the skybox to a material
    @param mat The material to write the skybox to
    */
    void SpacescapePlugin::writeToMaterial(MaterialPtr& mat, unsigned int size)
    {
        if(mat.isNull()) {
            return;
        }

        // update the rtt
        updateRTT(size);

        Pass* p = mat->getTechnique(0)->getPass(0);

        // create a texture unit if none exists
        if(!p->getNumTextureUnitStates()) {
            p->createTextureUnitState();
        }

        p->setLightingEnabled(false);
        p->setDepthWriteEnabled(false);
        p->setTextureFiltering(TFO_ANISOTROPIC);
        

        // assign the rtt texture we created
        p->getTextureUnitState(0)->setCubicTextureName("SpacescapeRTT",true);
        p->getTextureUnitState(0)->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);

        mat->load();
    }
}
