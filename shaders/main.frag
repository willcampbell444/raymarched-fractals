#version 150

in vec2 screenPos;
in vec2 TexCoords;
in mat3 viewRot;

out vec4 outColor;

uniform vec3 cameraPos = vec3(0.0f, 0.0f, 8.0f);

const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;
const float WINDOW_CROSS = 1835.755975f;
const float RATIO = 16.0f/9.0f;

const float PI = 3.14159f;
const float FOV = 50.0f;
const float d = 1.0f/tan((PI/180.0f)*(FOV/2.0f));

const vec3 lightDir = vec3(-1.0f,-1.0f, 1.0f);

float SDF(vec3 P) {
    // return length(P)-1.0f;
    return length(mod(P+2.5f, 5.0f)-2.5f)-0.6f;
}

vec3 calcNormal(vec3 p, float t) {
    float h = 0.001f*t; // replace by an appropriate value
    const vec2 k = vec2(1,-1);
    return normalize( k.xyy*SDF( p + k.xyy*h ) +
                      k.yyx*SDF( p + k.yyx*h ) +
                      k.yxy*SDF( p + k.yxy*h ) +
                      k.xxx*SDF( p + k.xxx*h ) );
}

vec3 march(vec3 start, vec3 dir) {
    vec3 P = start;
    for (int i = 0; i < 200; i++) {
        P += dir*SDF(P);
    }
    return P;
}

vec3 color(vec3 start, vec3 dir) {
    vec3 P = march(start, dir);
    float t = length(P-cameraPos);
    if (SDF(P) < 0.0001f*t) {
        vec3 norm = calcNormal(P, t);
        return exp(-t*0.01)*vec3(0.3f, 0.8f, 1.0f)*(0.4+0.4*clamp(dot(norm, lightDir), 0.0, 1.0));
    }
    return vec3(0.0f);
}

void main() {
    vec3 viewDir = viewRot*vec3(screenPos.x*RATIO, screenPos.y, d);
	outColor = vec4(color(cameraPos, normalize(viewDir)), 1.0f);
}
