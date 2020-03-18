#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far;

void main(){
    float distance = length(FragPos.xyz - lightPos);
    distance = distance / far;
    gl_FragDepth = distance;
}