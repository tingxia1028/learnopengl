#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT{
   vec3 Normal;
} gs_in[];

const float MAGNITUDE = 0.02;

void generateNormal(int index){
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].Normal, 0.0f) * MAGNITUDE;
    EmitVertex();

    EndPrimitive();
}

void main(){
    generateNormal(0);
    generateNormal(1);
    generateNormal(2);
}
