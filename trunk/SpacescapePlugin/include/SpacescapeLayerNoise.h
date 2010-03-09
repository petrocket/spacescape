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
#ifndef __SPACESCAPELAYERNOISE_H__
#define __SPACESCAPELAYERNOISE_H__

#include "SpacescapePrerequisites.h"
#include "SpacescapeLayer.h"
#include "OgreTexture.h"
#include "OgreSceneManager.h"

namespace Ogre
{
    /** The SpacescapeLayerNoise class defines a layer of a space 
    background that draws FBM (Fractal Brownian Motion) noise or Ridged FBM
    noise.
    */
    class _SpacescapePluginExport SpacescapeLayerNoise : public SpacescapeLayer
    {
    public:
        /** Constructor
        */
        SpacescapeLayerNoise(const String& name, SpacescapePlugin* plugin);

        /** Destructor
        */
        ~SpacescapeLayerNoise(void);

       /** Get the layer type
        @return the layer type
        */
        String getLayerTypeName(void) { return "noise"; }

        /** Get the layer type
        @return the layer type
        */
        int getLayerType(void) { return SpacescapePlugin::SLT_NOISE; }

        /** Initialize this layer based on the given params
        @remarks Params for this layer type are:
        NAME -  VALUE TYPE
        destBlendFactor - string (i.e. one, dest_colour  etc.)
        ditherAmount - real (string i.e. "0.1") should be in range 0.0 to 1.0
        gain - real (string i.e. "2.0")
        innerColor - ColourValue (string i.e. "0.1 0.9 1.0")
        lacunarity - real (string i.e. "2.0")
        noiseType - string either "fbm" or "ridged"
        octaves - unsigned int (string i.e. "100")
        outerColor - ColourValue (string i.e. "0.0 0.5 1.0")
        persistance - real (string i.e. "2.0") SAME AS GAIN
        powerAmount - real (string i.e. " 2.0") applied to noise to affect gradient
        scale - real (string, i.e. "1.0") initial position scale
        shelfAmount - real (string i.e. "0.1") should be in range 0.0 to 1.0
        sourceBlendFactor - string (i.e. one, dest_colour  etc.)
        @param params Layer params that will be specific to the derived class
        */
        void init(Ogre::NameValuePairList params);

        /** Set whether to display the high resolution implementation
        or the faster preview version
        @param displayHighRes Whether to display the high resolution or not
        */
        void setDisplayHighRes(bool displayHighRes);

    private:

        /** Utility function for building a regular sphere based on class params
        */
        void build(void);

        /** Utility function for building a cubed sphere based on class params
        @remarks this version is used for a less stretched uv map
        */
        void buildCubedSphere(void);

        /** Utility function for building a skybox based on class params
        @remarks copied from Ogre::SceneManager::_setSkyBox
        */
        void buildSkybox(void);

        /** Create the sphered cube material we'll need if not created already
        */
        void createCubedMaterial(void);

        /** Create the material we'll need if not created already
        */
        void createMaterial(void);

        /** this function pulled from hexidave's code
        point must be between -1,-1,-1 and 1,1,1
        @remarks Thank you HexiDave!
        @param p  The point to spherize
        */
	    static inline void spherizePoint(Vector3& p) 
        {
            // vertices are spaced better, but slower method
		    const double dX2 = p.x * p.x;
		    const double dY2 = p.y * p.y;
		    const double dZ2 = p.z * p.z;
            
		    const double dX2Half = dX2 * 0.5;
		    const double dY2Half = dY2 * 0.5;
		    const double dZ2Half = dZ2 * 0.5;
            
		    p.x *= Math::Sqrt(1-(dY2Half)-(dZ2Half)+((dY2 * dZ2) * 0.33333333333333333333333333333333));
		    p.y *= Math::Sqrt(1-(dZ2Half)-(dX2Half)+((dZ2 * dX2) * 0.33333333333333333333333333333333));
		    p.z *= Math::Sqrt(1-(dX2Half)-(dY2Half)+((dX2 * dY2) * 0.33333333333333333333333333333333));
	    };

        /** Utility function for updating the cubed texture material.
        */
        void updateCubedMaterialParams(void);

        /** Utility function for updating the cubed texture material.
        */
        void updateCubedMaterialParamsOld(void);

        /** Utility function for updating material fragment program 
        parameters.
        @param mat The material to update
        */
        void updateMaterialParams(MaterialPtr mat);

        /** Utility function for updating saved params list
        @param params The list of params
        */
        void updateParams(NameValuePairList params);

        // true if this layer is built
        bool mBuilt;

        // destination blend factor
        SceneBlendFactor mDestBlendFactor;

        // dither amount for noise
        Real mDitherAmount;

        // noise gain for fbm noise and ridged
        Real mGain;

        // flag for using gpu or cpu generated noise
        bool mGPU;

        // gradient texture
        TexturePtr mGradTexture;

        // inner color
        ColourValue mInnerColor;

        // lacunarity for noise
        Real mLacunarity;

        // material for this noise layer
        MaterialPtr mMaterial;

        // material name to copy
        String mMaterialName;

        // noise type - either "fbm" or "ridged"
        String mNoiseType;

        // size of the preview texture
        unsigned int mPreviewTextureSize;

        // size of the real texture that gets saved
        unsigned int mTextureSize;

        // offset for ridged fbm noise
        Real mOffset;

        // outer color
        ColourValue mOuterColor;

        // num octaves for noise
        unsigned int mOctaves;

        // Permutation table texture
        TexturePtr mPermTexture;

        // Power adjustment (gamma like function) for noise tweaking
        Real mPowerAmount;

        // noise scale
        Real mScale;

        // noise shelf amount
        Real mShelfAmount;

        // source blend factor
        SceneBlendFactor mSourceBlendFactor;
    };
}
#endif