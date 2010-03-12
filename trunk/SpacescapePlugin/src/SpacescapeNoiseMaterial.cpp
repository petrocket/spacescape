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
#include "../include/SpacescapeNoiseMaterial.h"
#include "OgreMaterialManager.h"
#include "OgreMaterial.h"
#include "OgreTechnique.h"
#include "OgreRoot.h"
#include "OgrePass.h"
#include "OgreTextureManager.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreRenderSystem.h"

namespace Ogre
{
    static const String spacescape_noise_hlsl_fbm = "";
    static const String spacescape_noise_hlsl_ridged = "";

    static const String spacescape_noise_glsl_vp = "uniform mat4 worldViewProj;\n\
            varying vec3 vertexPos;\n\
            void main()\n\
            {\n\
	           gl_Position = worldViewProj * gl_Vertex;\n\
               vertexPos = normalize(gl_Vertex.xyz);\n\
            }";

    static const String spacescape_noise_glsl_fbm_fp = "uniform sampler2D permTexture;\n\
            uniform sampler1D gradTexture;\n\
            uniform float ditherAmt;\n\
            uniform float gain;\n\
            uniform vec3 innerColor;\n\
            uniform float lacunarity;\n\
            uniform int octaves;\n\
            uniform vec3 outerColor;\n\
            uniform float powerAmt;\n\
            uniform float shelfAmt;\n\
            uniform float noiseScale;\n\
\n\
            varying vec3 vertexPos;\n\
            vec3 fade(vec3 t)  \n\
            {  \n\
                return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); // new curve\n\
            }\n\
\n\
            vec4 perm2d(vec2 p)  \n\
            {  \n\
                return texture2D(permTexture, p);  \n\
            }  \n\
               \n\
            float gradperm(float x, vec3 p)  \n\
            {  \n\
                vec3 v = texture1D(gradTexture, x).xyz;\n\
                v *= 2.0;\n\
                v -= 1.0;    \n\
                return dot(v, p);  \n\
            } \n\
\n\
            // noise values returned are between -1.0 and 1.0!!!\n\
            float perlinNoise(vec3 p)\n\
            {\n\
                vec3 P = mod(floor(p), 256.0);     // FIND UNIT CUBE THAT CONTAINS POINT  \n\
                p -= floor(p);                      // FIND RELATIVE X,Y,Z OF POINT IN CUBE.  \n\
                vec3 f = fade(p);                  // COMPUTE FADE CURVES FOR EACH OF X,Y,Z.  \n\
               \n\
                P = P / 256.0;\n\
                   \n\
                // HASH COORDINATES OF THE 8 CUBE CORNERS  \n\
                vec4 AA = perm2d(P.xy) + P.z;  \n\
               \n\
                 // AND ADD BLENDED RESULTS FROM 8 CORNERS OF CUBE  \n\
                 return mix( mix( mix( gradperm(AA.x, p ),  \n\
                                          gradperm(AA.z, p + vec3(-1, 0, 0) ), f.x),  \n\
                                    mix( gradperm(AA.y, p + vec3(0, -1, 0) ),  \n\
                                          gradperm(AA.w, p + vec3(-1, -1, 0) ), f.x), f.y),  \n\
                              mix( mix( gradperm(AA.x+(1.0 / 256.0), p + vec3(0, 0, -1) ),  \n\
                                          gradperm(AA.z+(1.0 / 256.0), p + vec3(-1, 0, -1) ), f.x),  \n\
                                    mix( gradperm(AA.y+(1.0 / 256.0), p + vec3(0, -1, -1) ),  \n\
                                          gradperm(AA.w+(1.0 / 256.0), p + vec3(-1, -1, -1) ), f.x), f.y), f.z);\n\
            }\n\
\n\
            /*\n\
             * FBM (Fractal Brownian Motion) noise\n\
             */\n\
            float fbmNoise(vec3 vIn, int octaves, float lacunarity, float gain)\n\
            {\n\
                vec3 v = vIn;\n\
                \n\
                float noiseSum = 0.0;\n\
                float amplitude = 1.0;\n\
                float amplitudeSum = 0.0;\n\
                \n\
                // make some fbm noise\n\
                for( int i = 0; i < octaves; i++) {\n\
                    noiseSum += perlinNoise(v) * amplitude;\n\
                    amplitudeSum += amplitude;\n\
                    amplitude *= gain;\n\
                    v *= lacunarity;\n\
                }\n\
                \n\
                // get noiseSum in range -1..1   \n\
                return noiseSum / amplitudeSum;\n\
            }\n\
\n\
            void main( void )\n\
            {\n\
                vec3 v = normalize(vertexPos);\n\
                float noiseSum = fbmNoise(noiseScale * v, octaves, lacunarity, gain);\n\
\n\
                // add a crazy amount of dithering noise\n\
                noiseSum += fbmNoise(v * 10000.0, 2, lacunarity, gain) * ditherAmt;\n\
\n\
                // get noiseSum in range 0..1\n\
                noiseSum = (noiseSum*0.5) + 0.5;\n\
                \n\
                // apply shelf\n\
                noiseSum = max(0.0,noiseSum - shelfAmt);\n\
                \n\
                // scale whatever survives back into 0..1 range\n\
                noiseSum *= 1.0/(1.0-shelfAmt);\n\
                \n\
                // apply optional power function\n\
                noiseSum = pow(noiseSum,1.0/powerAmt);\n\
\n\
                gl_FragColor.xyz = mix(outerColor, innerColor, noiseSum);\n\
                gl_FragColor.w = noiseSum;\n\
            }";
    static const String spacescape_noise_glsl_ridged_fp = "uniform sampler2D permTexture;\n\
            uniform sampler1D gradTexture;\n\
            uniform float ditherAmt;\n\
            uniform float gain;\n\
            uniform vec3 innerColor;\n\
            uniform float lacunarity;\n\
            uniform float offset;\n\
            uniform int octaves;\n\
            uniform vec3 outerColor;\n\
            uniform float shelfAmt;\n\
            uniform float powerAmt;\n\
            uniform float noiseScale;\n\
\n\
            varying vec3 vertexPos;\n\
            vec3 fade(vec3 t)  \n\
            {  \n\
                return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); // new curve\n\
            }\n\
\n\
            vec4 perm2d(vec2 p)  \n\
            {  \n\
                return texture2D(permTexture, p);  \n\
            }  \n\
               \n\
            float gradperm(float x, vec3 p)  \n\
            {  \n\
                vec3 v = texture1D(gradTexture, x).xyz;\n\
                v *= 2.0;\n\
                v -= 1.0;    \n\
                return dot(v, p);  \n\
            } \n\
\n\
            // noise values returned are between -1.0 and 1.0!!!\n\
            float perlinNoise(vec3 p)\n\
            {\n\
                vec3 P = mod(floor(p), 256.0);     // FIND UNIT CUBE THAT CONTAINS POINT  \n\
                p -= floor(p);                      // FIND RELATIVE X,Y,Z OF POINT IN CUBE.  \n\
                vec3 f = fade(p);                  // COMPUTE FADE CURVES FOR EACH OF X,Y,Z.  \n\
               \n\
                P = P / 256.0;\n\
                   \n\
                // HASH COORDINATES OF THE 8 CUBE CORNERS  \n\
                vec4 AA = perm2d(P.xy) + P.z;  \n\
               \n\
                 // AND ADD BLENDED RESULTS FROM 8 CORNERS OF CUBE  \n\
                 return mix( mix( mix( gradperm(AA.x, p ),  \n\
                                          gradperm(AA.z, p + vec3(-1, 0, 0) ), f.x),  \n\
                                    mix( gradperm(AA.y, p + vec3(0, -1, 0) ),  \n\
                                          gradperm(AA.w, p + vec3(-1, -1, 0) ), f.x), f.y),  \n\
                              mix( mix( gradperm(AA.x+(1.0 / 256.0), p + vec3(0, 0, -1) ),  \n\
                                          gradperm(AA.z+(1.0 / 256.0), p + vec3(-1, 0, -1) ), f.x),  \n\
                                    mix( gradperm(AA.y+(1.0 / 256.0), p + vec3(0, -1, -1) ),  \n\
                                          gradperm(AA.w+(1.0 / 256.0), p + vec3(-1, -1, -1) ), f.x), f.y), f.z);\n\
            }\n\
\n\
            /*\n\
             * Ridge function for Ridged FBM noise below\n\
             */\n\
            float ridge(float noiseVal, float offset)\n\
            {\n\
                float newVal = offset - abs(noiseVal);\n\
                return newVal * newVal;\n\
            }\n\
            \n\
            /*\n\
             * Ridged FBM (Fractal Brownian Motion) noise\n\
             */\n\
            float ridgedFbmNoise(vec3 vIn, int octaves, float lacunarity, float gain, float offset)\n\
            {\n\
                vec3 v = vIn;\n\
                \n\
                float noiseSum = 0.0;\n\
                float amplitude = 1.0;\n\
                float amplitudeSum = 0.0;\n\
                float prev = 1.0;\n\
                float n;\n\
                \n\
                // make some ridged fbm noise\n\
                for( int i = 0; i < octaves; i++) {\n\
                    n = ridge(perlinNoise(v), offset);\n\
                    noiseSum += n * amplitude * prev;\n\
                    prev = n;\n\
                    amplitudeSum += amplitude;\n\
                    amplitude *= gain;\n\
                    v *= lacunarity;\n\
                }\n\
                \n\
                // get noiseSum in range -1..1    \n\
                return noiseSum / amplitudeSum;       \n\
            }\n\
            void main( void )\n\
            {\n\
                vec3 v = normalize(vertexPos);\n\
                float noiseSum = ridgedFbmNoise(noiseScale * v, octaves, lacunarity, gain, offset);\n\
            \n\
                // add a crazy amount of dithering noise\n\
                noiseSum += ridgedFbmNoise(v * 10000.0, octaves, lacunarity, gain, offset) * ditherAmt;\n\
            \n\
                // get noiseSum in range 0..1\n\
                noiseSum = (noiseSum*0.5) + 0.5;\n\
                \n\
                // apply shelf\n\
                noiseSum = max(0.0,noiseSum - shelfAmt);\n\
                \n\
                // scale whatever survives back into 0..1 range\n\
                noiseSum *= 1.0/(1.0-shelfAmt);\n\
            \n\
                // apply optional power function\n\
                noiseSum = pow(noiseSum,1.0/powerAmt);\n\
                \n\
                gl_FragColor.xyz = mix(outerColor, innerColor, noiseSum);\n\
                gl_FragColor.w = noiseSum;\n\
            }";

    /** Constructor
    */
    SpacescapeNoiseMaterial::SpacescapeNoiseMaterial(void):
        mMaterialName("SpacescapeNoiseMaterial")
    {
    }

    /** Destructor
    */
    SpacescapeNoiseMaterial::~SpacescapeNoiseMaterial(void)
    {
    }

    /** Get the noise material (create if necessary)
    @return the noise material
    */
    MaterialPtr SpacescapeNoiseMaterial::getMaterial()
    {
        MaterialPtr material = (MaterialPtr)MaterialManager::getSingletonPtr()->getByName(mMaterialName);
        if(material.isNull()) {
            GpuProgramParametersSharedPtr params;
            HighLevelGpuProgramPtr gpuProgram;
            Pass* pass;

            // create the permutation texture
            TextureManager::getSingleton().createManual(
                "spacescape_permutation_texture",
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                TEX_TYPE_2D,
                256,
                256,
                0,
                PF_BYTE_RGBA,
                TU_STATIC_WRITE_ONLY
            );

            // create the gradient texture
            TextureManager::getSingleton().createManual(
                    "spacescape_gradient_texture",
                    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                    TEX_TYPE_1D,
                    256,
                    1,
                    0,
                    PF_BYTE_RGB,
                    TU_STATIC_WRITE_ONLY
            );

            // create the noise material
            material = MaterialManager::getSingletonPtr()->create(
                    mMaterialName,
                    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
             );

            // create fbm technique first
            material->setReceiveShadows(false);
            material->getTechnique(0)->setName("fbm");

            pass = material->getTechnique(0)->getPass(0);
            pass->setCullingMode(CULL_ANTICLOCKWISE);
            pass->setLightingEnabled(false);
            pass->setDepthCheckEnabled(false);
            pass->setDepthWriteEnabled(false);

            // get the current render system type
            String renderSystem = Root::getSingleton().getRenderSystem()->getName();
            if(renderSystem == "Direct3D9 Rendering Subsystem") {
			    gpuProgram = HighLevelGpuProgramManager::getSingleton().
			          createProgram("spacescape_noise_hlsl_fbm_vp", 
			                        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
					  		        "hlsl", 
			                        GPT_VERTEX_PROGRAM);
	            gpuProgram->setSource(spacescape_noise_hlsl_fbm);
                gpuProgram->setParameter("entry_point", "main");
                gpuProgram->setParameter("target", "vs_1_1");
		        gpuProgram->load();
            }
            else {
                // load the vertex program
			    gpuProgram = HighLevelGpuProgramManager::getSingleton().
			          createProgram("spacescape_noise_glsl_vp", 
			                        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
					  		        "glsl", 
			                        GPT_VERTEX_PROGRAM);
	            gpuProgram->setSource(spacescape_noise_glsl_vp);
		        gpuProgram->load();

                // set the vertex program
                pass->setVertexProgram("spacescape_noise_glsl_vp");

                // load the fragment program
			    gpuProgram = HighLevelGpuProgramManager::getSingleton().
			          createProgram("spacescape_noise_glsl_fbm_fp",
			                        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
					  		        "glsl", 
			                        GPT_FRAGMENT_PROGRAM);
	            gpuProgram->setSource(spacescape_noise_glsl_fbm_fp);
		        gpuProgram->load();

                // set default fragment program params
                params = gpuProgram->getDefaultParameters();
                params->setNamedConstant("permTexture",(int)0);
                params->setNamedConstant("gradTexture",(int)1);

                // set the fragment program
                pass->setFragmentProgram("spacescape_noise_glsl_fbm_fp");
            }

            // create texture unit states for the two textures
            pass->createTextureUnitState("permTexture");
            pass->getTextureUnitState(0)->setTextureName("spacescape_permutation_texture");
            pass->getTextureUnitState(0)->setTextureFiltering(TFO_NONE);
            pass->getTextureUnitState(0)->setTextureAddressingMode(TextureUnitState::TAM_WRAP);
            
            pass->createTextureUnitState("gradTexture");
            pass->getTextureUnitState(1)->setTextureName("spacescape_gradient_texture");
            pass->getTextureUnitState(1)->setTextureFiltering(TFO_NONE);
            pass->getTextureUnitState(1)->setTextureAddressingMode(TextureUnitState::TAM_WRAP);

            // set vertex program params
            params = pass->getVertexProgramParameters();
            params->setNamedAutoConstant("worldViewProj",GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);

            // set fragment program params
            params = pass->getFragmentProgramParameters();
            params->setNamedConstant("ditherAmt",(float)0.0);
            params->setNamedConstant("gain",(float)0.1);
            params->setNamedConstant("innerColor",ColourValue(1.0,1.0,1.0));
            params->setNamedConstant("lacunarity",(float)2.0);
            params->setNamedConstant("noiseScale",(float)1.0);
            params->setNamedConstant("octaves",1);
            params->setNamedConstant("outerColor",ColourValue(0.0,0.0,0.0));
            params->setNamedConstant("powerAmt",(float)1.0);
            params->setNamedConstant("shelfAmt",(float)0.0);

            // create ridged technique
            material->createTechnique();
            material->getTechnique(1)->createPass();
            material->getTechnique(1)->setName("ridged");

            pass = material->getTechnique(1)->getPass(0);
            pass->setCullingMode(CULL_ANTICLOCKWISE);
            pass->setLightingEnabled(false);
            pass->setDepthCheckEnabled(false);
            pass->setDepthWriteEnabled(false);

            if(renderSystem == "Direct3D9 Rendering Subsystem") {
            }
            else {
                // set the vertex program
                pass->setVertexProgram("spacescape_noise_glsl_vp");

                // load the fragment program
			    gpuProgram = HighLevelGpuProgramManager::getSingleton().
			          createProgram("spacescape_noise_glsl_ridged_fp",
			                        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
					  		        "glsl", 
			                        GPT_FRAGMENT_PROGRAM);
	            gpuProgram->setSource(spacescape_noise_glsl_ridged_fp);
		        gpuProgram->load();

                // set default fragment program params
                params = gpuProgram->getDefaultParameters();
                params->setNamedConstant("permTexture",(int)0);
                params->setNamedConstant("gradTexture",(int)1);

                // set the fragment program
                pass->setFragmentProgram("spacescape_noise_glsl_ridged_fp");
            }

            // create texture unit states for the two textures
            pass->createTextureUnitState("permTexture");
            pass->getTextureUnitState(0)->setTextureName("spacescape_permutation_texture");
            pass->getTextureUnitState(0)->setTextureFiltering(TFO_NONE);
            pass->getTextureUnitState(0)->setTextureAddressingMode(TextureUnitState::TAM_WRAP);

            pass->createTextureUnitState("gradTexture");
            pass->getTextureUnitState(1)->setTextureName("spacescape_gradient_texture");
            pass->getTextureUnitState(1)->setTextureFiltering(TFO_NONE);
            pass->getTextureUnitState(1)->setTextureAddressingMode(TextureUnitState::TAM_WRAP);

            // set vertex program params
            params = pass->getVertexProgramParameters();
            params->setNamedAutoConstant("worldViewProj",GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);

            // set fragment program defaults
            params = pass->getFragmentProgramParameters();
            params->setNamedConstant("ditherAmt",(float)0.0);
            params->setNamedConstant("gain",(float)0.1);
            params->setNamedConstant("innerColor",ColourValue(1.0,1.0,1.0));
            params->setNamedConstant("lacunarity",(float)2.0);
            params->setNamedConstant("noiseScale",(float)1.0);
            params->setNamedConstant("octaves",1);
            params->setNamedConstant("outerColor",ColourValue(0.0,0.0,0.0));
            params->setNamedConstant("powerAmt",(float)1.0);
            params->setNamedConstant("shelfAmt",(float)0.0);
            params->setNamedConstant("offset",(float)1.0);

            material->load();
        }

        return material;
    }
}