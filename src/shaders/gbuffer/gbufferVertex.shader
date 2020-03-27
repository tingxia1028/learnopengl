#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;


void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vs_out.TexCoords = vec2(aTexture);
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    vs_out.Normal = N;
    T = normalize(T - dot(T,N)*N);
    vec3 B = cross(N, T);
    vs_out.TBN = mat3(T, B, N);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}