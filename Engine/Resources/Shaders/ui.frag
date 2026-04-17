#version 330 core

in vec2 vUV;
in vec4 vColor;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform int useTexture;

void main()
{
    vec4 col = vColor;

    if (useTexture == 1)
        col *= texture(uTexture, vUV);

    FragColor = col;
}