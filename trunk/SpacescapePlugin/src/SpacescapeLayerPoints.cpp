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
#include "SpacescapeLayerPoints.h"
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include "OgreHardwarePixelBuffer.h"
#include "OgreTextureManager.h"

namespace Ogre
{
    /** Constructor
    */
    SpacescapeLayerPoints::SpacescapeLayerPoints(const String& name, SpacescapePlugin* plugin) : 
        SpacescapeLayer(name, plugin),
        mBuilt(false),
        mDestBlendFactor(SBF_ONE),
        mFarColor(0.0,0.0,0.0),
        mMaskNoiseType("fbm"),
        mNearColor(1.0,1.0,1.0),
        mMaskEnabled(false),
        mMaskGain(0.5),
        mMaskLacunarity(2.0),
        mMaskOctaves(1),
        mMaskOffset(1.0),
        mMaskPower(1.0),
        mMaskScale(1.0),
        mMaskSeed(1),
        mMaskThreshold(0.0),
        mNumPoints(1000),
        mPointSize(1),
        mSourceBlendFactor(SBF_ONE),
        mMaterial(0)
    {
    }

    /** Destructor
    */
    SpacescapeLayerPoints::~SpacescapeLayerPoints(void)
    {
        // clear the list
        clear();
    }

    /** Utility function for building based on class params
    */
    void SpacescapeLayerPoints::build(void) 
    {
        // clear the old list
        clear();
        
        // create the material we'll need if not created already
        createMaterial();

        begin(mMaterial->getName(), RenderOperation::OT_POINT_LIST);

        // seed the random number generator
        srand ( mSeed );

        Vector3 v;
        for(unsigned int i = 0; i < mNumPoints; ++i) {
            // nicer distribution
            Real u = -1.0 + 2.0 * rand() / ((double) RAND_MAX);
            Real a = Ogre::Math::TWO_PI * rand() / ((double) RAND_MAX);
            Real s = sqrt(1 - u*u);

            position(
                s * cos(a),
                s * sin(a),
                u
            );

            // random distance
            float dist = rand() / ((double) RAND_MAX);

            // color is based on distance (linear interpolation here)
            colour(mNearColor + (dist * (mFarColor - mNearColor)));
        }

        end();

        mBuilt = true;
    }


    /** Utility function for building based on class params when masked
    */
    void SpacescapeLayerPoints::buildMasked(void)
    {
        // clear the old list
        clear();
        
        // create the material we'll need if not created already
        createMaterial();

        // should be a good approximation
        uint maskSize = 512;

        // create the noise texture (cubic)
        TexturePtr t = TextureManager::getSingleton().createManual(
            "SpacescapeNoiseTexture" + StringConverter::toString(mUniqueID),
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            TEX_TYPE_CUBE_MAP,
            maskSize, maskSize, 
            1,
            0, // no mip maps
            PF_BYTE_RGBA,
            TU_RENDERTARGET
        );

        // rtt a noise mask to a cubic texture then use this texture
        // in place of the noise functions below
        renderNoiseToTexture(
            t,
            mMaskSeed,
            mMaskNoiseType,
            ColourValue::White,
            ColourValue::Black,
            mMaskOctaves,
            mMaskLacunarity,
            mMaskGain,
            mMaskPower,
            mMaskThreshold,
            0.0,
            mMaskScale,
            mMaskOffset
        );

        // face orientation is +X (0), -X (1), +Y (2), -Y (3), +Z (4), -Z (5)
        // copy the texture into buffers
        uchar* faceBuffers[6];
        for(int i = 0 ; i < 6; ++i) {
            faceBuffers[i] = OGRE_ALLOC_T( uchar, maskSize * maskSize * 4, MEMCATEGORY_GENERAL);
            t->getBuffer(i)->blitToMemory(
                PixelBox(t->getWidth(), t->getHeight(), 1, PF_BYTE_RGBA, faceBuffers[i])
            );
        }
        
        // instead of generating random points on a sphere, generate random points
        // on a cube and then it should be easy to sample these points from the cubic
        // map and use them if they're valid - unfortunately this method will cause
        // bunching up at cube corners.  Using an even spherical sample would be better
        // but then we need to translate from spherical to cube space to sample from
        // the noise map (TODO?)
        begin(mMaterial->getName(), RenderOperation::OT_POINT_LIST);

        // seed the random number generator
        srand ( mSeed );

        unsigned int numPoints = std::min<unsigned int>(RAND_MAX * RAND_MAX,mNumPoints);
        unsigned int numPointsTested = 0;
        unsigned int maxNumTestPoints = 99999;

        Real n;
        Real scale = 2.0 / (Real)maskSize;
        Real noiseScale = 1.0 / 255.0;
        while(numPoints) {
            // pick random co-ords on the top face
            Real rU = rand() / ((double) RAND_MAX);
            Real rV = rand() / ((double) RAND_MAX);

            // scale u,v to 0..maskSize
            uint u = std::min<uint>(rU * maskSize,maskSize);
            uint v = std::min<uint>(rV * maskSize,maskSize);

            // pick a random face
            uchar face = rand() % 6;

            // use noise mask to discard positions
            ++numPointsTested;

            // get the noise value at this position 0..255
            n = faceBuffers[face][((v * maskSize) + u) * 4];

            // scale n to between 0..1
            n *= noiseScale;

            // get a random value between 0..1 to use for density test
            double r = rand() / ((double) RAND_MAX);

            // now see if the random value is less than the noise value
            // should give us a greater density of points for higher noise values
            if(r > (n * n)) {

                // only test a certain number of points so we don't infinite loop
                if(numPointsTested == maxNumTestPoints) {
                    numPoints--;
                }
                continue;
            }
            
            // scale u and v to range -1 .. 1
            Vector3 p = Vector3( (rU * 2.0) - 1.0 , 1.0, (rV * 2.0) - 1.0);

            // rotate v to this face on the unit cube centered at 0,0,0
            rotatePoint(p,face);
            p.z = -p.z;

            // use this position
            position(p);

            numPoints--;
            numPointsTested = 0;

            // random distance
            float dist = rand() / ((double) RAND_MAX);

            // color is based on distance (linear interpolation here)
            colour(mNearColor + (dist * (mFarColor - mNearColor)));
        }

        for(int i = 0 ; i < 6; ++i) {
            OGRE_FREE(faceBuffers[i],MEMCATEGORY_GENERAL);
        }

        end();

        TextureManager::getSingleton().remove(t->getHandle());

        mBuilt = true;
    }

    /** Create the material we'll need if not created already
    */
    void SpacescapeLayerPoints::createMaterial(void)
    {
        if(mMaterial.isNull()) {
            // create the material and set initial params that don't change
            mMaterial = (MaterialPtr)MaterialManager::getSingleton().create(
                "SpacescapePointsMaterial" + StringConverter::toString(mUniqueID), 
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            mMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
            mMaterial->getTechnique(0)->getPass(0)->setPointSpritesEnabled(true);
            mMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
            mMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
        }

        // update the point size if necessary
        mMaterial->getTechnique(0)->getPass(0)->setPointSize(mPointSize);

        // set blending
        mMaterial->getTechnique(0)->getPass(0)->setSceneBlending(mSourceBlendFactor,mDestBlendFactor);

        // make sure the material is loaded
        mMaterial->load();
    }

    /** Initialize this layer based on the given params
    @remarks Params for this layer type are:
    NAME -  VALUE TYPE
    farColor - ColourValue (string i.e. "0.0 0.5 1.0")
    nearColor - ColourValue (string i.e. "0.1 0.9 1.0")
    numPoints - unsigned int (string i.e. "100")
    pointSize - unsigned int (string i.e. "1")
    seed - unsigned int (string i.e. "999")
    @param params Layer params that will be specific to the derived class
    */
    void SpacescapeLayerPoints::init(Ogre::NameValuePairList params)
    {
        bool shouldUpdate = false;

        NameValuePairList::iterator ii;
        for(ii = params.begin(); ii != params.end(); ii++) {
            if(ii->first == "destBlendFactor") {
                shouldUpdate |= mDestBlendFactor != getBlendMode(ii->second);
                mDestBlendFactor = getBlendMode(ii->second);
            }
            else if(ii->first == "farColor") {
                shouldUpdate |= mFarColor != StringConverter::parseColourValue(ii->second);
                mFarColor = StringConverter::parseColourValue(ii->second);
            }
            else if(ii->first == "maskEnabled") {
                shouldUpdate |= mMaskEnabled != StringConverter::parseBool(ii->second);
                mMaskEnabled = StringConverter::parseBool(ii->second);
            }
            else if(ii->first == "maskGain") {
                shouldUpdate |= mMaskGain != StringConverter::parseReal(ii->second);
                mMaskGain = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "maskLacunarity") {
                shouldUpdate |= mMaskLacunarity != StringConverter::parseReal(ii->second);
                mMaskLacunarity = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "maskNoiseType") {
                shouldUpdate |= mMaskNoiseType != ii->second;
                mMaskNoiseType = ii->second;
            }
            else if(ii->first == "maskOctaves") {
                shouldUpdate |= mMaskOctaves != StringConverter::parseUnsignedInt(ii->second);
                mMaskOctaves = StringConverter::parseUnsignedInt(ii->second);
            }
            else if(ii->first == "maskOffset") {
                shouldUpdate |= mMaskOffset != StringConverter::parseReal(ii->second);
                mMaskOffset = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "maskPower") {
                shouldUpdate |= mMaskPower != StringConverter::parseReal(ii->second);
                mMaskPower = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "maskScale") {
                shouldUpdate |= mMaskScale != StringConverter::parseReal(ii->second);
                mMaskScale = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "maskSeed") {
                shouldUpdate |= mMaskSeed != StringConverter::parseInt(ii->second);
                mMaskSeed = StringConverter::parseInt(ii->second);
            }
            else if(ii->first == "maskThreshold") {
                shouldUpdate |= mMaskThreshold != std::min<Real>(1.0,std::max<Real>(0.0,StringConverter::parseReal(ii->second)));
                mMaskThreshold = std::min<Real>(1.0,std::max<Real>(0.0,StringConverter::parseReal(ii->second)));
            }
            else if(ii->first == "name") {
                mName = ii->second;
            }
            else if(ii->first == "nearColor") {
                shouldUpdate |= mNearColor != StringConverter::parseColourValue(ii->second);
                mNearColor = StringConverter::parseColourValue(ii->second);
            }
            else if(ii->first =="numPoints") {
                shouldUpdate |= mNumPoints != StringConverter::parseInt(ii->second);
                mNumPoints = StringConverter::parseInt(ii->second);
            }
            else if(ii->first == "pointSize") {
                shouldUpdate |= mPointSize != StringConverter::parseInt(ii->second);
                mPointSize = StringConverter::parseInt(ii->second);
            }
            else if(ii->first == "seed") {
                shouldUpdate |= mSeed != StringConverter::parseInt(ii->second);
                mSeed = StringConverter::parseInt(ii->second);
            }
            else if(ii->first == "sourceBlendFactor") {
                shouldUpdate |= mSourceBlendFactor != getBlendMode(ii->second);
                mSourceBlendFactor = getBlendMode(ii->second);
            }
        }

        // update our string params - also checks for changes that would cause an update
        updateParams(params);

        // now build based on these settings
        if(shouldUpdate || !mBuilt) {
            if(mMaskEnabled) {
                buildMasked();
            }
            else {
                build();        
            }
        }
    }

    /** Utility function for updating saved params list
    @param params The list of params
    */
    void SpacescapeLayerPoints::updateParams(NameValuePairList params)
    {
        SpacescapeLayer::updateParams(params);

        // update shared params
        mParams["destBlendFactor"] = getBlendMode(mDestBlendFactor);
        mParams["farColor"] = StringConverter::toString(mFarColor);
        mParams["maskEnabled"] = StringConverter::toString(mMaskEnabled);
        mParams["maskGain"] = StringConverter::toString(mMaskGain);
        mParams["maskLacunarity"] = StringConverter::toString(mMaskLacunarity);
        mParams["maskNoiseType"] = mMaskNoiseType;
        mParams["maskOctaves"] = StringConverter::toString(mMaskOctaves);
        mParams["maskOffset"] = StringConverter::toString(mMaskOffset);
        mParams["maskPower"] = StringConverter::toString(mMaskPower);
        mParams["maskScale"] = StringConverter::toString(mMaskScale);
        mParams["maskSeed"] = StringConverter::toString(mMaskSeed);
        mParams["maskThreshold"] = StringConverter::toString(mMaskThreshold);
        mParams["nearColor"] = StringConverter::toString(mNearColor);
        mParams["numPoints"] = StringConverter::toString(mNumPoints);
        mParams["pointSize"] = StringConverter::toString(mPointSize);
        mParams["sourceBlendFactor"] = getBlendMode(mSourceBlendFactor);
    }
}
