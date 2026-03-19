#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D u_Texture;  // ← Сэмплер текстуры

void main() {
    FragColor = texture(u_Texture, TexCoord);
}