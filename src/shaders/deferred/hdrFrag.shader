#version 330 core

in vec2 TextureCoords;

uniform bool isHdr;
uniform float exposure;
uniform sampler2D deferredTex;

out vec4 FragColor;

void main(){
    vec3 color = texture(deferredTex, TextureCoords).rgb;
    if(isHdr){
        vec3 ldrColor = vec3(1.0f) - exp(- color * exposure);
        FragColor = vec4(ldrColor, 1.0f);
    }else{
        FragColor = vec4(color, 1.0f);
    }
}