#version 330 core

out vec4 FragColor;

in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
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
    mat4 lightSpaceTrans;
    sampler2D shadowMap;
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
    mat4 lightSpaceTrans;
    sampler2D shadowMap;
};

struct Material{
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    bool hasDiffuseTex;
    bool hasSpecularTex;
    sampler2D diffuse;
    sampler2D specular;
};

#define DIRCECT_LIGHTS 3
#define POINT_LIGHTS 3
#define SPOT_LIGHTS 3
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
const float bias = 0.0005;

vec3 CaculateDirectLight(DirectLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler, float shadow);
vec3 CaculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler, float shadow);
vec3 CaculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler, float shadow);
float shadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap);

void main()
{
    vec3 resultColor = vec3(0.0f);
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 diffuseSampler;
    if(materials[0].hasDiffuseTex){
        vec4 diffuseTex = texture(materials[0].diffuse, fs_in.TexCoords);
        if(diffuseTex.a<0.5){
            discard;
        }
        diffuseSampler = vec3(diffuseTex);
    }else{
        diffuseSampler = materials[0].diffuseColor;
    }
    vec3 specularSampler;
    if(materials[0].hasSpecularTex){
        specularSampler = vec3(texture(materials[0].specular, fs_in.TexCoords));
    }else{
        specularSampler = materials[0].specularColor;
    }

    vec4 FragPosLightSpace;
    float shadow;
    for(int i = 0; i< dirNum; ++i){
        FragPosLightSpace = directLights[i].lightSpaceTrans * vec4(fs_in.FragPos, 1.0);
        shadow = shadowCalculation(FragPosLightSpace, directLights[i].shadowMap);
        resultColor += CaculateDirectLight(directLights[i], norm, viewDir, diffuseSampler, specularSampler, shadow);
    }
    for(int i = 0; i< pointNum; ++i){
        FragPosLightSpace = pointLights[i].lightSpaceTrans * vec4(fs_in.FragPos, 1.0);
        shadow = shadowCalculation(FragPosLightSpace, pointLights[i].shadowMap);
        resultColor += CaculatePointLight(pointLights[i], norm, viewDir, diffuseSampler, specularSampler, shadow);
    }
    for(int i = 0; i< spotNum; ++i){
        FragPosLightSpace = spotLights[i].lightSpaceTrans * vec4(fs_in.FragPos, 1.0);
        shadow = shadowCalculation(FragPosLightSpace, spotLights[i].shadowMap);
        resultColor += CaculateSpotLight(spotLights[i], norm, viewDir, diffuseSampler, specularSampler, shadow);
    }

    resultColor = pow(resultColor, vec3(1.0f/gamma));
    FragColor = vec4(resultColor, 1.0f);
}

vec3 CaculateDirectLight(DirectLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler, float shadow){
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
    return ambient + (1.0 - shadow)*(diffuse + specular);
}

vec3 CaculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler, float shadow){
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
    return ambient + (1.0 - shadow) * (diffuse + specular);
}

vec3 CaculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 diffuseSampler, vec3 specularSampler, float shadow){
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
    return ambient + (1.0 - shadow) * (diffuse + specular);
}

float shadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap){
    vec3 projCoord = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoord = projCoord * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoord.xy).r;
    return projCoord.z > closestDepth ? 1.0 : 0.0;
}