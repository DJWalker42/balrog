#version 420

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 ReflectDir;

uniform bool DrawSkyBox;
uniform vec3 WorldCameraPosition;

uniform mat4 model;
uniform mat4 normMat;
uniform mat4 mvp;

void main()
{
    if( DrawSkyBox ) {
        ReflectDir = position;
    } else {
        vec3 worldPos = vec3( model * vec4(position,1.0) );
        vec3 worldNorm = vec3(normMat * vec4(normal, 0.0));
        vec3 worldView = normalize( WorldCameraPosition - worldPos );

        //ReflectDir = reflect(-worldView, worldNorm );
		ReflectDir = position;
    }

    gl_Position = mvp * vec4(position,1.0);
}