#version 410 core

in vec3 position;
in vec2 texCoord;
in vec3 normal;


out vec3 fragPos;
out vec2 fragTex;
out vec3 fragNorm;


uniform mat4 modelView;
uniform mat3 normMat;
uniform mat4 mvp;

void main() {
	fragTex = texCoord;
	fragNorm = normalize(normMat * normal);
	fragPos = vec3( modelView * vec4(position, 1.0));
	gl_Position = mvp * vec4(position, 1.0);
}