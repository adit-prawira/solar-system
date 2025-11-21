#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
    // World-space position
    FragPos = vec3(model * vec4(position, 1.0));

    // Transform normals correctly (remove translation)
    Normal = mat3(transpose(inverse(model))) * normal;

    // Final vertex position in clip space
    gl_Position = projection * view * vec4(FragPos, 1.0);

    // Pass through texture coordinates
    TexCoord = texture;
}
