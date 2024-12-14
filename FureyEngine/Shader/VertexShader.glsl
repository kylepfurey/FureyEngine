// .glsl
// Basic Vertex Shader Script
// by Kyle Furey

#version 330 core

// The model's transform matrix.
uniform mat4 model;

// The projection matrix.
uniform mat4 projection;

// The position of the vertex.
layout(location = 0) in vec3 position;

// The texture coordinate of the vertex.
layout(location = 1) in vec2 texCoord;

// Output texture coordinate.
out vec2 TexCoord;

// Executes the shader.
void main() {
    gl_Position = projection * model * vec4(position, 1);
    TexCoord = texCoord;
}
