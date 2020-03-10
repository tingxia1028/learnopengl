#version 330 core

layout (location=0) in vec3 aPos;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec3 textureDir;

void main(){
    textureDir = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0f);
    gl_Position = pos.xyww;
}