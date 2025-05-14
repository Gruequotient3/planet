#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

in vec3 Pos;

out vec4 fragColor;

uniform vec3 cameraPos;
uniform vec3 color;

void main(){
    vec3 camPos = normalize(cameraPos);
    vec3 pos = normalize(Pos);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(pos - camPos);
    float dotprod = dot(viewDir, norm);
    
    vec4 fragmentColor = vec4(color, 0.0);
    if (dotprod > 0.1 && dotprod < 1.0){
        fragmentColor.a = ((dotprod > 0.5 ? 0.5 : dotprod) - 0.35) / (0.5 - 0.35);

    }
    fragColor = fragmentColor;
}