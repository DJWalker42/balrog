//#version 420 core
#version 440 core

in vec3 fragPos;
in vec3 fragNorm;
in vec2 fragTex;

layout(binding=0) uniform sampler2D BaseMap;

out vec4 outColour;

struct LightInfo 
{
	vec4 Position;
	vec3 Intensity;
};
uniform LightInfo Light;

struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float shine;
};
uniform MaterialInfo Material;

void phongModel (vec3 pos, vec3 norm, out vec3 ambDiff, out vec3 spec)
{
	vec3 s = normalize(vec3(Light.Position) - pos);	
	float sDotN = max( dot(s,norm), 0.0 );
	spec = vec3(0.0);
	if(sDotN > 0.0) {
		vec3 v = normalize(-pos.xyz);
		//phong model
		//vec3 r = reflect(-s, norm);
		//spec = Light.Intensity * Material.Ks * pow( max( dot(r,v), 0.0 ), Material.shine );

		//blinn-phong alternative
		vec3 halfDir = normalize(s + v);
		float specAngle = max(dot(halfDir, norm),0.0);
		spec = Light.Intensity * Material.Ks * pow(specAngle, Material.shine);
	}		
	vec3 amb = Light.Intensity * Material.Ka;	
	vec3 diff = Light.Intensity * Material.Kd * sDotN;
	ambDiff = amb + diff;
}

void main() 
{
	vec3 ambDiff, spec;
	vec4 BaseColour = texture(BaseMap, fragTex);
	phongModel(fragPos, fragNorm, ambDiff, spec);
	outColour = vec4(ambDiff, 1.0) * BaseColour + vec4(spec, 1.0);
}