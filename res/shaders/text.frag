#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 fragColor;

uniform vec3 color;
uniform sampler2D texture2D;

void main(void) {
    vec4 c = texture(texture2D, TexCoord);
    fragColor = vec4(color, length(c.rgb) > 0.0 ? 1.0 : 0.0);
}