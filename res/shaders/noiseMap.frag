#version 460 core

in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D ourTexture;
uniform int mode;

vec3 regionsColor[] = {
    vec3(0.2051, 0.5, 0.4973),
    vec3(0.3656, 0.783, 0.6877),
    vec3(0.945, 0.98, 0.71),
    vec3(0.757, 0.898, 0.333),
    vec3(0.486, 0.8, 0.404),
    vec3(0.8, 0.8, 0.8),
    vec3(1.0, 1.0, 1.0)
};


float height[] = {
    0.3,
    0.55,
    0.57,
    0.63,
    0.7,
    0.77,
    1.0
};


float inverseLerp(float a, float b, float t);
vec3 getGradientColor(float value);
vec3 getRaycastGradientColor(float value);


void main(){
    vec4 texColor = texture(ourTexture, TexCoord);
    float noiseValue = texColor.x;
    vec3 finalColor;
    switch (mode){
        case 0:
            fragColor = texColor;
            break;

        case 1:
            for (int i = 0; i < regionsColor.length(); ++i){
                if (noiseValue < height[i]){
                    finalColor = regionsColor[i];
                    break;
                }

            }
            fragColor = vec4(finalColor, 1.0);
            break;

        case 2:
            finalColor = getGradientColor(noiseValue);
            fragColor = vec4(finalColor, 1.0);
            break;

    }
}


float inverseLerp(float a, float b, float t){
    return (t - a) / (b - a);
}

vec3 getGradientColor(float value){
    vec3 finalColor;
    for (int i = 0; i < regionsColor.length(); ++i){
        if (value < height[i]){
            float percentRegion = inverseLerp(
                i == 0 ? height[i] : height[i-1],
                i == regionsColor.length() - 1 ? height[i] : height[i+1],
                value);

            finalColor = mix(regionsColor[i], i == regionsColor.length() - 1 ?
                             regionsColor[i] : regionsColor[i+1],
                             percentRegion);
            break;
        }
    }
    return finalColor;
}

