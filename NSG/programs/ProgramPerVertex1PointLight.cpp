/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#include "ProgramPerVertex1PointLight.h"
#include "Types.h"
#include "ResourceMemory.h"

static const char* vShader = STRINGIFY(void main()
{
    vec3 normalDirection = normalize(u_model_inv_transp * a_normal);
    vec3 worldPos = GetWorldPos(u_m);
    vec3 viewDirection = normalize(vec3(u_v_inv * vec4(0.0, 0.0, 0.0, 1.0)) - worldPos);
    vec3 ambientLighting = vec3(u_scene_ambient) * vec3(u_material.ambient);
    vec3 vertexToLightSource = u_light0.position - worldPos;
    float distance = length(vertexToLightSource);
    vec3 lightDirection = normalize(vertexToLightSource);
    float attenuation = 1.0 / (u_light0.constantAttenuation + u_light0.linearAttenuation * distance + u_light0.quadraticAttenuation
                               * distance * distance);
    float ndotl = dot(normalDirection, lightDirection);
    vec3 diffuseReflection = max(0.0, ndotl) * attenuation * vec3(u_light0.diffuse) * vec3(u_material.diffuse);
    vec3 specularReflection = min(1.0, max(0.0, ndotl)) * attenuation * vec3(u_light0.specular) * vec3(u_material.specular)
                              * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), u_material.shininess);

    v_color = a_color * vec4(ambientLighting + diffuseReflection + specularReflection, u_material.diffuse.w);
    gl_Position = u_mvp * a_position;
    v_texcoord = a_texcoord;
});

static const char* fShader = STRINGIFY(void main()
{
    gl_FragColor = v_color * texture2D(u_texture0, v_texcoord);
});

namespace NSG
{
    ProgramPerVertex1PointLight::ProgramPerVertex1PointLight()
        : Program("ProgramPerVertex1PointLight", PResourceMemory(new ResourceMemory(vShader)), PResourceMemory(new ResourceMemory(fShader)))
    {
    }

    ProgramPerVertex1PointLight::~ProgramPerVertex1PointLight()
    {

    }
}
