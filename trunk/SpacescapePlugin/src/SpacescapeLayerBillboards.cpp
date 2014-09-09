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
#include "SpacescapeLayerBillboards.h"
#include "OgreRoot.h"
#include "OgreBillboard.h"
#include "OgreMaterialManager.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreTextureManager.h"
#include "OgreLogManager.h"
#include "OgreTechnique.h"
#include "OgreHighLevelGpuProgram.h"

#ifdef EXR_SUPPORT
#include "OgreHighLevelGpuProgramManager.h"
#endif

namespace Ogre
{
#ifdef EXR_SUPPORT
    static const String spacescape_billboards_glsl_vp = "attribute vec2 uv0;\n\
    uniform mat4 worldViewProj;\n\
    varying vec3 hdrColor;\n\
    varying vec2 UV;\n\
    void main()\n\
    {\n\
        gl_Position = worldViewProj * gl_Vertex;\n\
        hdrColor = gl_Normal.xyz;\n\
        UV = uv0;\n\
    }";
    
	static const String spacescape_billboards_glsl_fp = "uniform sampler2D tex;\n\
														    varying vec3 hdrColor;\n\
															    varying vec2 UV;\n\
																    void main()\n\
																	    {\n\
		vec4 d = texture2D(tex,UV); \n\
        gl_FragColor = vec4(hdrColor.r * d.r,hdrColor.g * d.g,hdrColor.b * d.b,d.a);\n\
    }";

#endif
    
    /** Constructor
    */
    SpacescapeLayerBillboards::SpacescapeLayerBillboards(const String& name, SpacescapePlugin* plugin) :
        SpacescapeLayer(name, plugin),
        mBillboardSet(0),
        mBuilt(false),
        mDestBlendFactor(SBF_ONE),
//        mDestBlendFactor(SBF_ONE_MINUS_SOURCE_COLOUR),
        mFarColor(ColourValue(1.0,1.0,1.0)),
#ifdef EXR_SUPPORT
        mHDRPower(1.0),
        mHDRMultiplier(1.0),
#endif
        mMaskEnabled(false),
        mMaskGain(0.5),
        mMaskLacunarity(2.0),
        mMaskOctaves(1),
        mMaskOffset(1.0),
        mMaskPower(1.0),
        mMaskScale(1.0),
        mMaskSeed(1),
        mMaskThreshold(0.0),
        mMaxSize(0.2),
        mMinSize(0.2),
        mNearColor(ColourValue(1.0,1.0,1.0)),
        mNumBillboards(100),
        mSourceBlendFactor(SBF_ONE),
        mStarDataFilename("")
    {
        mMaterial.setNull();
#ifdef EXR_SUPPORT
        mTextureName = "hdr-flare-white2.exr";
#else
        mTextureName = "default.png";
#endif
        //mStarDataFilename = "/Users/alex/Documents/workspace/spacescape/spacescape-git/stars-all.csv";
    }

    /** Destructor
    */
    SpacescapeLayerBillboards::~SpacescapeLayerBillboards(void)
    {
        // remove all billboards
        /*
        if(mBillboardSet) {
            mBillboardSet->clear();
            OGRE_DELETE mBillboardSet;
        }
        */
    }

    /** Utility function for building based on class params
    */
    void SpacescapeLayerBillboards::build(void) 
    {
        createBillboardSet();

        // seed random number generator
        srand(mSeed);

        // now create the billboards
        Vector3 v;
        ColourValue c;
        for(unsigned int i = 0; i < mNumBillboards; ++i) {
           // nice distribution of random points on sphere
            Real u = -1.0 + 2.0 * rand() / ((double) RAND_MAX);
            Real a = Ogre::Math::TWO_PI * rand() / ((double) RAND_MAX);
            Real s = sqrt(1 - u*u);

//            v.x = s * cos(a);
//            v.y = s * sin(a);
//            v.z = u;
//            v.normalise();
            SpacescapeBillboard* b = mBillboardSet->createBillboard(
                s * cos(a),
                s * sin(a),
                u
            );

            // random distance
            double dist = rand() / ((double) RAND_MAX);

#ifdef EXR_SUPPORT
            dist = powf(dist, mHDRPower);
#endif
            // size is based on distance and min/max allowed sizes
            // closer distances are larger
            Real size = mMinSize + (mMaxSize - mMinSize) * (1.0 - dist);
            b->setDimensions(size, size);

            // color is based on distance (linear interpolation here)
            c = mNearColor + (dist * (mFarColor - mNearColor));
            b->setColour(c);
            
#ifdef EXR_SUPPORT
            c *= mHDRMultiplier;
            b->mHDRColour = c;
#endif
        }

        mBuilt = true;
    }

    /** Utility function for building based on class params (masked)
    */
    void SpacescapeLayerBillboards::buildMasked(void) 
    {
        createBillboardSet();

        // remove the old noise texture if it exists (new one might be diff size due to layer switch ups)
        TexturePtr t = TextureManager::getSingleton().getByName("SpacescapeBillboardMask");
        if(!t.isNull()) {
            TextureManager::getSingleton().remove(t->getHandle());
        }

        // should be a good approximation
        uint maskSize = 512;

        // create the noise texture (cubic)
        t = TextureManager::getSingleton().createManual(
            "SpacescapeBillboardMask",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            TEX_TYPE_CUBE_MAP,
            maskSize, maskSize, 
            1,
            0, // no mip maps
            mMaskFBOPixelFormat,
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
                PixelBox(t->getWidth(), t->getHeight(), 1, t->getFormat(), faceBuffers[i])
            );
        }

        // seed random number generator
        srand(mSeed);

        // now create the billboards
        unsigned int numPoints = std::min<unsigned int>(RAND_MAX,mNumBillboards);
        unsigned int numPointsTested = 0;
        unsigned int maxNumTestPoints = 99999;

        Real n;
        //Real scale = 2.0 / (Real)maskSize;
        ColourValue c;
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
            SpacescapeBillboard* b = mBillboardSet->createBillboard(p.normalisedCopy());
            numPoints--;
            numPointsTested = 0;

            // random distance
            float dist = rand() / ((double) RAND_MAX);

#ifdef EXR_SUPPORT
            dist = powf(dist, mHDRPower);
#endif
            // size is based on distance and min/max allowed sizes
            // closer distances are larger
            Real size = mMinSize + (mMaxSize - mMinSize) * (1.0 - dist);
            b->setDimensions(size, size);
            
            // color is based on distance (linear interpolation here)
            c = mNearColor + (dist * (mFarColor - mNearColor));
            b->setColour(c);
            
#ifdef EXR_SUPPORT
            c *= mHDRMultiplier;
            b->mHDRColour = c;
#endif
        }

        for(int i = 0 ; i < 6; ++i) {
            OGRE_FREE(faceBuffers[i],MEMCATEGORY_GENERAL);
        }

        // free the temp noise mask texture
        TextureManager::getSingleton().remove(t->getHandle());

        mBuilt = true;
    }

    /** Utility function for building based on predefined positions/colours
     */
    void SpacescapeLayerBillboards::buildFromFile(const String &filename)
    {
        createBillboardSet();
        
        // load/parse the file - we don't handle quotes!!
        std::fstream dataFile(filename, std::ios_base::in);
        std::string line;
        int xOffset = -1;
        int yOffset = -1;
        int zOffset = -1;
        int bvOffset = -1;
        int magOffset = -1;
        int nameOffset = -1;
        int distanceOffset = -1;
        int isHeader = true;
        
        double maxDist = 20000.0; // in parsecs?
        
        // remember magnitude is reversed! -1.5 is brightest and 6.5 is dimmest
        double magMin = -1.5;
        double magMax = 6.5;
        double magRatio = 1.0 / (magMax - magMin);
        while(getline(dataFile, line)) {
            std::stringstream ss(line);
            std::string item;
            if(isHeader) {
                int offset = 0;
                while(std::getline(ss, item, ',')) {
                    if(item == "x" || item == "X") {
                        xOffset = offset;
                    }
                    else if(item == "y" || item == "Y") {
                        yOffset = offset;
                    }
                    else if(item == "z" || item == "Z") {
                        zOffset = offset;
                    }
                    else if(item == "colorIndex" || item == "ColorIndex" || item == "bv" || item == "BV") {
                        bvOffset = offset;
                    }
                    else if(item == "AbsMag" || item == "absmag") {
                        magOffset = offset;
                    }
                    else if(item == "distance" || item == "Distance") {
                        distanceOffset = offset;
                    }
                    else if(item == "ProperName" || item == "name") {
                        nameOffset = offset;
                    }
                    offset++;
                }
            }
            else {
                if(xOffset == -1 || yOffset == -1 || zOffset == -1 || nameOffset == -1) {
                    Ogre::LogManager::getSingleton().getDefaultLog()->stream() <<
                    "CSV file first line must have x,y,z,spectrum,absmag,distance,name";
                    break;
                }
                
                std::vector<std::string> elems;
                while(std::getline(ss,item,',')) {
                    elems.push_back(item);
                }
                
                if(elems.size() < 6) {
                    continue;
                }
                

                // scale distance from 0..maxDist to 0..1
                Real dist = Ogre::StringConverter::parseReal(elems[distanceOffset]);
                
                // skip objects like our sun that are too close
                if(dist < 0.1) continue;

                // magnitude is inverse! wierdo astronomers
                Real mag = Ogre::StringConverter::parseReal(elems[magOffset]);
                
                Real brightness = mag - 5*log10(10.0/dist);
                // skip objects that are too faint

                if(brightness > magMax) continue;
                
                // position gets normalised
                Vector3 pos = Ogre::StringConverter::parseVector3(elems[xOffset] + " " + elems[yOffset] + " " + elems[zOffset]);
                pos.normalise();
                SpacescapeBillboard* b = mBillboardSet->createBillboard(pos);
                
                
                dist = std::min<Real>(dist,maxDist);
                dist *= 1.0/maxDist;
                dist = std::max<Real>(0,dist);

                // size is based on distance and min/max allowed sizes
                // closer distances are larger
                Real size = mMinSize + (mMaxSize - mMinSize) * (1.0 - dist);
                b->setDimensions(size, size);
                
                ColourValue c = getColourValueFromBV(Ogre::StringConverter::parseReal(elems[bvOffset]));
                
                b->setColour(c);
                
                mag = (magMax - magMin) - brightness - magMin;
#ifdef EXR_SUPPORT
                if(mHDRPower != 1.0) {
                    mag *= magRatio;
                    mag = pow(mag,mHDRPower);
                    mag *= (magMax - magMin);
                }
#endif
                
               
                c *= mag;
#ifdef EXR_SUPPORT
                c *= mHDRMultiplier;
//                c *=
                b->mHDRColour = c;
#endif
//                Ogre::LogManager::getSingleton().getDefaultLog()->stream() <<
//                "Billboard: mag: " << Ogre::StringConverter::toString(mag) << "(" << elems[magOffset] << ")";
//                Ogre::LogManager::getSingleton().getDefaultLog()->stream() <<
//                "Billboard: " << elems[xOffset] << " " << elems[yOffset] << " " << elems[zOffset] << " distance: " << Ogre::StringConverter::toString(dist) << "(" << elems[distanceOffset] << ")" << " color: " << Ogre::StringConverter::toString(c) << "(" << elems[bvOffset] << ") mag: " << elems[magOffset];
            }
            isHeader = false;
        }
        dataFile.close();
        
        mBuilt = true;
    }
    
    /** Utility function for preparing the billboard set
     */
    void SpacescapeLayerBillboards::createBillboardSet()
    {
        // get the default scene manager
        if(!Ogre::Root::getSingleton().getSceneManagerIterator().hasMoreElements()) {
            Ogre::LogManager::getSingleton().getDefaultLog()->stream() <<
            "No scene manager found in SpacescapePlugin::addLayer().  You can't add a layer before creating a scene manager.";
        }
        SceneManager* sceneMgr = Root::getSingleton().getSceneManagerIterator().peekNextValue();
        
        // create the billboardset if it doesn't exist
        String name = "SpacescapeLayerBillboardset" + StringConverter::toString(mUniqueID);
        if(!sceneMgr->hasMovableObject(name, SpacescapeBillboardSetFactory::FACTORY_TYPE_NAME)) {
            if(!Ogre::Root::getSingleton().hasMovableObjectFactory(SpacescapeBillboardSetFactory::FACTORY_TYPE_NAME)) {
                Ogre::Root::getSingleton().addMovableObjectFactory(OGRE_NEW SpacescapeBillboardSetFactory());
            }
            NameValuePairList params;
            params["poolSize"] = StringConverter::toString(mNumBillboards);
            mBillboardSet = static_cast<SpacescapeBillboardSet*>(sceneMgr->createMovableObject(name, SpacescapeBillboardSetFactory::FACTORY_TYPE_NAME, &params));
        }
        else {
            mBillboardSet = static_cast<SpacescapeBillboardSet*>(sceneMgr->getMovableObject(name, SpacescapeBillboardSetFactory::FACTORY_TYPE_NAME));
            
        }
        
        // clear out all existing billboards
        if(mBillboardSet) {
            mBillboardSet->clear();
        }
        
        // initialize the billboard set
        mBillboardSet->setPoolSize(mNumBillboards);
        mBillboardSet->setMaterialName(mMaterial->getName());
        mBillboardSet->setDefaultDimensions(mMinSize,mMinSize);
        mBillboardSet->setCastShadows(false);
        mBillboardSet->setUseAccurateFacing(true);
    }

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
    void SpacescapeLayerBillboards::init(Ogre::NameValuePairList params)
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
            else if(ii->first == "minSize") {
                shouldUpdate |= mMinSize != StringConverter::parseReal(ii->second);
                mMinSize = StringConverter::parseReal(ii->second);
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
            else if(ii->first == "maxSize") {
                shouldUpdate |= mMaxSize != StringConverter::parseReal(ii->second);
                mMaxSize = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "name") {
                mName = ii->second;
            }
            else if(ii->first == "nearColor") {
                shouldUpdate |= mNearColor != StringConverter::parseColourValue(ii->second);
                mNearColor = StringConverter::parseColourValue(ii->second);
            }
            else if(ii->first == "numBillboards") {
                shouldUpdate |= mNumBillboards != StringConverter::parseInt(ii->second);
                mNumBillboards = StringConverter::parseInt(ii->second);
            }
            else if(ii->first == "seed") {
                shouldUpdate |= mSeed != StringConverter::parseInt(ii->second);
                mSeed = StringConverter::parseInt(ii->second);
            }
            else if(ii->first == "sourceBlendFactor") {
                shouldUpdate |= mSourceBlendFactor != getBlendMode(ii->second);
                mSourceBlendFactor = getBlendMode(ii->second);
            }
            else if(ii->first == "texture") {
                shouldUpdate |= mTextureName != ii->second;
                mTextureName = ii->second;
            }
#ifdef EXR_SUPPORT
            else if(ii->first == "hdrPower") {
                shouldUpdate |= mHDRPower != StringConverter::parseReal(ii->second);
                mHDRPower = StringConverter::parseReal(ii->second);
            }
            else if(ii->first == "hdrMultiplier") {
                shouldUpdate |= mHDRMultiplier != StringConverter::parseReal(ii->second);
                mHDRMultiplier = StringConverter::parseReal(ii->second);
            }
#endif
            else if(ii->first == "dataFile") {
                shouldUpdate |= mStarDataFilename != ii->second;
                mStarDataFilename = ii->second;
            }
        }

        // update our saved params
        updateParams(params);

        // build/update based on these settings
        if(shouldUpdate || !mBuilt) {
            // update material fragment program parameters
            updateMaterial();

            if(mStarDataFilename != "") {
                buildFromFile(mStarDataFilename);
            }
            else if(mMaskEnabled) {
                buildMasked();
            }
            else {
                build();
            }
        }
    }

    /** Update the material with new params - will create if needed
    */
    void SpacescapeLayerBillboards::updateMaterial(void)
    {
        // get our unique material
        if(mMaterial.isNull()) {
            // create the material
            mMaterial = MaterialManager::getSingletonPtr()->create(
                "SpacescapeBillboardMat" + StringConverter::toString(mUniqueID),
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
            );
            mMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
            mMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
            mMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);

            // create a single texture unit state for our billboard texture
            mMaterial->getTechnique(0)->getPass(0)->createTextureUnitState();
            
#ifdef EXR_SUPPORT
            GpuProgramParametersSharedPtr params;
            HighLevelGpuProgramPtr gpuProgram;
            Pass* pass = mMaterial->getTechnique(0)->getPass(0);
            
            // load the vertex program
            gpuProgram = HighLevelGpuProgramManager::getSingleton().
            createProgram("spacescape_billboards_glsl_vp",
                          ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                          "glsl",
                          GPT_VERTEX_PROGRAM);
            gpuProgram->setSource(spacescape_billboards_glsl_vp);
            gpuProgram->load();
            
            // set the vertex program
            pass->setVertexProgram("spacescape_billboards_glsl_vp");
            
            // set vertex program params
            params = pass->getVertexProgramParameters();
            params->setNamedAutoConstant("worldViewProj",GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
            
            // load the fragment program
            gpuProgram = HighLevelGpuProgramManager::getSingleton().
            createProgram("spacescape_billboards_glsl_fp",
                          ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                          "glsl",
                          GPT_FRAGMENT_PROGRAM);
            gpuProgram->setSource(spacescape_billboards_glsl_fp);
            
            params = gpuProgram->getDefaultParameters();
            params->setNamedConstant("tex",(int)0);
            
            gpuProgram->load();
            
            
            // set the fragment program
            pass->setFragmentProgram("spacescape_billboards_glsl_fp");
#endif
        }
        
        // set blending
        mMaterial->getTechnique(0)->getPass(0)->setSceneBlending(mSourceBlendFactor,mDestBlendFactor);

        // load texture if not loaded yet
        if(!mTextureName.empty() && TextureManager::getSingleton().getByName(mTextureName).isNull()) {
            try {
                // try to load
#ifdef EXR_SUPPORT
                TextureManager::getSingleton().load(mTextureName,
                                                    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,TEX_TYPE_2D,
                                                    0,
                                                    1.0,
                                                    false,
                                                    PF_FLOAT32_RGBA,
                                                    false);
//                virtual TexturePtr load(
//                                        const String& name, const String& group,
//                                        TextureType texType = TEX_TYPE_2D, int numMipmaps = MIP_DEFAULT,
//                                        Real gamma = 1.0f, bool isAlpha = false,
//                                        PixelFormat desiredFormat = PF_UNKNOWN, 
//                                        bool hwGammaCorrection = false);
#else
                TextureManager::getSingleton().load(mTextureName,ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#endif
            }
            catch(...) {
                // couldn't find the file
            }
        }

        if(!mTextureName.empty() && !TextureManager::getSingleton().getByName(mTextureName).isNull()) {
//#ifdef EXR_SUPPORT
//            TexturePtr tex = TextureManager::getSingleton().getByName(mTextureName);
//            Ogre::LogManager::getSingleton().getDefaultLog()->stream() << "tex format is " <<
//            Ogre::StringConverter::toString(tex->getFormat());
//
//            mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureFiltering(TFO_ANISOTROPIC);
//            mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTexture(tex);
//#else
//            Ogre::LogManager::getSingleton().getDefaultLog()->stream() << "billboard texture is " <<
//            mTextureName;
            mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(mTextureName);
//#endif
        }

        // make sure material is loaded
        mMaterial->load();
    }

    /** Utility function for updating saved params list
    @param params The list of params
    */
    void SpacescapeLayerBillboards::updateParams(NameValuePairList params)
    {
        SpacescapeLayer::updateParams(params);

        // update shared params
        mParams["destBlendFactor"] = getBlendMode(mDestBlendFactor);
        mParams["dataFile"] = mStarDataFilename;
        mParams["farColor"] = StringConverter::toString(mFarColor);
        mParams["minSize"] = StringConverter::toString(mMinSize);
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
        mParams["maxSize"] = StringConverter::toString(mMaxSize);
        mParams["nearColor"] = StringConverter::toString(mNearColor);
        mParams["numBillboards"] = StringConverter::toString(mNumBillboards);
        mParams["sourceBlendFactor"] = getBlendMode(mSourceBlendFactor);
        mParams["texture"] = mTextureName;
#ifdef EXR_SUPPORT
        mParams["hdrPower"] = StringConverter::toString(mHDRPower);
        mParams["hdrMultiplier"] = StringConverter::toString(mHDRMultiplier);
#endif
    }
}
