#version 330 core

layout (location=0) out vec4 FragColor;
layout (location=1) out vec4 BrightColor;

in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

struct DirectLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    mat4 lightSpaceTrans;
    sampler2D shadowMap;
};

struct PointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float farPlane;
//    samplerCube shadowMap;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutoff;
    float outCutoff;
    float farPlane;
//    samplerCube shadowMap;
};

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

#define DIRCECT_LIGHTS 2
#define POINT_LIGHTS 2
#define SPOT_LIGHTS 2
#define MATERIALS 1
uniform int dirNum;
uniform int pointNum;
uniform int spotNum;
uniform DirectLight directLights[DIRCECT_LIGHTS];
uniform PointLight pointLights[POINT_LIGHTS];
uniform SpotLight spotLights[SPOT_LIGHTS];
// only one material present, may extend to mix/blend later
uniform Material materials[MATERIALS];
uniform vec3 viewPos;

const float gamma = 2.2;
const float pointShadowBias = 0.15;
const int samples = 20;
const vec3 sampleOffsetDirections[20] = vec3[]
(
vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);
const float heightScale = 0.1;
const float minLayers = 8;
const float maxLayers = 32;

vec3 CaculateDirectLight(DirectLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler, vec4 FragPosLightSpace);
vec3 CaculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler);
vec3 CaculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler);
float directShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap, float bias);
float pointShadowCalculation(vec3 fragPos, vec3 lightPos, float farPlane, samplerCube shadowMap);
vec2 ParallaxMapping(Material material, vec2 texCoords, vec3 viewDir);

void main()
{
    vec3 resultColor = vec3(0.0f);
    vec3 norm = fs_in.Normal;
    if(materials[0].hasNormalMap){
        norm = texture(materials[0].normal, fs_in.TexCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        norm = normalize(fs_in.TBN * norm);
    }else{
         norm = normalize(fs_in.Normal);
    }
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

    vec3 diffuseSampler;
    if(materials[0].hasDiffuseTex){
        vec4 diffuseTex = texture(materials[0].diffuse, texCoord);
        diffuseSampler = vec3(diffuseTex);
    }else{
        diffuseSampler = materials[0].diffuseColor;
    }
    vec3 specularSampler;
    if(materials[0].hasSpecularTex){
        specularSampler = vec3(texture(materials[0].specular, texCoord));
    }else{
        specularSampler = materials[0].specularColor;
    }

    for(int i = 0; i< dirNum; ++i){
        vec4 FragPosLightSpace = directLights[i].lightSpaceTrans * vec4(fs_in.FragPos, 1.0);
        resultColor += CaculateDirectLight(directLights[i], norm, viewDir, diffuseSampler, specularSampler, FragPosLightSpace);
    }
    for(int i = 0; i< pointNum; ++i){
        resultColor += CaculatePointLight(pointLights[i], norm, viewDir, diffuseSampler, specularSampler);
    }
    for(int i = 0; i< spotNum; ++i){
        resultColor += CaculateSpotLight(spotLights[i], norm, viewDir, diffuseSampler, specularSampler);
    }

    resultColor = pow(resultColor, vec3(1.0f/gamma));
    FragColor = vec4(resultColor, 1.0f);

    // bloom
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0){
        BrightColor = vec4(FragColor.rgb, 1.0f);
    }else{
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

vec3 CaculateDirectLight(DirectLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler, vec4 FragPosLightSpace){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfDir = normalize(lightDir+viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), materials[0].shininess);
    // combine results
    vec3 ambient = light.ambient * diffuseSampler;
    vec3 diffuse = light.diffuse * diff * diffuseSampler;
    vec3 specular = light.specular * spec * specularSampler;
    // shadow
    float bias = max(0.05 * (1.0 - diff), 0.005);
    float shadow = directShadowCalculation(FragPosLightSpace, light.shadowMap, bias);
    return ambient + (1.0 - shadow)*(diffuse + specular);
}

vec3 CaculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler){
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfDir = normalize(lightDir+viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), materials[0].shininess);
    // combine results
    vec3 ambient = light.ambient * diffuseSampler;
    vec3 diffuse = light.diffuse * diff * diffuseSampler;
    vec3 specular = light.specular * spec * specularSampler;
    // attenuation
    float distance    = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    // shadow
//    float shadow = pointShadowCalculation(fs_in.FragPos, light.position, light.farPlane, light.shadowMap);
    float shadow = 0.0;
    return ambient + (1.0 - shadow) * (diffuse + specular);
}

vec3 CaculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler){
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfDir = normalize(lightDir+viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), materials[0].shininess);
    // combine results
    vec3 ambient = light.ambient * diffuseSampler;
    vec3 diffuse = light.diffuse * diff * diffuseSampler;
    vec3 specular = light.specular * spec * specularSampler;
    // attenuation
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outCutoff;
    float intensity = clamp((theta - light.outCutoff) / epsilon, 0.0, 1.0);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    // shadow
//    float shadow = pointShadowCalculation(fs_in.FragPos, light.position, light.farPlane, light.shadowMap);
    float shadow = 0.0;
    return ambient + (1.0 - shadow) * (diffuse + specular);
}

float directShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap, float bias){
    vec3 projCoord = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoord = projCoord * 0.5 + 0.5;
    if(projCoord.z > 1.0f){
        return 0.0f;
    }

    float shadow = 0.0;
    vec2 texelSize = 1.0/textureSize(shadowMap, 0);
    for(int x = -1; x<=1; ++x){
        for(int y = -1; y<=1;++y){
            float pcfDepth = texture(shadowMap, projCoord.xy+vec2(x,y)*texelSize).r;
            shadow+=(projCoord.z - bias > pcfDepth ? 1.0 : 0.0);
        }
    }
    return shadow/9.0f;
}

float pointShadowCalculation(vec3 fragPos, vec3 lightPos, float farPlane, samplerCube shadowMap){
    float shadow = 0.0;
    float viewDistance = length(viewPos-fragPos);
    float diskRadius = (1.0+(viewDistance/farPlane))/25.0;
    vec3 direction = fragPos - lightPos;
    float currentDepth = length(direction);
    for(int i=0; i<samples; ++i){
        float shadowDepth = texture(shadowMap, direction+sampleOffsetDirections[i]*diskRadius).r;
        shadowDepth *= farPlane;
        if(currentDepth-pointShadowBias>shadowDepth){
            shadow+=1.0;
        }
    }

    return shadow/float(samples);
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