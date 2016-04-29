#include "BasicLighting.hpp"
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

BaseLight::BaseLight() : m_colour(0.f), m_ambient(0.f), m_diffuse(0.f)
{}

BaseLight::BaseLight(	const glm::vec3& colour, 
						float ambientI, 
						float diffuseI) :
						m_colour(colour), 
						m_ambient(ambientI), 
						m_diffuse(diffuseI)
{}


DirectionalLight::DirectionalLight() : BaseLight(), m_direction(0.f)
{}


DirectionalLight::DirectionalLight(	const glm::vec3& direction,
									const glm::vec3& colour,
									float ambientI,
									float diffuseI) :
									BaseLight(colour, ambientI, diffuseI),
									m_direction(direction)
{}

PointLight::PointLight() : BaseLight(), m_position(0.f), m_attenuation(1.f,0.f,0.f)
{}


PointLight::PointLight(		const glm::vec3& position,
							const glm::vec3& attenuation,
							const glm::vec3& colour,
							float ambientI,
							float diffuseI) :
							BaseLight(colour, ambientI, diffuseI),
							m_position(position),
							m_attenuation(attenuation)
{}

SpotLight::SpotLight() : PointLight(), m_direction(0.f), m_cutoff(0.f)
{}

SpotLight::SpotLight(	const glm::vec3& direction,
						float cutoff,
						const glm::vec3& position,
						const glm::vec3& attenuation,
						const glm::vec3& colour,
						float ambientI,
						float diffuseI) :
						PointLight(position, attenuation, colour, ambientI, diffuseI),
						m_direction(direction),
						m_cutoff(cutoff)
{}

LightingTechnique::LightingTechnique(	const std::string& vs_fn, 
										const std::string& fs_fn) : 
										GLSLProgram(vs_fn, fs_fn)  //base constructor creates program handle, and compiles and links shaders
{
	size_t index = shader_names_size - 3;

	for (size_t i = 0; i < index; ++i) {
		m_parameters[i] = getUniformLocation(shader_names[i]);
		if(m_parameters[i] < 0)
			throw(std::runtime_error("Error finding uniform name " + shader_names[i]));
	}
	
	GLint tmp[9];
	tmp[0] = m_dirLight.Colour		= getUniformLocation(shader_names[index] + member_names[BASE] + member_names[COL]);
	tmp[1] = m_dirLight.Ambient		= getUniformLocation(shader_names[index] + member_names[BASE] + member_names[AMB]);
	tmp[2] = m_dirLight.Diffuse		= getUniformLocation(shader_names[index] + member_names[BASE] + member_names[DIF]);
	tmp[3] = m_dirLight.Direction	= getUniformLocation(shader_names[index] + member_names[DIR]);

	for (int i = 0; i < 4; ++i) {
		if(tmp[i] < 0) {
			throw(std::runtime_error("Error finding uniform name from directional light"));
		}
	}

	++index;

	for(size_t i = 0; i < MAX_POINT_LIGHTS; ++i){
		std::stringstream idx;
		idx << "[" << i << "]"; 
		std::string shaderName = shader_names[index] + idx.str();
		tmp[0] = m_pointLights[i].Colour	= getUniformLocation(shaderName + member_names[BASE] + member_names[COL]);
		tmp[1] = m_pointLights[i].Ambient	= getUniformLocation(shaderName + member_names[BASE] + member_names[AMB]);
		tmp[2] = m_pointLights[i].Diffuse	= getUniformLocation(shaderName + member_names[BASE] + member_names[DIF]);
		tmp[3] = m_pointLights[i].Position	= getUniformLocation(shaderName + member_names[POS]);
		shaderName += member_names[ATT];
		tmp[4] = m_pointLights[i].Atten.Constant	= getUniformLocation(shaderName + member_names[CON]);
		tmp[5] = m_pointLights[i].Atten.Linear		= getUniformLocation(shaderName + member_names[LIN]);
		tmp[6] = m_pointLights[i].Atten.Exp			= getUniformLocation(shaderName + member_names[EXP]);

		for (int k = 0; k < 7; ++k) {
			if (tmp[k] < 0) {
				throw(std::runtime_error("Error finding uniform name from point light " + idx.str()));
			}
		}
	}

	++index;

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; ++i) {
		std::stringstream idx;
		idx << "[" << i << "]"; 
		std::string shaderName = shader_names[index] + idx.str();
		tmp[0] = m_spotLights[i].Direction	= getUniformLocation(shaderName + member_names[DIR]);
		tmp[1] = m_spotLights[i].Cutoff		= getUniformLocation(shaderName + member_names[CUT]);
		shaderName += member_names[PNTL]; 
		tmp[2] = m_spotLights[i].Position	= getUniformLocation(shaderName + member_names[POS]);
		tmp[3] = m_spotLights[i].Colour		= getUniformLocation(shaderName + member_names[BASE] + member_names[COL]);
		tmp[4] = m_spotLights[i].Ambient	= getUniformLocation(shaderName + member_names[BASE] + member_names[AMB]);
		tmp[5] = m_spotLights[i].Diffuse	= getUniformLocation(shaderName + member_names[BASE] + member_names[DIF]);
		shaderName += member_names[ATT];
		tmp[6] = m_spotLights[i].Atten.Constant = getUniformLocation(shaderName + member_names[CON]);
		tmp[7] = m_spotLights[i].Atten.Linear	= getUniformLocation(shaderName + member_names[LIN]);
		tmp[8] = m_spotLights[i].Atten.Exp		= getUniformLocation(shaderName + member_names[EXP]);

		for (int k = 0; k < 9; ++k) {
			if (tmp[k] < 0) {
				throw(std::runtime_error("Error finding uniform name from spot light " + idx.str()));
			}
		}
	}

}


void LightingTechnique::setMVP(const glm::mat4& modelViewProj)
{
	glUniformMatrix4fv(m_parameters[MVP], 1, GL_FALSE, glm::value_ptr(modelViewProj));
}

void LightingTechnique::setLightMVP(const glm::mat4& lightMVP)
{
	glUniformMatrix4fv(m_parameters[LIGHT_MVP], 1, GL_FALSE, glm::value_ptr(lightMVP));
}

void LightingTechnique::setWorldPos(const glm::mat4& WorldInverse)
{
	glUniformMatrix4fv(m_parameters[WORLD_POS], 1, GL_FALSE, glm::value_ptr(WorldInverse));
}

void LightingTechnique::setColourTexture(GLuint TextureUnit)
{
	glUniform1i(m_parameters[COLOUR], TextureUnit);
}

void LightingTechnique::setShadowTexture(GLuint TextureUnit)
{
	glUniform1i(m_parameters[SHADOW], TextureUnit);
}

void LightingTechnique::setNormalTexture(GLuint TextureUnit)
{
	glUniform1i(m_parameters[NORMAL], TextureUnit);
}

void LightingTechnique::setDirectionalLight(const DirectionalLight& Light)
{
	glm::vec3 t_colour = Light.getColour();
	glUniform3f(m_dirLight.Colour, t_colour.x, t_colour.y, t_colour.z);
	
	glm::vec3 t_direction = glm::normalize(Light.getDirection());
	glUniform3f(m_dirLight.Direction, t_direction.x, t_direction.y, t_direction.z);

	glUniform1f(m_dirLight.Ambient, Light.getAmbient());
	glUniform1f(m_dirLight.Diffuse, Light.getDiffuse());
}

void LightingTechnique::setEyeWorldPos(const glm::vec3& EyeWorldPos)
{
	glUniform3f(m_parameters[EYE], EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void LightingTechnique::setMatSpecularIntensity(float Intensity)
{
	glUniform1f(m_parameters[SPEC_INT], Intensity);
}

void LightingTechnique::setMatSpecularPower(float Power)
{
	glUniform1f(m_parameters[SPEC_POW], Power);
}

void LightingTechnique::setPointLights(unsigned int NumLights, const PointLight* pLights)
{
	glUniform1i(m_parameters[NUM_POINTS], NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {

		glm::vec3 t_colour = pLights[i].getColour();
		glm::vec3 t_position = pLights[i].getPosition();
		glm::vec3 t_atten = pLights[i].getAttenuation();

		glUniform3f(m_pointLights[i].Colour, t_colour.x, t_colour.y, t_colour.z);
		glUniform3f(m_pointLights[i].Position, t_position.x, t_position.y, t_position.z);

		glUniform1f(m_pointLights[i].Ambient, pLights[i].getAmbient());
		glUniform1f(m_pointLights[i].Diffuse, pLights[i].getDiffuse());
		
		glUniform1f(m_pointLights[i].Atten.Constant, t_atten[0]);
		glUniform1f(m_pointLights[i].Atten.Linear, t_atten[1]);
		glUniform1f(m_pointLights[i].Atten.Exp, t_atten[2]);
	}
}


void LightingTechnique::setSpotLights(unsigned int NumLights, const SpotLight* pLights)
{
	glUniform1i(m_parameters[NUM_SPOTS], NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {

		glm::vec3 t_colour = pLights[i].getColour();
		glm::vec3 t_position = pLights[i].getPosition();
		glm::vec3 t_atten = pLights[i].getAttenuation();
		glm::vec3 t_direction = glm::normalize(pLights[i].getDirection());
		float t_cutoff = pLights[i].getCutoff();

		glUniform3f(m_spotLights[i].Colour, t_colour.x, t_colour.y, t_colour.z);
		glUniform3f(m_spotLights[i].Position, t_position.x, t_position.y, t_position.z);
		glUniform3f(m_spotLights[i].Direction, t_direction.x, t_direction.y, t_direction.z);
		glUniform1f(m_spotLights[i].Ambient, pLights[i].getAmbient());
		glUniform1f(m_spotLights[i].Diffuse, pLights[i].getDiffuse());		
		glUniform1f(m_spotLights[i].Cutoff, cosf(glm::radians(t_cutoff)));

		glUniform1f(m_spotLights[i].Atten.Constant, t_atten[0]);
		glUniform1f(m_spotLights[i].Atten.Linear, t_atten[1]);
		glUniform1f(m_spotLights[i].Atten.Exp, t_atten[2]);
	}
}