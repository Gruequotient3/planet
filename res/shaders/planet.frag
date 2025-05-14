#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

in vec3 Pos;

out vec4 fragColor;

uniform float time;

uniform vec3 colors[16];
uniform float heights[16];
uniform int nbColor;

uniform float minValue;
uniform float maxValue;

uniform bool isSun;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float sunLightStrength; 
uniform float sunStrength;

float lerp(float a, float b, float t);
float inverseLerp(float a, float b, float t);

void main(){
    int i;

    float ambientStrength = isSun ? 0.5 : sunStrength;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos) *
        (isSun ? sunLightStrength + lerp(-0.5, -1.0, cos(time * 1.25)) : 1.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float percent = inverseLerp(minValue, maxValue, length(Pos));
    for (i = 0; i < nbColor-1; ++i){
        if (percent < heights[i]){
            break;
        }
    }
    float percentColor = inverseLerp(heights[i-1 < 0 ? 0 : i-1], heights[i], percent);
    vec3 color = mix(colors[i-1 < 0 ? 0 : i-1], colors[i], percentColor);
    vec3 result = color * (ambient + diffuse);
    fragColor = vec4(result, 1.0);
}

float lerp(float a, float b, float t){
    float value = a + t * (b - a);
    return t < 0.0 ? a : t > 1.0 ? b : value;
}

float inverseLerp(float a, float b, float t){
    float value  = (t - a) / (b - a);
    return value < 0.0 ? 0.0 : value > 1.0 ? 1.0 : value;
}

