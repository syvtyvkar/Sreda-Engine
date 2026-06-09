// MSDF text shader

#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

out vec4 v_Color;
out vec2 v_TexCoord;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjection;
};

void main() {
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_FontAtlas;

void main() {
    float alpha = texture(u_FontAtlas, v_TexCoord).r;
    color = vec4(v_Color.rgb, v_Color.a * alpha);
}