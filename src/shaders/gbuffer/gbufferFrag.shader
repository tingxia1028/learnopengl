#version 330 core

layout (location=0) out vec3 gPosition;
layout (location=1) out vec3 gNormal;
layout (location=2) out vec3 gDiffuse;
layout (location=3) out vec4 gSpecularShininess;

in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;


struct Material{
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    bool hasDiffuseTex;
    bool hasSpecularTex;
    bool hasNormalMap;
    bool hasDepthMap;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D depth;
};

#define MATERIALS 1
uniform vec3 viewPos;
uniform Material materials[MATERIALS];

const float heightScale = 0.1;
const float minLayers = 8;
const float maxLayers = 32;

vec2 ParallaxMapping(Material material, vec2 texCoords, vec3 viewDir);

void main()
{
    gPosition = fs_in.FragPos;
    vec3 norm = fs_in.Normal;
    if(materials[0].hasNormalMap){
        norm = texture(materials[0].normal, fs_in.TexCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        norm = normalize(fs_in.TBN * norm);
    }else{
         norm = normalize(fs_in.Normal);
    }
    gNormal = norm;
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 tangentViewDir = normalize(fs_in.TBN * viewPos - fs_in.TBN * fs_in.FragPos);
    vec2 texCoord;
    if(materials[0].hasDepthMap){
        texCoord = ParallaxMapping(materials[0], fs_in.TexCoords, tangentViewDir);
        if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
            discard;
    }else{
        texCoord = fs_in.TexCoords;
    }

    if(materials[0].hasDiffuseTex){
        gDiffuse = vec3(texture(materials[0].diffuse, texCoord));
    }else{
        gDiffuse = materials[0].diffuseColor;
    }
    if(materials[0].hasSpecularTex){
        gSpecularShininess.rgb = vec3(texture(materials[0].specular, texCoord));
    }else{
        gSpecularShininess.rgb = materials[0].specularColor;
    }
    gSpecularShininess.a = materials[0].shininess;
}

vec2 ParallaxMapping(Material material, vec2 texCoords, vec3 viewDir){
    // linear interpolation: x*(1-level)+y*level
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 p = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = p / numLayers;

    // init
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(material.depth, currentTexCoords).r;
    while(currentLayerDepth < currentDepthMapValue){
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(material.depth, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    // interpolation
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(material.depth, prevTexCoords).r - currentLayerDepth + layerDepth;
    float weight = afterDepth / (afterDepth - beforeDepth);
    return prevTexCoords * weight + currentTexCoords * (1.0-weight);
}