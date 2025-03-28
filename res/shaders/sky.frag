#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D ourTexture;

void main(){
    fragColor = vec4(1.0) * texture(ourTexture, TexCoord);
}
