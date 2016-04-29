#ifndef BASIC_LIGHTING_HPP
#define BASIC_LIGHTING_HPP

#include "GLSLProgram.hpp"
#include <glm/glm.hpp>

class BaseLight{
public:
	BaseLight();
	BaseLight(const glm::vec3& colour, float ambientI, float diffuseI); 

	glm::vec3 getColour() const {return m_colour;}
	float getAmbient() const {return m_ambient;}
	float getDiffuse() const {return m_diffuse;}

	void setColour(const glm::vec3& value) {m_colour = value;}
	void setAmbient(float value){m_ambient = value;}
	void setDiffuse(float value){m_diffuse = value;}

protected:
	glm::vec3 m_colour;
	float m_ambient;
	float m_diffuse;

private:

};

class DirectionalLight : public BaseLight{
public:
	DirectionalLight();
	DirectionalLight(	const glm::vec3& direction, 
						const glm::vec3& colour = glm::vec3(0.f), 
						float ambientI = 0.f, 
						float diffuseI = 0.f);

	glm::vec3 getDirection() const {return m_direction;}
	void setDirection(const glm::vec3& value) {m_direction = value;}

protected:

private:
	glm::vec3 m_direction;
};

class PointLight : public BaseLight{
public:
	PointLight();
	PointLight(	const glm::vec3& position,
				const glm::vec3& attenuation = glm::vec3(1.f, 0.f, 0.f),
				const glm::vec3& colour = glm::vec3(0.f),
				float ambientI = 0.f,
				float diffuseI = 0.f);

	glm::vec3 getPosition() const {return m_position;}
	glm::vec3 getAttenuation() const {return m_attenuation;}

	void setPosition(const glm::vec3& value) {m_position = value;}
	void setAttenuation(const glm::vec3& value) {m_attenuation = value;}

protected:
	glm::vec3 m_position;
	glm::vec3 m_attenuation;	//!< contains model of attenuation coefficients: 0. constant, 1. linear, 2. exponential 
private:

};

class SpotLight : public PointLight{
public:
	SpotLight();
	SpotLight(	const glm::vec3& direction,
				float cutoff,
				const glm::vec3& position,
				const glm::vec3& attenuation = glm::vec3(1.f, 0.f, 0.f),
				const glm::vec3& colour = glm::vec3(0.f),
				float ambientI = 0.f,
				float diffuseI = 0.f);

	float getCutoff() const {return m_cutoff;}
	glm::vec3 getDirection() const{return m_direction;}

	void setCutoff(float value) {m_cutoff = value;}
	void setDirection(const glm::vec3& value) {m_direction = value;}

protected:

private:
	glm::vec3 m_direction; 
	float m_cutoff; 

};

struct LightBase
{
	GLint Colour;
	GLint Ambient;
	GLint Diffuse;
};

struct LightDirectional : public LightBase
{
	GLint Direction; 
};

struct LightPoint : public LightBase
{
	GLint Position;
	struct{
		GLint Constant;
		GLint Linear;
		GLint Exp;
	} Atten;
};

struct LightSpot : public LightPoint
{
	GLint Direction;
	GLint Cutoff;
};


const enum {MVP, LIGHT_MVP, WORLD_POS, COLOUR, SHADOW, NORMAL, EYE, SPEC_INT, SPEC_POW, NUM_POINTS, NUM_SPOTS};

/*
	Uniform names from the GLSL fragment shader - last three names correspond to the three separate data members in the
	LightingTechnique class
*/
const std::string shader_names[] = {
	"gMVP", 
	"gLightMVP", 
	"gWorld", 
	"gColour", 
	"gShadow", 
	"gNormal", 
	"gEye", 
	"gSpecularInt", 
	"gSpecularPow", 
	"gNumPointLights", 
	"gNumSpotLights",
	"gDirectionalLight",
	"gPointLights",
	"gSpotLights"
};
const unsigned int shader_names_size = sizeof(shader_names) / sizeof(shader_names[0]);

/* Structures from the fragment shader demonstrating the member names:

	struct BaseLight
	{
		vec3 Color;
		float Ambient;
		float Diffuse;
	};

	struct DirectionalLight
	{
		BaseLight Base;
		vec3 Direction;
	};

	struct Attenuation
	{
		float Constant;
		float Linear;
		float Exp;
	};

	struct PointLight
	{
		BaseLight Base;
		vec3 Position;
		Attenuation Atten;
	};

	struct SpotLight
	{
		PointLight Point;
		vec3 Direction;
		float Cutoff;
	};
*/


const enum { BASE, PNTL, COL, AMB, DIF, DIR, POS, CUT, ATT, CON, LIN, EXP };
const std::string member_names[] = {
	".Base", 
	".Point",
	".Colour",
	".Ambient", 
	".Diffuse",
	".Direction",
	".Position",
	".Cutoff",
	".Atten",
	".Constant",
	".Linear",
	".Exp"
};
const unsigned int member_names_size = sizeof(member_names) / sizeof(member_names[0]); 




class LightingTechnique : public GLSLProgram{
public:
	LightingTechnique(const std::string& vertex_shader_path, const std::string& frag_shader_path);

	static const unsigned int MAX_POINT_LIGHTS = 2;
	static const unsigned int MAX_SPOT_LIGHTS = 2;

	void setMVP(const glm::mat4& modelViewProj); 
	void setLightMVP(const glm::mat4& L_MPV);
	void setWorldPos(const glm::mat4& worldInverse);
	void setColourTexture(GLuint TextureUnit);
	void setShadowTexture(GLuint TextureUnit);
	void setNormalTexture(GLuint TextureUnit);
	void setEyeWorldPos(const glm::vec3& eyeWorldPos);
	void setMatSpecularIntensity(float Intensity);
	void setMatSpecularPower(float Power);

	void setDirectionalLight(const DirectionalLight& dirLight); 
	void setPointLights(GLuint NumLights, const PointLight* pLights);
	void setSpotLights(GLuint NumLights, const SpotLight* pLights);

private:

	GLint m_parameters[shader_names_size];

	LightDirectional m_dirLight;
	LightPoint m_pointLights[MAX_POINT_LIGHTS];
	LightSpot m_spotLights[MAX_SPOT_LIGHTS];

};


#endif