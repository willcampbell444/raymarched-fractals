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

const float roty = 0.5f;
const mat3 rott = mat3(cos(roty), 0, -sin(roty),
                       0,         1, 0,
                       sin(roty), 0, cos(roty));
const float scale=4.0f;
const float bailout=1000.f;
float SDF(vec3 P) {
    //vec3 q = abs(P) - 1.f;
    //return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
    P = 0.1f*P;
    //return length(P)-1.0f;
    // return length(mod(P+2.5f, 5.0f)-2.5f)-0.6f;
    P = mod(P+1.0f, 2.0f)-1.0f;

    float r=length(P);
    float i=0;
    for(i=0;i<5;i += 1) {
        //Folding... These are some of the symmetry planes of the tetrahedron
        //if(P.x+P.y<0){P = vec3(-P.y, -P.x, P.z);}
        //if(P.x+P.z<0){P = vec3(-P.z, P.y, -P.x);}
        //if(P.y+P.z<0){P = vec3(P.x, -P.z, -P.y);}
        //menger
        P = abs(P);
        if(P.x-P.y<0){P = vec3(P.y, P.x, P.z);}
        if(P.x-P.z<0){P = vec3(P.z, P.y, P.x);}
        if(P.y-P.z<0){P = vec3(P.x, P.z, P.y);}

        P.z-=0.5*(scale-1)/scale;
        P.z=-abs(-P.z);
        P.z+=0.5*(scale-1)/scale;

        P = rott*P;

        P.xy = scale*P.xy-(scale-1);
        P.z = scale*P.z;

        //P.xy = scale*P.xy-(scale-1.0f);
        //P.z = scale*P.z;
        //if (P.z > 0.5f*(scale-1.0f)) {P.z -= scale-1.0f;}
    }
    //r=length(P);
    //return (r-1.0f)*pow(scale, -i);//the estimated distance

    vec3 q = abs(P) - 2.f;
    return pow(scale, -i)*length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

const float th = 0.001f;
vec3 calcNormal(vec3 p, float t) {
    return vec3(0.0f);
    float h = 0.01f*t; // replace by an appropriate value
    const vec2 k = vec2(1,-1);
    return normalize( k.xyy*SDF( p + k.xyy*h ) +
                      k.yyx*SDF( p + k.yyx*h ) +
                      k.yxy*SDF( p + k.yxy*h ) +
                      k.xxx*SDF( p + k.xxx*h ) );
}

// return (intersection point, num_iters)
vec4 march(vec3 start, vec3 dir) {
    vec3 P = start;
    float d = SDF(P);
    int i = 0;
    for (i = 0; i < 500 && d > th; i++) {
        P += dir*d;
        d = SDF(P);
    }
    return vec4(P, i);
}

vec3 color(vec3 start, vec3 dir) {
    vec4 res = march(start, dir);
    vec3 P = res.xyz;
    float t = length(P-cameraPos);
    if (SDF(P) < th*t) {
        vec3 norm = calcNormal(P, t);
        //return vec3(0.3f, 0.8f, 1.0f)*(0.4+0.4*clamp(dot(norm, lightDir), 0.0, 1.0));
        // ssao
        return vec3(0.3f, 0.8f, 1.0f)*(exp(-res.w*0.01f)*0.8f+0.4*clamp(dot(norm, lightDir), 0.0, 1.0));
        // distance fog
        //exp(-t*0.01)
    }
    return vec3(0.0f);
}

void main() {
    vec3 viewDir = viewRot*vec3(screenPos.x*RATIO, screenPos.y, d);
	outColor = vec4(color(cameraPos, normalize(viewDir)), 1.0f);
}
