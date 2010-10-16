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
#include "SpacescapeLayer.h"
#include "OgreMaterialManager.h"
#include "OgreMaterial.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreTextureManager.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreBlendMode.h"
#include "OgreSceneNode.h"

namespace Ogre
{
    static char grad3[16][3] = {
        {1,1,0},    {-1,1,0},    {1,-1,0},    {-1,-1,0},
        {1,0,1},    {-1,0,1},    {1,0,-1},    {-1,0,-1},
        {0,1,1},    {0,-1,1},    {0,1,-1},    {0,-1,-1},
        {1,1,0},    {0,-1,1},    {-1,1,0},    {0,-1,-1}
    };

    /* Constructor
    */
    SpacescapeLayer::SpacescapeLayer(const String& name,SpacescapePlugin* plugin) :
        ManualObject(name),
        mDisplayHighRes(false),
        mRTTManualObject(0),
        mPlugin(plugin),
        mSeed(0)
    {
        setCastShadows(false);
        initNoise(mSeed);
        mParams.clear();
        mUniqueID = plugin->getUniqueId();

        // create our noise material
        mNoiseMaterial = OGRE_NEW_T(SpacescapeNoiseMaterial,MEMCATEGORY_GENERAL);
    }

    /* Destructor
    */
    SpacescapeLayer::~SpacescapeLayer(void)
    {
        if(mNoiseMaterial) {
            OGRE_DELETE_T(mNoiseMaterial, SpacescapeNoiseMaterial, MEMCATEGORY_GENERAL);
            mNoiseMaterial = NULL;
        }

        if(mRTTManualObject) {
            OGRE_DELETE mRTTManualObject;
        }
    }

    /** Utility function to add a sphere section with the given material name
    and num segments
    @remarks Thank you http://www.ogre3d.org/wiki/index.php/ManualSphereMeshes
    @param material Material name
    @param numSegments number of sphere segments and rings
    */
    void SpacescapeLayer::buildSphere(const String& material, unsigned int segments, ManualObject* manualObj)
    {
        Real radius = 1.0;

        if(!manualObj) {
            manualObj = this;
        }

        manualObj->begin(material, RenderOperation::OT_TRIANGLE_LIST);

        unsigned int rings = segments;

        float fDeltaRingAngle = (Math::PI / rings);
        float fDeltaSegAngle = (2 * Math::PI / segments);
        unsigned short wVerticeIndex = 0 ;

        // Generate the group of rings for the sphere
        for(unsigned int ring = 0; ring <= rings; ring++ ) {
            float r0 = radius * sinf (ring * fDeltaRingAngle);
            float y0 = radius * cosf (ring * fDeltaRingAngle);

            // Generate the group of segments for the current ring
            for(unsigned int seg = 0; seg <= segments; seg++) {
                float x0 = r0 * sinf(seg * fDeltaSegAngle);
                float z0 = r0 * cosf(seg * fDeltaSegAngle);

                // Add one vertex to the strip which makes up the sphere
                manualObj->position(x0,y0,z0);

                if (ring != rings) {
                    // each vertex (except the last) has six indicies pointing to it
                    manualObj->index(wVerticeIndex + segments + 1);
                    manualObj->index(wVerticeIndex);               
                    manualObj->index(wVerticeIndex + segments);
                    manualObj->index(wVerticeIndex + segments + 1);
                    manualObj->index(wVerticeIndex + 1);
                    manualObj->index(wVerticeIndex);
                    wVerticeIndex ++;
                }
            }; // end for seg
        } // end for ring

        manualObj->end();
    }

    /** Utility noise function for fbm perlin noise
    @param v The 3d position
    @param octaves Number of octaves
    @param gain Noise gain at each level - same as persistance
    @param lacunarity Applied at each level
    @return The noise value
    */
    Real SpacescapeLayer::fbmNoise(Vector3 v, unsigned int octaves, Real gain, Real lacunarity)
    {
        Real noiseSum = 0.0;
        Real amplitude = 1.0;
        Real amplitudeSum = 0.0;
        
        // make some fbm noise
        for( unsigned int i = 0; i < octaves; i++) {
            noiseSum += perlinNoise(v.x, v.y, v.z) * amplitude;
            amplitudeSum += amplitude;
            amplitude *= gain;
            v *= lacunarity;
        }
        
        // get noiseSum in range -1..1    
        return noiseSum / amplitudeSum;
    }

    /** Utility function to convert a blend mode string to int
    @param param blend mode string like "one" or "dest_colour"
    */
    SceneBlendFactor SpacescapeLayer::getBlendMode(const String& param)
    {
        if (param == "one")
            return SBF_ONE;
        else if (param == "zero")
            return SBF_ZERO;
        else if (param == "dest_colour")
            return SBF_DEST_COLOUR;
        else if (param == "src_colour")
            return SBF_SOURCE_COLOUR;
        else if (param == "one_minus_dest_colour")
            return SBF_ONE_MINUS_DEST_COLOUR;
        else if (param == "one_minus_src_colour")
            return SBF_ONE_MINUS_SOURCE_COLOUR;
        else if (param == "dest_alpha")
            return SBF_DEST_ALPHA;
        else if (param == "src_alpha")
            return SBF_SOURCE_ALPHA;
        else if (param == "one_minus_dest_alpha")
            return SBF_ONE_MINUS_DEST_ALPHA;
        else if (param == "one_minus_src_alpha")
            return SBF_ONE_MINUS_SOURCE_ALPHA;
        else
            return SBF_ONE;
    }

   /** Utility function to convert a blend mode string to int
    @param mode - blend mode 
    */
    String SpacescapeLayer::getBlendMode(SceneBlendFactor mode)
    {
        if (mode == SBF_ONE)
            return "one";
        else if (mode == SBF_ZERO)
            return "zero";
        else if (mode == SBF_DEST_COLOUR)
            return "dest_colour";
        else if (mode == SBF_SOURCE_COLOUR)
            return "src_colour";
        else if (mode == SBF_ONE_MINUS_DEST_COLOUR)
            return "one_minus_dest_colour";
        else if (mode == SBF_ONE_MINUS_SOURCE_COLOUR)
            return "one_minus_src_colour";
        else if (mode == SBF_DEST_ALPHA)
            return "dest_alpha";
        else if (mode == SBF_SOURCE_ALPHA)
            return "src_alpha";
        else if (mode == SBF_ONE_MINUS_DEST_ALPHA)
            return "one_minus_dest_alpha";
        else if (mode == SBF_ONE_MINUS_SOURCE_ALPHA)
            return "one_minus_src_alpha";
        else
            return "one";
    }

    /*
     * Helper functions to compute gradients-dot-residualvectors (1D to 4D)
     * Note that these generate gradients of more than unit length. To make
     * a close match with the value range of classic Perlin noise, the final
     * noise values need to be rescaled to fit nicely within [-1,1].
     */
    double SpacescapeLayer::grad( int hash, double x, double y , double z ) 
    {
        int h = hash & 15;     // Convert low 4 bits of hash code into 12 simple
        double u = h<8 ? x : y; // gradient directions, and compute dot product.
        double v = h<4 ? y : h==12||h==14 ? x : z; // Fix repeats at h = 12 to 15
        return ((h&1)? -u : u) + ((h&2)? -v : v);
    }

    /** Utiltiy function for initializing the perlin noise
    */
    void SpacescapeLayer::initNoise(unsigned int seed)
    {
        // update the permutation table
        // thank you http://britonia-game.com/?p=60
        for(int i = 0; i < 256; i++) {
            mPermutations[i] = i;
        }

        // seed the random number generator
        srand(seed);

        // randomize the permutation table
        for(int i = 0; i < 256; i++) {
            // for each value swap with a random slot in the array 
            uchar swapIndex = rand() % 256;

            int oldVal = mPermutations[i];
            mPermutations[i] = mPermutations[swapIndex];
            mPermutations[swapIndex] = oldVal;
        }

        for(int i = 0; i < 256; i++) {
            mPermutations[i + 256] = mPermutations[i];
            int h = mPermutations[i] & 15;
            mGradients[i] = Vector3(grad3[h][0],grad3[h][1],grad3[h][2]).normalisedCopy();
            mGradients[i + 256] = mGradients[i];
        }
    }
    
    #define lerp(t,a,b) ( (a)+(t)*((b)-(a)) )
    #define fade(t) ( (t)*(t)*(t)*(t)*((t)*((t)*6-15)+10) )
    #define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : (((int)x)-1) )

    /** Perlin improved noise (3d)
    @param x
    @param y
    @param z
    @return the noise value
    */
    double SpacescapeLayer::perlinNoise(double x, double y, double z)
    {
        int   X = FASTFLOOR(x) & 255,              /* FIND UNIT CUBE THAT */
              Y = FASTFLOOR(y) & 255,              /* CONTAINS POINT.     */
              Z = FASTFLOOR(z) & 255;
        x -= FASTFLOOR(x);                         /* FIND RELATIVE X,Y,Z */
        y -= FASTFLOOR(y);                         /* OF POINT IN CUBE.   */
        z -= FASTFLOOR(z);
        double  u = fade(x),                       /* COMPUTE FADE CURVES */
                v = fade(y),                       /* FOR EACH OF X,Y,Z.  */
                w = fade(z);
        int  A = mPermutations[X]+Y, 
             AA = mPermutations[A]+Z, 
             AB = mPermutations[A+1]+Z, /* HASH COORDINATES OF */
             B = mPermutations[X+1]+Y, 
             BA = mPermutations[B]+Z, 
             BB = mPermutations[B+1]+Z; /* THE 8 CUBE CORNERS, */
          
        return lerp(w,lerp(v,lerp(u, grad(mPermutations[AA], x, y, z),   /* AND ADD */
                             grad(mPermutations[BA], x-1, y, z)),        /* BLENDED */
                     lerp(u, grad(mPermutations[AB], x, y-1, z),         /* RESULTS */
                             grad(mPermutations[BB], x-1, y-1, z))),     /* FROM  8 */
                     lerp(v, lerp(u, grad(mPermutations[AA+1], x, y, z-1 ),/* CORNERS */
                             grad(mPermutations[BA+1], x-1, y, z-1)),      /* OF CUBE */
                     lerp(u, grad(mPermutations[AB+1], x, y-1, z-1),
                             grad(mPermutations[BB+1], x-1, y-1, z-1))));
    }

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
    void SpacescapeLayer::renderNoiseToTexture(TexturePtr& texture, 
        unsigned int seed, 
        const String& noiseType,
        ColourValue innerColor,
        ColourValue outerColor,
        unsigned int octaves, 
        Real lacunarity, Real gain, 
        Real power, Real threshold,
        Real dither,
        Real scale,
        Real offset)
    {
        // update the rtt material
        MaterialPtr material = mNoiseMaterial->getMaterial();

        // only accept two types of noise for now
        String validNoiseType = noiseType != "ridged" ? "fbm" : "ridged";

        // shuffle the techniques around because we can't control 
        // the selected technique without overriding the renderable
        // class (ManualObjectSection)
        while(material->getTechnique(0)->getName() != validNoiseType) {
            // move the technique at index 0 to back of the line
            Technique* t = material->createTechnique();
            *t = *(material->getTechnique(0));
            material->removeTechnique(0);
        }

        // set blending to not draw anything but this sphere (opaque)
        Pass* pass = material->getTechnique(0)->getPass(0);
        pass->setSceneBlending(SBF_ONE,SBF_ZERO);

        // set properties for our material
        GpuProgramParametersSharedPtr params = pass->getFragmentProgramParameters();

        params->setNamedConstant( "ditherAmt",  dither );
        params->setNamedConstant( "gain",       gain );
        params->setNamedConstant( "innerColor", innerColor );
        params->setNamedConstant( "lacunarity", lacunarity );
        params->setNamedConstant( "octaves",    (int)octaves );
        params->setNamedConstant( "outerColor", outerColor );
        params->setNamedConstant( "powerAmt",   power );
        params->setNamedConstant( "shelfAmt",   threshold );
        params->setNamedConstant( "noiseScale", scale );

        if(validNoiseType == "ridged") {
            params->setNamedConstant( "offset",   offset );
        }

        // initialize permutations table
        initNoise(seed);

        // now write the perm and gradient values to a texture
        uchar* permTexture = OGRE_ALLOC_T( uchar, 256 * 256 * 4, MEMCATEGORY_GENERAL);

        // we add the Z part in the shader
        for(int Y = 0; Y<256; Y++) {
            for(int X = 0; X<256; X++) {
                int offset = (Y*256+X)*4;

                uchar A = mPermutations[X]; // A = mPermutations[X]+Y (add y later below)
                permTexture[offset + 0]   = mPermutations[A + Y]; // AA = mPermutations[A]+Z (add z in shader)
                permTexture[offset+ 1] = mPermutations[A+Y+1]; // AB = mPermutations[A+1]+Z
                uchar B = mPermutations[X+1]; // we add the y later
                permTexture[offset+2] = mPermutations[B + Y]; // BA = mPermutations[B]+Z (add z in shader)
                permTexture[offset+3] = mPermutations[B+Y+1]; // BB = mPermutations[B+1]+Z (add z in shader)             
            }
        }

        TexturePtr permTexturePtr = pass->getTextureUnitState(0)->_getTexturePtr();
        HardwarePixelBufferSharedPtr pb = permTexturePtr->getBuffer();
        if(!pb->isLocked()) {
            // blit from memory to the texture surface
            pb->blitFromMemory(PixelBox(256,256,1,PF_BYTE_RGBA,permTexture));
        }

        pass->getTextureUnitState(0)->setTextureName(permTexturePtr->getName());

        // free memory
        OGRE_DELETE_T(permTexture,uchar,MEMCATEGORY_GENERAL);

        // create the gradient texture
        uchar* gradTexture = OGRE_ALLOC_T( uchar, 256 * 3, MEMCATEGORY_GENERAL);
        for(int i = 0; i < 256; i++) {
            int offset = i * 3;
            int index = mPermutations[i] & 15;
            Vector3 v = Vector3(grad3[index][0],grad3[index][1],grad3[index][2]);
            v.normalise();
            v *= 0.5;
            v += 0.5;

            gradTexture[offset + 0] = floor(v.x * 255.0);
            gradTexture[offset + 1] = floor(v.y * 255.0);
            gradTexture[offset + 2] = floor(v.z * 255.0);
        }

        TexturePtr gradTexturePtr = pass->getTextureUnitState(1)->_getTexturePtr();
        pb = gradTexturePtr->getBuffer();
        if(!pb->isLocked()) {
            // blit from memory to the texture surface
            pb->blitFromMemory(PixelBox(256,1,1,PF_BYTE_RGB,gradTexture));
        }

        pass->getTextureUnitState(1)->setTextureName(gradTexturePtr->getName());

        material->load();

        // free memory
        OGRE_FREE(gradTexture,MEMCATEGORY_GENERAL);

        // create the rtt sphere section
        if(!mRTTManualObject) {
            mRTTManualObject = OGRE_NEW ManualObject("SpacescapeRttNoise" + StringConverter::toString(mUniqueID));
            buildSphere(material->getName(),16,mRTTManualObject);
            
            // set to draw last (overlays will be ignored)
            mRTTManualObject->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
        }
        
        // hide other layers
        mPlugin->getSceneNode()->setVisible(false,false);

        // attach this object to a node
        SceneNode* node = mPlugin->getSceneNode()->getParentSceneNode()->createChildSceneNode("SpacescapeRttNoise");
        node->attachObject(mRTTManualObject);

        // rtt to the given texture
        mPlugin->_rtt(texture,(uint)texture->getNumMipmaps());

        // remove the scene node we just created
        mPlugin->getSceneNode()->getParentSceneNode()->removeAndDestroyChild("SpacescapeRttNoise");

        // show other layers again
        mPlugin->getSceneNode()->setVisible(true,false);
    }

    /** Ridge function for Ridged FBM noise
    @param noiseVal
    @param offset
    @return ridged value
    */
    Real SpacescapeLayer::ridge(Real noiseVal, Real offset)
    {
        float newVal = offset - abs(noiseVal);
        return newVal * newVal;
    }

    /** Utility noise function for ridged fbm perlin noise
    @param v The 3d position
    @param octaves Number of octaves
    @param gain Noise gain at each level - same as persistance
    @param lacunarity Applied at each level
    @param offset
    @return The noise value
    */
    Real SpacescapeLayer::ridgedFbmNoise(Vector3 v, unsigned int octaves, Real gain, Real lacunarity, Real offset)
    {
        float noiseSum = 0.0;
        float amplitude = 1.0;
        float amplitudeSum = 0.0;
        float prev = 1.0;
        float n;
        
        // make some ridged fbm noise
        for( unsigned int i = 0; i < octaves; i++) {
            n = ridge(perlinNoise(v.x, v.y, v.z), offset);
            noiseSum += n * amplitude * prev;
            prev = n;
            amplitudeSum += amplitude;
            amplitude *= gain;
            v *= lacunarity;
        }
        
        // get noiseSum in range -1..1    
        return noiseSum / amplitudeSum;  
    }

    /** 3D simplex noise
    // SimplexNoise1234
    // Copyright © 2003-2005, Stefan Gustavson
    //
    // Contact: stegu@itn.liu.se
    //
    // This library is free software; you can redistribute it and/or
    // modify it under the terms of the GNU General Public
    // License as published by the Free Software Foundation; either
    // version 2 of the License, or (at your option) any later version.
    //
    // This library is distributed in the hope that it will be useful,
    // but WITHOUT ANY WARRANTY; without even the implied warranty of
    // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    // General Public License for more details.
    //
    // You should have received a copy of the GNU General Public
    // License along with this library; if not, write to the Free Software
    // Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    @param x
    @param y
    @param z
    @return the noise value
    */
    double SpacescapeLayer::simplexNoise(double x, double y, double z) 
    {
        // Simple skewing factors for the 3D case
        #define F3 0.333333333
        #define G3 0.166666667

        double n0, n1, n2, n3; // Noise contributions from the four corners

        // Skew the input space to determine which simplex cell we're in
        double s = (x+y+z)*F3; // Very nice and simple skew factor for 3D
        double xs = x+s;
        double ys = y+s;
        double zs = z+s;
        int i = FASTFLOOR(xs);
        int j = FASTFLOOR(ys);
        int k = FASTFLOOR(zs);

        double t = (double)(i+j+k)*G3; 
        double X0 = i-t; // Unskew the cell origin back to (x,y,z) space
        double Y0 = j-t;
        double Z0 = k-t;
        double x0 = x-X0; // The x,y,z distances from the cell origin
        double y0 = y-Y0;
        double z0 = z-Z0;

        // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
        // Determine which simplex we are in.
        int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
        int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

        /* This code would benefit from a backport from the GLSL version! */
        if(x0>=y0) {
            if(y0>=z0)
            { i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; } // X Y Z order
            else if(x0>=z0) { i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; } // X Z Y order
            else { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; } // Z X Y order
        }
        else { // x0<y0
            if(y0<z0) { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; } // Z Y X order
            else if(x0<z0) { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; } // Y Z X order
            else { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; } // Y X Z order
        }

        // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
        // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
        // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
        // c = 1/6.

        double x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
        double y1 = y0 - j1 + G3;
        double z1 = z0 - k1 + G3;
        double x2 = x0 - i2 + 2.0f*G3; // Offsets for third corner in (x,y,z) coords
        double y2 = y0 - j2 + 2.0f*G3;
        double z2 = z0 - k2 + 2.0f*G3;
        double x3 = x0 - 1.0f + 3.0f*G3; // Offsets for last corner in (x,y,z) coords
        double y3 = y0 - 1.0f + 3.0f*G3;
        double z3 = z0 - 1.0f + 3.0f*G3;

        // Wrap the integer indices at 256, to avoid indexing mPermutations[] out of bounds
        int ii = i % 256;
        int jj = j % 256;
        int kk = k % 256;

        // Calculate the contribution from the four corners
        double t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
        if(t0 < 0.0f) n0 = 0.0f;
        else {
            t0 *= t0;
            n0 = t0 * t0 * grad(mPermutations[ii+mPermutations[jj+mPermutations[kk]]], x0, y0, z0);
        }

        double t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
        if(t1 < 0.0f) n1 = 0.0f;
        else {
            t1 *= t1;
            n1 = t1 * t1 * grad(mPermutations[ii+i1+mPermutations[jj+j1+mPermutations[kk+k1]]], x1, y1, z1);
        }

        double t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
        if(t2 < 0.0f) n2 = 0.0f;
        else {
            t2 *= t2;
            n2 = t2 * t2 * grad(mPermutations[ii+i2+mPermutations[jj+j2+mPermutations[kk+k2]]], x2, y2, z2);
        }

        double t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
        if(t3<0.0f) n3 = 0.0f;
        else {
            t3 *= t3;
            n3 = t3 * t3 * grad(mPermutations[ii+1+mPermutations[jj+1+mPermutations[kk+1]]], x3, y3, z3);
        }

        // Add contributions from each corner to get the final noise value.
        // The result is scaled to stay just inside [-1,1]
        return 32.0 * (n0 + n1 + n2 + n3); // TODO: The scale factor is preliminary!
    }

    /** Utility function for updating saved params list
    @param params The list of params
    */
    void SpacescapeLayer::updateParams(NameValuePairList params)
    {
        NameValuePairList::iterator ii;
        for(ii = params.begin(); ii != params.end(); ii++) {
            mParams[ii->first] = ii->second;
        }

        // update shared params
        mParams["seed"] = StringConverter::toString(mSeed);
        mParams["name"] = getName();
        mParams["type"] = getLayerTypeName();
    }
}
