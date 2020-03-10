#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT{
   vec3 Normal;
}vs_out;

void main(){
    vs_out.Normal = normalize(vec3(projection * vec4(mat3(transpose(inverse(view * model))) * aNormal, 0.0)));
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}