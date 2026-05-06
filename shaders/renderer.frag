#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 VertColor;

uniform sampler2D texture_diffuse;

void main() {
    vec4 texColor = texture(texture_diffuse, TexCoord);

    FragColor = texColor * VertColor;
}