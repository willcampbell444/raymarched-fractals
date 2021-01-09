#version 150

in vec3 position;
in vec2 texPos;


out vec2 screenPos;
out vec2 TexCoords;
out mat3 viewRot;

uniform vec3 viewDir = vec3(0.0f, 0.0f, -1.0f);
const vec3 up = vec3(0.0f, 1.0f, 0.0f);

void main() {
    vec3 left = normalize(cross(viewDir, up));
    vec3 newUp = normalize(cross(left, viewDir));
    viewRot = mat3(left, newUp, viewDir);

	TexCoords = texPos;
    screenPos = position.xy;
	gl_Position = vec4(position, 1);
}
