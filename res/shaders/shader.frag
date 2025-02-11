#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 fragColor;

uniform vec3 color;

uniform sampler2D ourTexture;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main(){
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    fragColor = vec4(color, 1.0) * vec4(diffuse + ambient, 1.0);
}
