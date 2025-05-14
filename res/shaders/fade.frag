#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 fragColor;

uniform vec3 color;
uniform float percent;

void main(){
    fragColor = vec4(color, percent);
}

