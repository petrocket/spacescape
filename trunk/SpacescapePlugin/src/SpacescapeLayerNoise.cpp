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
#include "SpacescapeLayerNoise.h"
#include "OgreMaterialManager.h"
#include "OgreMaterial.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreTextureManager.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreBlendMode.h"
#include "OgreOverlayManager.h"
#include "OgreOverlayContainer.h"

namespace Ogre
{
       /* Use of IUNI in an expression will produce a 32-bit unsigned
        random integer, while UNI will produce a random real in [0,1).
        The static variables z and w can be reassigned to i1 and i2
        by setseed(i1,i2);
       You may replace the two constants 36969 and 18000 by any
       pair of distinct constants from this list:
       18000 18030 18273 18513 18879 19074 19098 19164 19215 19584
       19599 19950 20088 20508 20544 20664 20814 20970 21153 21243
       21423 21723 21954 22125 22188 22293 22860 22938 22965 22974
       23109 23124 23163 23208 23508 23520 23553 23658 23865 24114
       24219 24660 24699 24864 24948 25023 25308 25443 26004 26088
       26154 26550 26679 26838 27183 27258 27753 27795 27810 27834
       27960 28320 28380 28689 28710 28794 28854 28959 28980 29013
       29379 29889 30135 30345 30459 30714 30903 30963 31059 31083
       (or any other 16-bit constants k for which both k*2^16-1
       and k*2^15-1 are prime)*/
        /*
        // http://www.math.uni-bielefeld.de/~sillke/ALGORITHMS/random/marsaglia-c
           #define znew  ((z=36969*(z&65535)+(z>>16))<<16)
           #define wnew  ((w=18000*(w&65535)+(w>>16))&65535)
           #define IUNI  (znew+wnew)
           #define UNI   (znew+wnew)*2.328306e-10
           static unsigned long z=362436069, w=521288629;
           void setseed(unsigned long i1,unsigned long i2){z=i1; w=i2;}
       */

    /* These are Ken Perlin's proposed gradients for 3D noise. I kept them for
       better consistency with the reference implementation, but there is really
       no need to pad this to 16 gradients for this particular implementation.
       If only the "proper" first 12 gradients are used, they can be extracted
       from the grad4[][] array: grad3[i][j] == grad4[i*2][j], 0<=i<=11, j=0,1,2
    */
    /*
    int grad3[16][3] = {{0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},
                       {1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1},
                       {1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0}, // 12 cube edges
                       {1,0,-1},{-1,0,-1},{0,-1,1},{0,1,1}}; // 4 more to make 16
*/
    static char grad3[16][3] = {
        {1,1,0},    {-1,1,0},    {1,-1,0},    {-1,-1,0},
        {1,0,1},    {-1,0,1},    {1,0,-1},    {-1,0,-1},
        {0,1,1},    {0,-1,1},    {0,1,-1},    {0,-1,-1},
        {1,1,0},    {0,-1,1},    {-1,1,0},    {0,-1,-1}
    };


    /** Constructor
    */
    SpacescapeLayerNoise::SpacescapeLayerNoise(const String& name, SpacescapePlugin* plugin) :
        SpacescapeLayer(name, plugin),
        mBuilt(false),
        mDestBlendFactor(SBF_ONE),
        mDitherAmount(0.03),
        mGain(0.5),
        mGPU(false),
        mInnerColor(1.0,1.0,1.0),
        mLacunarity(2.0),
        mMaterialName("NoiseMaterial"),
        mNoiseType("fbm"),
        mOuterColor(0.0,0.0,0.0),
        mOctaves(2),
        mOffset(1.0),
        mPowerAmount(1.0),
        mPreviewTextureSize(256),
        mTextureSize(1024),
        mScale(1.0),
        mShelfAmount(0.0),
        mSourceBlendFactor(SBF_ONE)
    {
        mSeed = 0;
        mMaterial.setNull();
        mPermTexture.setNull();
    }

    /** Destructor
    */
    SpacescapeLayerNoise::~SpacescapeLayerNoise(void)
    {
        clear();

        if(!mMaterial.isNull()) {
            MaterialManager::getSingleton().remove(mMaterial->getHandle());
        }
        if(!mGradTexture.isNull()) {
            TextureManager::getSingleton().remove(mGradTexture->getHandle());
        }
        if(!mPermTexture.isNull()) {
            TextureManager::getSingleton().remove(mPermTexture->getHandle());
        }
    }

    /** Utility function for building a regular sphere based on class params
    */
    void SpacescapeLayerNoise::build(void) 
    {
        // clear the old list
        clear();
        
        // create the material we'll need if not created already
        createMaterial();

        // build the sphere
        buildSphere(mMaterial->getName());

        // we're built!
        mBuilt = true;
    }

    /** Utility function for building a cubed sphere based on class params
    @remarks this version is used for a less stretched uv map
    */
    void SpacescapeLayerNoise::buildCubedSphere(void)
    {
        int segments = 16;

        // clear the old list
        clear();
        
        // create the material we'll need if not created already
        createCubedMaterial();

        begin(mMaterial->getName(), RenderOperation::OT_TRIANGLE_LIST);

        Real step = 2.0 / (Real)segments;
        for(int f = 0; f < 6; ++f) {
            for(int y = 0; y <= segments; ++y) {
                for(int x = 0; x <= segments; ++x) {

                    Vector3 p = Vector3(-1.0 + x * step, 1.0, -1.0 + y * step);

                    // rotate this point to the current face(f)
                    rotatePoint(p,f);

                    // spherize this cube point 
                    Vector3 spherized = p;
                    spherizePoint(spherized);
                    position(spherized);
                    
                    textureCoord(p);
                }
            }
        }

        // indexes
        for(int f = 0; f < 6; ++f) {
            int base = f * (segments+1) * (segments+1);
            for(int y = 0; y < segments; ++y) {
                int yBase = base + (segments+1)*y;
                for(int x = 0; x < segments; ++x) {
                    // indices
                    quad(
                        yBase + x,
                        yBase + x + 1,
                        yBase + (segments + 1) + x + 1,
                        yBase + (segments + 1) + x
                    );
                }
            }
        }

        end();

        mBuilt = true;
    }

    /** Utility function for building a skybox based on class params
    @remarks copied from Ogre::SceneManager::_setSkyBox
    */
    void SpacescapeLayerNoise::buildSkybox(void)
    {
        begin(mMaterial->getName());

        for (int i = 0; i < 6; ++i) {
			Vector3 middle, up, right;

			switch(i) {
			    case 0:
                    middle = Vector3::NEGATIVE_UNIT_Z;
				    up = Vector3::UNIT_Y;
				    right = Vector3::UNIT_X;
				    break;
			    case 1:
                    middle = Vector3::UNIT_Z;
				    up = Vector3::UNIT_Y;
				    right = Vector3::NEGATIVE_UNIT_X;
				    break;
			    case 2:
                    middle = Vector3::NEGATIVE_UNIT_X;
				    up = Vector3::UNIT_Y;
				    right = Vector3::NEGATIVE_UNIT_Z;
				    break;
			    case 3:
                    middle = Vector3::UNIT_X;
				    up = Vector3::UNIT_Y;
				    right = Vector3::UNIT_Z;
				    break;
			    case 4:
                    middle = Vector3::UNIT_Y;
				    up = Vector3::UNIT_Z;
				    right = Vector3::UNIT_X;
				    break;
			    case 5:
                    middle = Vector3::NEGATIVE_UNIT_Y;
				    up = Vector3::NEGATIVE_UNIT_Z;
				    right = Vector3::UNIT_X;
				    break;
			}
            
			// top left
			Vector3 pos;
			pos = middle + up - right;
			position(pos);
			textureCoord(pos.normalisedCopy() * Vector3(1,1,-1));

			// bottom left
			pos = middle - up - right;
			position(pos);
			textureCoord(pos.normalisedCopy() * Vector3(1,1,-1));

			// bottom right
			pos = middle - up + right;
			position(pos);
			textureCoord(pos.normalisedCopy() * Vector3(1,1,-1));

			// top right
			pos = middle + up + right;
			position(pos);
			textureCoord(pos.normalisedCopy() * Vector3(1,1,-1));

			uint16 base = i * 4;
			quad(base, base+1, base+2, base+3);
        }

        end();

        mBuilt = true;
    }


    /** Create the sphered cube material we'll need if not created already
    */
    void SpacescapeLayerNoise::createCubedMaterial(void)
    {
        if(mMaterial.isNull()) {
            // create a skybox 3d / cubic texture
            mMaterial = (MaterialPtr)MaterialManager::getSingletonPtr()->create(
                "SpacescapeNoiseMaterial" + StringConverter::toString(mUniqueID),
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
            );

            // turn off lighting and depth write/check
            mMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
            mMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
            mMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);

            // create the texture for the cube face
            TexturePtr t = TextureManager::getSingleton().createManual(
                "SpacescapeNoiseTexture" + StringConverter::toString(mUniqueID),
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                TEX_TYPE_CUBE_MAP,
                mPreviewTextureSize, mPreviewTextureSize,
                1,
                // not sure if we need mip maps on this cube face? 
                // enabling mipmaps causes image corruption on some ATI cards x1950 Pro
                0,//SpacescapePlugin::_log2(mPreviewTextureSize),
                mFBOPixelFormat,
                TU_RENDERTARGET
            );

            // assign the texture to the cube face
            TextureUnitState* tu = mMaterial->getTechnique(0)->getPass(0)->createTextureUnitState();
            tu->setCubicTextureName(t->getName(),true);
            tu->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
            tu->setTextureFiltering(TFO_TRILINEAR);
            mMaterial->load();
        }
    }

    /** Create the material we'll need if not created already
    */
    void SpacescapeLayerNoise::createMaterial(void)
    {
        if(mMaterial.isNull()) {
            // clone the noise material
            mMaterial = mNoiseMaterial->getMaterial()->clone("SpacescapeNoiseMaterial" + StringConverter::toString(mUniqueID));

            // create the permutation table texture
            mPermTexture = TextureManager::getSingleton().createManual(
                    "SpacescapeNoisePermTexture" + StringConverter::toString(mUniqueID),
                    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                    TEX_TYPE_2D,
                    256,
                    256,
                    0,
                    PF_BYTE_RGBA,
                    TU_STATIC_WRITE_ONLY
            );

            // create the gradient look up table
            mGradTexture = TextureManager::getSingleton().createManual(
                    "SpacescapeNoiseGradTexture" + StringConverter::toString(mUniqueID),
                    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                    TEX_TYPE_1D,
                    256,
                    1,
                    0,
                    PF_BYTE_RGB,
                    TU_STATIC_WRITE_ONLY
            );
        }
    }

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
    void SpacescapeLayerNoise::init(Ogre::NameValuePairList params)
    {
        bool shouldUpdate = false;

        NameValuePairList::iterator ii;
        for(ii = params.begin(); ii != params.end(); ii++) {
            if(ii->first == "destBlendFactor") {
                shouldUpdate |= mDestBlendFactor != getBlendMode(ii->second);
                mDestBlendFactor = getBlendMode(ii->second);
            }
            else if(ii->first == "ditherAmount") {
                shouldUpdate |= mDitherAmount != StringConverter::parseReal(ii->second);
                mDitherAmount = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "gain") {
                shouldUpdate |= mGain != StringConverter::parseReal(ii->second);
                mGain = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "gpu") {
                shouldUpdate |= mGPU != StringConverter::parseBool(ii->second);
                mGPU = StringConverter::parseBool(ii->second);
            }
            else if(ii->first == "innerColor") {
                shouldUpdate |= mInnerColor != StringConverter::parseColourValue(ii->second);
                mInnerColor = StringConverter::parseColourValue(ii->second);
            }
            else if(ii->first == "lacunarity") {
                shouldUpdate |= mLacunarity != StringConverter::parseReal(ii->second);
                mLacunarity = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "offset") {
                shouldUpdate |= mOffset != StringConverter::parseReal(ii->second);
                mOffset = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "octaves") {
                shouldUpdate |= mOctaves != StringConverter::parseInt(ii->second);
                mOctaves = StringConverter::parseInt(ii->second);
            }
            else if(ii->first == "outerColor") {
                shouldUpdate |= mOuterColor != StringConverter::parseColourValue(ii->second);
                mOuterColor = StringConverter::parseColourValue(ii->second);
            }
            else if(ii->first == "persistance") {
                shouldUpdate |= mGain != StringConverter::parseReal(ii->second);
                mGain = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "powerAmount") {
                shouldUpdate |= mPowerAmount != StringConverter::parseReal(ii->second);
                mPowerAmount = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "previewTextureSize") {
                shouldUpdate |= mPreviewTextureSize != StringConverter::parseUnsignedInt(ii->second);
                mPreviewTextureSize = StringConverter::parseUnsignedInt(ii->second);
            }
            else if(ii->first == "name") {
                mName = ii->second;
            }
            else if(ii->first == "noiseType") {
                // only two possible noise types for now
                shouldUpdate |= mNoiseType != ii->second;
                mNoiseType = ii->second == "ridged" ? "ridged" : "fbm";
            }
            else if(ii->first == "scale") {
                shouldUpdate |= mScale != StringConverter::parseReal(ii->second);
                mScale = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "seed") {
                shouldUpdate |= mSeed != StringConverter::parseInt(ii->second);
                mSeed = StringConverter::parseInt(ii->second);
            }
            else if(ii->first == "shelfAmount") {
                shouldUpdate |= mShelfAmount != StringConverter::parseReal(ii->second);
                mShelfAmount = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "sourceBlendFactor") {
                shouldUpdate |= mSourceBlendFactor != getBlendMode(ii->second);
                mSourceBlendFactor = getBlendMode(ii->second);
            }
        }

        // update our saved params
        updateParams(params);

        // now build based on these settings
        if(shouldUpdate || !mBuilt) {
            if(!mBuilt) {
                if(mGPU) {
                    build();
                }
                else {
                    createCubedMaterial();
                }
            }

            // update material fragment program parameters
            if(mGPU) {
                updateMaterialParams(mMaterial);
            }
            else {
                updateCubedMaterialParams();
            }
        }
    }

    /** Set whether to display the high resolution implementation
    or the faster preview version
    @param displayHighRes Whether to display the high resolution or not
    */
    void SpacescapeLayerNoise::setDisplayHighRes(bool displayHighRes)
    {
        // disabled for now - makes complex scenes crash on export
        return;

        if(mDisplayHighRes == displayHighRes) {
            return;
        }

        // remove current geometry
        clear();

        if(displayHighRes) {
            // copy the noise material
            MaterialPtr m = MaterialManager::getSingleton().getByName("SpacescapeNoiseMaterialHighRes"+StringConverter::toString(mUniqueID));
            if(m.isNull()) {
                m = mNoiseMaterial->getMaterial()->clone("SpacescapeNoiseMaterialHighRes"+StringConverter::toString(mUniqueID));
            }

            // update the material params
            updateMaterialParams(m);

            // display the sphere with this material
            buildSphere(m->getName(),16,this);
        }
        else {
            // free our high res version
            MaterialPtr m = MaterialManager::getSingleton().getByName("SpacescapeNoiseMaterialHighRes"+StringConverter::toString(mUniqueID));
            if(!m.isNull()) {
                // free the material
                MaterialManager::getSingleton().remove(m->getHandle());

                // free the perm and grad textures
                TextureManager::getSingleton().remove("SpacescapeNoisePermTexture" + StringConverter::toString(mUniqueID));
                TextureManager::getSingleton().remove("SpacescapeNoiseGradTexture" + StringConverter::toString(mUniqueID));
            }
            buildSkybox();
        }

        mDisplayHighRes = displayHighRes;
    }

    /** Utility function for updating the cubed texture material with GPU noise.
    */
    void SpacescapeLayerNoise::updateCubedMaterialParams(void)
    {
        TexturePtr t = mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->_getTexturePtr();
        if(t->getWidth() != mPreviewTextureSize) {
            // remove the old texture
			TextureManager::getSingleton().remove(t->getHandle());

            // create the texture for the cube face
            t = TextureManager::getSingleton().createManual(
                "SpacescapeNoiseTexture" + StringConverter::toString(mUniqueID),
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                TEX_TYPE_CUBE_MAP,
                mPreviewTextureSize, mPreviewTextureSize,
                1,
                0,// Creating mipmaps on a cubic RTT texture crashes some ATI cards so don't do it!
                mFBOPixelFormat,
                TU_RENDERTARGET
            );

            // assign the texture to the cube face
            mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setCubicTextureName(t->getName(),true);
            mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
            mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureFiltering(TFO_TRILINEAR);
            mMaterial->load();

        }

        // render the gpu noise to our cubic texture
        renderNoiseToTexture(
            t,
            mSeed,
            mNoiseType,
            mInnerColor,
            mOuterColor,
            mOctaves,
            mLacunarity,
            mGain,
            mPowerAmount,
            mShelfAmount,
            mDitherAmount,
            mScale,
            mOffset
        );

        // build the skybox with our cubic texture
        if(!mBuilt) {
            buildSkybox();
        }

        // set blending
        mMaterial->getTechnique(0)->getPass(0)->setSceneBlending(mSourceBlendFactor,mDestBlendFactor);
    }

    /** Old Utility function for updating the cubed texture material with CPU - doesn't match GPU
    precisely and has seams at cube edges.
    */
    void SpacescapeLayerNoise::updateCubedMaterialParamsOld(void)
    {
        // initialize permutations table
        initNoise(mSeed);

        // set blending
        mMaterial->getTechnique(0)->getPass(0)->setSceneBlending(mSourceBlendFactor,mDestBlendFactor);

        TexturePtr t = mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->_getTexturePtr();

        // update the six faces of this texture
        uchar* buf[6];
        for(int f = 0; f < 6; ++f) {
            buf[f] = OGRE_ALLOC_T( uchar, mPreviewTextureSize * mPreviewTextureSize * 4, MEMCATEGORY_GENERAL);
        }

        Real scale = 2.0 / (Real)mPreviewTextureSize;
        float n;
        float noiseScale = 1.0/(1.0-mShelfAmount);
        float powerFactor = 1.0/mPowerAmount;
        bool ridged = mNoiseType == "ridged";

        for(unsigned int f = 0; f < 6; ++f) {
            for(unsigned int y = 0; y < mPreviewTextureSize; ++y) {
                int yOffset = y * mPreviewTextureSize * 4;
                for(unsigned int x = 0; x < mPreviewTextureSize; ++x) {
                    // get the 3d point on the top surface of the cube
                    Vector3 p = Vector3(-1.0 + x*scale, 1.0, -1.0 + y*scale);
                    
                    // rotate to the cube face we want
                    rotatePoint(p,f);

                    // spherize the point from cube space to sphere space
                    spherizePoint(p);

                    // scale down to avoid noise irregularities at edges
                    p.normalise();

                    // get noise at this location
                    if(ridged) {
                        n = ridgedFbmNoise(p * mScale, mOctaves,mGain,mLacunarity,mOffset);
                    }
                    else {
                        n = fbmNoise(p * mScale, mOctaves,mGain,mLacunarity);
                    }

                    // add a dithering noise if requested
                    if(mDitherAmount > 0.0) {
                        n += rand() / ((double) RAND_MAX) * mDitherAmount;
                    }

                    // scale noise value to between 0..1
                    n += 1.0;
                    n *= 0.5;

                    // apply shelf
                    if(mShelfAmount > 0.0) {
                        n = std::max<float>(0.0,n - mShelfAmount);

                        // scale remainder back to between 0..1
                        n *= noiseScale;
                    }

                    // apply optional power function
                    if(mPowerAmount) {
                        n = pow(n,powerFactor);
                    }

                    int offset = yOffset + (x * 4);

                    // clamp n to between 0 .. 1
                    n = std::min<float>(1.0,n);
                    n = std::max<float>(0.0,n);

                    ColourValue c = mOuterColor + (n * (mInnerColor - mOuterColor));
                    buf[f][offset] = c.r * 255.0;
                    buf[f][offset + 1] = c.g * 255.0;
                    buf[f][offset + 2] = c.b * 255.0;
                    buf[f][offset + 3] = 1.0;//n * 255.0;
                }
            }
        }
        // write to the surface
        for(int f = 0; f < 6; ++f) {
            HardwarePixelBufferSharedPtr pb = t->getBuffer(f);
            if(!pb->isLocked()) {
                // blit from memory to the texture surface
                pb->blitFromMemory(PixelBox(mPreviewTextureSize,mPreviewTextureSize,1,PF_BYTE_RGBA,buf[f]));
            }

            OGRE_FREE(buf[f], MEMCATEGORY_GENERAL);
        }
    }

    /** Utility function for updating material fragment program 
    parameters.
    @param mat The material to update
    */
    void SpacescapeLayerNoise::updateMaterialParams(MaterialPtr mat)
    {
        // shuffle the techniques around because we can't control 
        // the selected technique without overriding the renderable
        // class (ManualObjectSection)
        while(mat->getTechnique(0)->getName() != mNoiseType) {
            // move the technique at index 0 to back of the line
            Technique* t = mat->createTechnique();
            *t = *(mat->getTechnique(0));
            mat->removeTechnique(0);
        }

        // set blending
        mat->getTechnique(0)->getPass(0)->setSceneBlending(mSourceBlendFactor,mDestBlendFactor);

        // set properties for our material
        GpuProgramParametersSharedPtr params = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

        params->setNamedConstant( "ditherAmt",  mDitherAmount );
        params->setNamedConstant( "gain",       mGain );
        params->setNamedConstant( "innerColor", mInnerColor );
        params->setNamedConstant( "lacunarity", mLacunarity );
        params->setNamedConstant( "octaves",    (int)mOctaves );
        params->setNamedConstant( "outerColor", mOuterColor );
        params->setNamedConstant( "powerAmt",   mPowerAmount );
        params->setNamedConstant( "shelfAmt",   mShelfAmount );

        if(mNoiseType == "ridged") {
            params->setNamedConstant( "offset",   mOffset );
        }

        // initialize permutations table
        initNoise(mSeed);

        // now write the perm and gradient values to a texture
        uchar* permLUT = OGRE_ALLOC_T( uchar, 256 * 256 * 4, MEMCATEGORY_GENERAL);

        // we add the Z part in the shader
        for(int Y = 0; Y<256; Y++) {
            for(int X = 0; X<256; X++) {
                int offset = (Y*256+X)*4;

                uchar A = mPermutations[X]; // A = mPermutations[X]+Y (add y later below)
                permLUT[offset + 0]   = mPermutations[A + Y]; // AA = mPermutations[A]+Z (add z in shader)
                permLUT[offset+ 1] = mPermutations[A+Y+1]; // AB = mPermutations[A+1]+Z
                uchar B = mPermutations[X+1]; // we add the y later
                permLUT[offset+2] = mPermutations[B + Y]; // BA = mPermutations[B]+Z (add z in shader)
                permLUT[offset+3] = mPermutations[B+Y+1]; // BB = mPermutations[B+1]+Z (add z in shader)             
            }
        }

        TexturePtr permTex = TextureManager::getSingleton().getByName("SpacescapeNoisePermTexture" + StringConverter::toString(mUniqueID));
        if(permTex.isNull()) {
            // create the permutation texture
            permTex = TextureManager::getSingleton().createManual(
                "SpacescapeNoisePermTexture" + StringConverter::toString(mUniqueID),
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                TEX_TYPE_2D,
                256,
                256,
                0,
                PF_BYTE_RGBA,
                TU_STATIC_WRITE_ONLY
            );
        }
        HardwarePixelBufferSharedPtr pb = permTex->getBuffer();
        if(!pb->isLocked()) {
            // blit from memory to the texture surface
            pb->blitFromMemory(PixelBox(256,256,1,PF_BYTE_RGBA,permLUT));
        }

        mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(permTex->getName());

        // free memory
        OGRE_DELETE_T(permLUT,uchar,MEMCATEGORY_GENERAL);

        // create the gradient texture
        uchar* gradLUT = OGRE_ALLOC_T( uchar, 256 * 3, MEMCATEGORY_GENERAL);
        for(int i = 0; i < 256; i++) {
            int offset = i * 3;
            int index = mPermutations[i] & 15;
            Vector3 v = Vector3(grad3[index][0],grad3[index][1],grad3[index][2]);
            v.normalise();
            v *= 0.5;
            v += 0.5;

            gradLUT[offset + 0] = floor(v.x * 255.0);
            gradLUT[offset + 1] = floor(v.y * 255.0);
            gradLUT[offset + 2] = floor(v.z * 255.0);
        }

        TexturePtr gradTex = TextureManager::getSingleton().getByName("SpacescapeNoiseGradTexture" + StringConverter::toString(mUniqueID));
        if(gradTex.isNull()) {
            // create the gradient look up table
            gradTex = TextureManager::getSingleton().createManual(
                    "SpacescapeNoiseGradTexture" + StringConverter::toString(mUniqueID),
                    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                    TEX_TYPE_1D,
                    256,
                    1,
                    0,
                    PF_BYTE_RGB,
                    TU_STATIC_WRITE_ONLY
            );
        }

        pb = gradTex->getBuffer();
        if(!pb->isLocked()) {
            // blit from memory to the texture surface
            pb->blitFromMemory(PixelBox(256,1,1,PF_BYTE_RGB,gradLUT));
        }

        mat->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName(gradTex->getName());
        mat->load();

        // free memory
        OGRE_FREE(gradLUT,MEMCATEGORY_GENERAL);
    }

    /** Utility function for updating saved params list
    @param params The list of params
    */
    void SpacescapeLayerNoise::updateParams(NameValuePairList params)
    {
        SpacescapeLayer::updateParams(params);

        // update shared params
        mParams["destBlendFactor"] = getBlendMode(mDestBlendFactor);
        mParams["ditherAmount"] = StringConverter::toString(mDitherAmount);
        mParams["gain"] = StringConverter::toString(mGain);
        //mParams["gpu"] = StringConverter::toString(mGPU);
        mParams["innerColor"] = StringConverter::toString(mInnerColor);
        mParams["lacunarity"] = StringConverter::toString(mLacunarity);
        mParams["offset"] = StringConverter::toString(mOffset);
        mParams["octaves"] = StringConverter::toString(mOctaves);
        mParams["outerColor"] = StringConverter::toString(mOuterColor);
        //mParams["persistance"] = StringConverter::toString(mGain);
        mParams["powerAmount"] = StringConverter::toString(mPowerAmount);
        mParams["previewTextureSize"] = StringConverter::toString(mPreviewTextureSize);
        mParams["noiseType"] = mNoiseType;
        mParams["scale"] = StringConverter::toString(mScale);
        mParams["shelfAmount"] = StringConverter::toString(mShelfAmount);
        mParams["sourceBlendFactor"] = getBlendMode(mSourceBlendFactor);
    }
}
