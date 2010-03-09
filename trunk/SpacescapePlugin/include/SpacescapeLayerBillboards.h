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
#ifndef __SPACESCAPELAYERBILLBOARDS_H__
#define __SPACESCAPELAYERBILLBOARDS_H__

#include "SpacescapePrerequisites.h"
#include "SpacescapeLayer.h"
#include "OgreBillboardSet.h"

namespace Ogre
{
    /** The SpacescapeLayerBillboards class defines a layer of a space 
    background that draws billboards (textured sprites that always face
    the camera)
    */
    class _SpacescapePluginExport SpacescapeLayerBillboards : public SpacescapeLayer
    {
    public:
        /** Constructor
        */
        SpacescapeLayerBillboards(const String& name, SpacescapePlugin* plugin);

        /** Destructor
        */
        ~SpacescapeLayerBillboards(void);

       /** Get the layer type
        @return the layer type
        */
        String getLayerTypeName(void) { return "billboards"; }

        /** Get the layer type
        @return the layer type
        */
        int getLayerType(void) { return SpacescapePlugin::SLT_BILLBOARDS; }

       /** Return our billboard set
        @return this object instance
        */
        MovableObject* getMovableObject() { return mBillboardSet; }

        /** Initialize this layer based on the given params
        @remarks Params for this layer type are:
        NAME -  VALUE TYPE
        destBlendFactor - string (i.e. one, dest_colour  etc.)
        minSize - Real (i.e. 1.0)
        maxSize - Real (i.e. 2.0)
        numBillboards - int (i.e. 200 etc.)
        seed - int (i.e. 2,3 etc.)
        sourceBlendFactor - string (i.e. one, dest_colour  etc.)
        texture - string (i.e. "my-flare.png")
        @param params Layer params that are specific to this class
        */
        void init(Ogre::NameValuePairList params);

    protected:
        /** Utility function for updating saved params list
        @param params The list of params
        */
        void updateParams(NameValuePairList params);

    private:
        /** Utility function for building based on class params
        */
        void build(void);

        /** Utility function for building based on class params (masked)
        */
        void buildMasked(void) ;

        /** Update the material with new params - will create if needed
        */
        void updateMaterial(void);

        // billboard set to use
        BillboardSet* mBillboardSet;

        // built flag
        bool mBuilt;

        // destination blend factor
        SceneBlendFactor mDestBlendFactor;

        // far color
        Ogre::ColourValue mFarColor; 

        // flag to enable/disable the noise mask
        bool mMaskEnabled;

        // noise scale
        Real mMaskScale;

        // mask noise gain
        Real mMaskGain;

        // mask noise lacunarity
        Real mMaskLacunarity;

        // mask noise type - either "fbm" or "ridged"
        String mMaskNoiseType;

        // octaves for noise
        unsigned int mMaskOctaves;

        // ridged noise offset
        Real mMaskOffset;

        // noise power
        Real mMaskPower;

        // random seed for mask
        unsigned int mMaskSeed;

        // threshold for the mask (floor / cutoff)
        Real mMaskThreshold;

        // material to use for billboard set
        MaterialPtr mMaterial;

        // maximum billboard size
        Real mMaxSize;

        // minimum billboard size
        Real mMinSize;

        // near color
        Ogre::ColourValue mNearColor;

        // number of billboards
        unsigned int mNumBillboards;

        // source blend factor
        SceneBlendFactor mSourceBlendFactor;

        // name of our sprite texture
        String mTextureName;
    };
}
#endif