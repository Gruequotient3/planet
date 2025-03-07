#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 fragColor;

uniform vec3 colors[16];
uniform float heights[16];
uniform int nbColor;

uniform float minValue;
uniform float maxValue;

uniform vec3 lightPos;
uniform vec3 lightColor;

float inverseLerp(float a, float b, float t);

void main(){
    vec3 color = vec3(0.0, 0.0, 0.0);
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float percent = inverseLerp(minValue, maxValue, length(FragPos));
    for (int i = 0; i < nbColor; ++i){
        if (percent <= heights[i]){
            color = colors[i];
            break;
        }
    }
    fragColor = vec4(color, 1.0) * vec4(diffuse + ambient, 1.0);
}

float inverseLerp(float a, float b, float t){
    return (t-a) / (b-a);
}

