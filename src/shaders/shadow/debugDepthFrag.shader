#version 330 core

out vec4 FragColor;

in vec2 TextureCoord;

uniform sampler2D depthMap;
uniform float near;
uniform float far;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main(){
    float depthValue = texture(depthMap, TextureCoord).r;
    FragColor = vec4(vec3(LinearizeDepth(depthValue) / far), 1.0);
}