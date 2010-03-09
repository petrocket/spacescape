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
#ifndef __SPACESCAPELAYER_H__
#define __SPACESCAPELAYER_H__

#include "SpacescapePrerequisites.h"
#include "OgreManualObject.h"
#include "OgreMovableObject.h"
#include "OgreCommon.h"
#include "SpacescapePlugin.h"
#include "SpacescapeNoiseMaterial.h"

namespace Ogre
{
    /** The SpacescapeLayer class defines a layer of a space background.
    Subclasses of this layer will draw different types of layers, whether
    they are point stars or 3d objects or billboards, etc.
    */
    class _SpacescapePluginExport SpacescapeLayer : public ManualObject
    {
    public:
        /** Constructor
        */
        SpacescapeLayer(const String& name, SpacescapePlugin* plugin);

        /** Destructor
        */
        ~SpacescapeLayer(void);

        /** Get the display high resolution flag value
        @return the high resolution flag value
        */
        bool getDisplayHighRes(void) { return mDisplayHighRes; }

        /** Get the layer id
        @return The layer id
        */
        unsigned int getLayerID(void) { return mLayerID; }

        /** Get the layer type - string version
        @return the layer type
        */
        virtual String getLayerTypeName(void) = 0;

        /** Get the layer type
        @return the layer type
        */
        virtual int getLayerType(void) = 0;

        /** Initialize this layer based on the given params
        @param params Layer params that will be specific to the derived class
        */
        virtual void init(NameValuePairList params) = 0;

        /** Set whether to display the high resolution implementation
        or the faster preview version
        @param displayHighRes Whether to display the high resolution or not
        */
        virtual void setDisplayHighRes(bool displayHighRes) { mDisplayHighRes = displayHighRes; }

        /** Set the layer id
        @param id The new layer id
        */
        virtual void setLayerID(unsigned int id) { mLayerID = id; }

        /** Get the param list used to create this layer
        @return The param list
        */
        NameValuePairList getParams() { return mParams; }

        /** This method allows subclasses to use object types that
        do not derive from ManualObject
        @return this object instance
        */
        virtual MovableObject* getMovableObject() { return this; }

    protected:
        /** Utility function to add a sphere section with the given material name
        and num segments
        @remarks Thank you http://www.ogre3d.org/wiki/index.php/ManualSphereMeshes
        @param material Material name
        @param numSegments number of sphere segments and rings
        */
        void buildSphere(const String& material, unsigned int segments = 16, ManualObject* manualObj = NULL);

       /** Utility noise function for fbm perlin noise
        @param v The 3d position
        @param octaves Number of octaves
        @param gain Noise gain at each level - same as persistance
        @param lacunarity Applied at each level
        @return The noise value
        */
        Real fbmNoise(Vector3 v, unsigned int octaves = 1, Real gain = 0.5, 
            Real lacunarity = 2.0);

        /** Utility function to convert a blend mode string to int
        @param param blend mode string like "one" or "dest_colour"
        */
        SceneBlendFactor getBlendMode(const String& param);

       /** Utility function to convert a blend mode string to int
        @param mode - blend mode 
        */
        String getBlendMode(SceneBlendFactor mode);

        /*
         * Helper functions to compute gradients-dot-residualvectors (1D to 4D)
         * Note that these generate gradients of more than unit length. To make
         * a close match with the value range of classic Perlin noise, the final
         * noise values need to be rescaled to fit nicely within [-1,1].
         */
        double grad( int hash, double x, double y , double z );

        /** Utiltiy function for initializing the noise permutations table
        @remarks This should be called any time mSeed is changed
        @param seed The seed to use for seeding the random number generator
        */
        void initNoise(unsigned int seed);

        /** Perlin improved noise (3d)
        @param x
        @param y
        @param z
        @return the noise value
        */
        double perlinNoise(double x, double y, double z);

        /** Render noise to 3d texture
        @param texture the texture to render to
        @param seed The seed for the random noise
        @param innerColor Noise inner color
        @param outerColor Noise outer color
        @param octaves Number of octaves
        @param lacunarity Lacunarity
        @param gain Applied to each octave
        @param power Power function to apply to final noise
        @param threshold Lower shelf/threshold
        @param dither Amount to dither the noise
        @param scale Initial scale amount applied to unit sphere noise coords
        @param offset Used for ridged noise
        */
        void renderNoiseToTexture( TexturePtr& texture, unsigned int seed, 
            const String& noiseType, ColourValue innerColor, 
            ColourValue outerColor, unsigned int octaves, Real lacunarity, 
            Real gain, Real power, Real threshold, Real dither, Real scale, Real offset);

        /** Ridge function for Ridged FBM noise
        @param noiseVal
        @param offset
        @return ridged value
        */
        Real ridge(Real noiseVal, Real offset);

        /** Utility noise function for ridged fbm perlin noise
        @param v The 3d position
        @param octaves Number of octaves
        @param gain Noise gain at each level - same as persistance
        @param lacunarity Applied at each level
        @param offset
        @return The noise value
        */
        Real ridgedFbmNoise(Vector3 v, unsigned int octaves = 1,
            Real gain = 0.5, Real lacunarity = 2.0, Real offset = 1.0);

        /** Rotate a point on a cube so that it is in the right position
        for a particular cube face.  The initial point should be on the top face
        @param p The 3d point on a cube
        @param face The face to move the point to
        */
        static inline void rotatePoint(Vector3& p, const int face)
        {
            Vector3 vTmp = p;

            if(face == 0) {
                // right
                p.x = vTmp.y;
                p.y = -vTmp.z;
                p.z = -vTmp.x;
            }
            else if(face == 1) {
                // left
                p.x = -vTmp.y;
                p.y = -vTmp.z;
                p.z = vTmp.x;
            }
            else if(face == 2) {
                // top - do nothing
            }
            else if(face == 3) {
                // bottom
                p.y = -vTmp.y;
                p.z = -vTmp.z;
            }
            else if(face == 4) {
                // front
                p.y = -vTmp.z;
                p.z = vTmp.y;
            }
            else if(face ==  5) {
                // back
                p.x = -vTmp.x;
                p.y = -vTmp.z;
                p.z = -vTmp.y;
            }
        }

        /** Perlin simplex noise (3d)
        @param x
        @param y
        @param z
        @return the noise value
        */
        double simplexNoise(double x, double y, double z);

        /** Utility function for updating saved params list
        @param params The list of params
        */
        virtual void updateParams(NameValuePairList params);

        // display high res version
        bool mDisplayHighRes;

        // gradients for noise
        Vector3 mGradients[512];

        // unique layer index - used for layer/draw order 0 is furthest away
        unsigned int mLayerID;

        // noise material
        SpacescapeNoiseMaterial* mNoiseMaterial;

        // noise manual object
        ManualObject* mRTTManualObject;

        // parameters
        NameValuePairList mParams;

        // perlin noise permutations
        unsigned char mPermutations[512];

        // plugin owner
        SpacescapePlugin* mPlugin;

        // random seed
        unsigned int mSeed;

        // layer title/name
        String mTitle;

        // unique id used for material and texture names
        unsigned int mUniqueID;
    };
}
#endif