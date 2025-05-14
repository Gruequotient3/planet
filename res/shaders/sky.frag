#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D tex;

void main(){
    vec4 color = texture(tex, TexCoord);
    if ((color.x + color.y + color.z) / 3.0 < 0.5) color = vec4(0.0, 0.0, 0.0, 1.0);
    fragColor = color;
}

