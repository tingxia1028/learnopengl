#version 330 core

in vec2 TextureCoords;

uniform bool isHdr;
uniform bool isBloom;
uniform float exposure;
uniform sampler2D deferredTex;
uniform sampler2D bloomBlur;

out vec4 FragColor;

void main(){
    vec3 color = texture(deferredTex, TextureCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TextureCoords).rgb;
    if(isBloom){
        color += bloomColor;
    }
    if(isHdr){
        vec3 ldrColor = vec3(1.0f) - exp(- color * exposure);
        FragColor = vec4(ldrColor, 1.0f);
    }else{
        FragColor = vec4(color, 1.0f);
    }
}