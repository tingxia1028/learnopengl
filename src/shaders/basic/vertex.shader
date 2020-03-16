#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

uniform mat4 model;
uniform mat4 lightSpaceTrans;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
} vs_out;


void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
    vs_out.Normal = mat3(transpose(inverse(model)))* aNormal;
    vs_out.TexCoords = vec2(aTexture);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}