/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//--------------------------------------------------------------------------------
// Teapot Renderer.h
// Renderer for teapots
//--------------------------------------------------------------------------------
#ifndef _TEAPOTRENDERER_H
#define _TEAPOTRENDERER_H

//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <errno.h>

#include <vector>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include <cpu-features.h>

#define CLASS_NAME "android/app/NativeActivity"
#define APPLICATION_CLASS_NAME "com/sample/teapot/TeapotApplication"

#include "NDKHelper.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

struct TEAPOT_VERTEX
{
    float pos[3];
    float normal[3];
};

enum SHADER_ATTRIBUTES
{
    ATTRIB_VERTEX, ATTRIB_NORMAL, ATTRIB_UV,
};

struct SHADER_PARAMS
{
    GLuint program_;
    GLuint light0_;
    GLuint material_diffuse_;
    GLuint material_ambient_;
    GLuint material_specular_;

    GLuint matrix_projection_;
    GLuint matrix_view_;
};

struct TEAPOT_MATERIALS
{
    float diffuse_color[3];
    float specular_color[4];
    float ambient_color[3];
};

class TeapotRenderer
{
    int32_t num_indices_;
    int32_t num_vertices_;
    GLuint ibo_;
    GLuint vbo_;

    SHADER_PARAMS shader_param_;
    bool LoadShaders( SHADER_PARAMS* params);
    
     const char* returnVertexSrc();
    const char* returnFramentSrc();

    ndk_helper::Mat4 mat_projection_;
    ndk_helper::Mat4 mat_view_;
    ndk_helper::Mat4 mat_model_;

    ndk_helper::TapCamera* camera_;
    
    const char* vert_shader_src_p = "#define USE_PHONG (1)\n"
            "\n"
            "attribute highp vec3    myVertex;\n"
            "attribute highp vec3    myNormal;\n"
            "attribute mediump vec2  myUV;\n"
            "attribute mediump vec4  myBone;\n"
            "\n"
            "varying mediump vec2    texCoord;\n"
            "varying lowp    vec4    colorDiffuse;\n"
            "\n"
            "#if USE_PHONG\n"
            "varying mediump vec3 position;\n"
            "varying mediump vec3 normal;\n"
            "#else\n"
            "varying lowp    vec4    colorSpecular;\n"
            "#endif\n"
            "\n"
            "uniform highp mat4      uMVMatrix;\n"
            "uniform highp mat4      uPMatrix;\n"
            "\n"
            "uniform highp vec3      vLight0;\n"
            "\n"
            "uniform lowp vec4       vMaterialDiffuse;\n"
            "uniform lowp vec3       vMaterialAmbient;\n"
            "uniform lowp vec4       vMaterialSpecular;\n"
            "\n"
            "void main(void)\n"
            "{\n"
            "    highp vec4 p = vec4(myVertex,1);\n"
            "    gl_Position = uPMatrix * p;\n"
            "\n"
            "    texCoord = myUV;\n"
            "\n"
            "    highp vec3 worldNormal = vec3(mat3(uMVMatrix[0].xyz, uMVMatrix[1].xyz, uMVMatrix[2].xyz) * myNormal);\n"
            "    highp vec3 ecPosition = p.xyz;\n"
            "\n"
            "    colorDiffuse = dot( worldNormal, normalize(-vLight0+ecPosition) ) * vMaterialDiffuse  + vec4( vMaterialAmbient, 1 );\n"
            "\n"
            "#if USE_PHONG\n"
            "    normal = worldNormal;\n"
            "    position = ecPosition;\n"
            "#else\n"
            "    highp vec3 halfVector = normalize(ecPosition - vLight0);\n"
            "\n"
            "    highp float NdotH = max(-dot(worldNormal, halfVector), 0.0);\n"
            "    float fPower = vMaterialSpecular.w;\n"
            "    highp float specular = min( pow(NdotH, fPower), 1.0);\n"
            "    colorSpecular = vec4( vMaterialSpecular.xyz * specular, 1 );\n"
            "#endif\n"
            "}";

    const char* frag_shader_src_p = "#define USE_PHONG (1)\n"
            "\n"
            "uniform lowp vec3       vMaterialAmbient;\n"
            "uniform mediump vec4       vMaterialSpecular;\n"
            "\n"
            "varying lowp vec4 colorDiffuse;\n"
            "\n"
            "#if USE_PHONG\n"
            "uniform highp vec3      vLight0;\n"
            "varying mediump vec3 position;\n"
            "varying mediump vec3 normal;\n"
            "#else\n"
            "varying lowp vec4 colorSpecular;\n"
            "#endif\n"
            "\n"
            "void main()\n"
            "{\n"
            "#if USE_PHONG\n"
            "    mediump vec3 halfVector = normalize(-vLight0 + position);\n"
            "    mediump float NdotH = max(dot(normalize(normal), halfVector), 0.0);\n"
            "    mediump float fPower = vMaterialSpecular.w;\n"
            "    mediump float specular = pow(NdotH, fPower);\n"
            "\n"
            "    lowp vec4 colorSpecular = vec4( vMaterialSpecular.xyz * specular, 1 );\n"
            "    gl_FragColor = colorDiffuse + colorSpecular;\n"
            "#else\n"
            "    gl_FragColor = colorDiffuse + colorSpecular;\n"
            "#endif\n"
            "}";

    
public:
    TeapotRenderer();
    virtual ~TeapotRenderer();
    void Init();
    void Render();
    void Update( float dTime );
    bool Bind( ndk_helper::TapCamera* camera );
    void Unload();
    void UpdateViewport();
};

#endif

