#version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Light{
    vec3 positon;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

#define NR_POINT_LIGHTS 1
uniform Light lights[NR_POINT_LIGHTS];
uniform Material material;
uniform vec3 viewPos;

vec3 CaculateLight(Light light, vec3 normal, vec3 lightDir, vec3 viewDir, vec3 reflectDir);

void main()
{
    vec3 resultColor = vec3(0.0f);
    vec3 norm = normalize(Normal);
    for(int i = 0; i< NR_POINT_LIGHTS; ++i){
        Light light = lights[i];
        vec3 lightDir = normalize(FragPos - light.positon);
        vec3 viewDir = normalize(FragPos - viewPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        resultColor += CaculateLight(light, Normal, lightDir, viewDir, reflectDir);
    }
    FragColor = vec4(resultColor, 1.0f);
}

vec3 CaculateLight(Light light, vec3 normal, vec3 lightDir, vec3 viewDir, vec3 reflectDir){
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return ambient+diffuse+specular;
}