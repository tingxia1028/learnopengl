#version 330 core

in vec3 textureDir;

uniform samplerCube cubemap;

out vec4 FragColor;

void main(){
    FragColor = texture(cubemap, textureDir);
}