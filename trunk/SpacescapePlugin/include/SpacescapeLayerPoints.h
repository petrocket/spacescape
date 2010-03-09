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
#ifndef __SPACESCAPELAYERPOINTS_H__
#define __SPACESCAPELAYERPOINTS_H__

#include "SpacescapePrerequisites.h"
#include "SpacescapeLayer.h"

namespace Ogre
{
    /** The SpacescapeLayerPoints class defines a layer of a space 
    background that draws point stars - solid square single color points.
    */
    class _SpacescapePluginExport SpacescapeLayerPoints : public SpacescapeLayer
    {
    public:
        /** Constructor
        */
        SpacescapeLayerPoints(const String& name, SpacescapePlugin* plugin);

        /** Destructor
        */
        ~SpacescapeLayerPoints(void);

       /** Get the layer type
        @return the layer type
        */
        String getLayerTypeName(void) { return "points"; }

        /** Get the layer type
        @return the layer type
        */
        int getLayerType(void) { return SpacescapePlugin::SLT_POINTS; }

        /** Initialize this layer based on the given params
        @remarks Params for this layer type are:
        NAME -  VALUE TYPE
        farColor - ColourValue (string i.e. "0.0 0.5 1.0")
        nearColor - ColourValue (string i.e. "0.1 0.9 1.0")
        numPoints - unsigned int (string i.e. "100")
        pointSize - unsigned int (string i.e. "1")
        @param params Layer params that will be specific to the derived class
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

        /** Utility function for building based on class params when masked
        */
        void buildMasked(void);

        /** Create the material we'll need if not created already
        */
        void createMaterial(void);

        // dest blend factor
        SceneBlendFactor mDestBlendFactor;

        // far color
        Ogre::ColourValue mFarColor; 
        
        // built flag
        bool mBuilt;

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

        // material
        MaterialPtr mMaterial;

        // near color
        Ogre::ColourValue mNearColor;

        // num points
        unsigned int mNumPoints;

        // point size
        unsigned int mPointSize;

        // source blend factor
        SceneBlendFactor mSourceBlendFactor;
    };
}
#endif