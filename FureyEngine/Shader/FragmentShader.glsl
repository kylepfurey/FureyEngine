// .glsl
// Basic Fragment Shader Script
// by Kyle Furey

#version 330 core

// The texture to read from.
uniform sampler2D texture1;

// Input texture coordinate.
in vec2 TexCoord;

// Output color.
out vec4 color;

// Executes the shader.
void main() {
    color = texture(texture1, TexCoord);
}
