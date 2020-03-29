#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 TextureCoord;

uniform mat4 model;
uniform mat4 lightSpaceTrans;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main(){
    vec4 posLightSpace = lightSpaceTrans * model * vec4(aPos, 1.0f);
    vec3 projCoord = (posLightSpace.xyz / posLightSpace.w) * 0.5 + 0.5;
    TextureCoord = projCoord.xy;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}