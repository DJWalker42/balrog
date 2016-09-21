#include <Frustum.h>

#include <glm/gtc/matrix_transform.hpp>

//----------------------------------------------------------------------------------------
Frustum::Frustum():
			m_vFOV(0.0f),
			m_aspectRatio(0.0f),
			m_zNear(0.0f),
			m_zFar(0.0f),
			m_recalcPerspectiveMatrix(false),
			m_projType(ORTHO),
			m_projectionMatrix()
{}

//----------------------------------------------------------------------------------------
Frustum::Frustum(const Frustum & frustum):
			m_vFOV(frustum.m_vFOV),
			m_aspectRatio(frustum.m_aspectRatio),
			m_zNear(frustum.m_zNear),
			m_zFar(frustum.m_zFar),
			m_recalcPerspectiveMatrix(frustum.m_recalcPerspectiveMatrix),
			m_projType(frustum.m_projType),
			m_projectionMatrix(frustum.m_projectionMatrix)
{}

//----------------------------------------------------------------------------------------
Frustum::~Frustum() {}

//----------------------------------------------------------------------------------------
/**
* Constructs an orthographic projection view Frustum.
*
* @note If m_zNear or m_zFar are negative, corresponding z clipping planes
* are considered behind the viewer.
*
* @param left - specify location of left clipping plane.
* @param right - specify location of right clipping plane.
* @param bottom - specify location of bottom clipping plane.
* @param top - specify location of top clipping plane.
* @param m_zNear - distance to near z clipping plane.
* @param m_zFar - distance to far z clipping plane.
* @note: viewWidth = right - left, viewHeight = top - bottom.
*/
Frustum::Frustum(float left, float right, float bottom, float top, float zNear, float zFar)
	:	m_vFOV(0.0f),
		m_aspectRatio((right - left) / (top - bottom)),
		m_viewWidth(right - left),
		m_viewHeight(top - bottom),
		m_zNear(zNear),
		m_zFar(zFar),
		m_recalcPerspectiveMatrix(false),
		m_projType(ORTHO),
		m_projectionMatrix(glm::ortho(left, right, bottom, top, m_zNear, m_zFar))
{}

//----------------------------------------------------------------------------------------
/**
* Constructs a perspective projection view Frustum.
*
* @note  Depth buffer precision is affected by the values specified for
* m_zNear and m_zFar. The greater the ratio of m_zFar to m_zNear the less
* effective the depth buffer will be at distinguishing between surfaces
* that are near each other. If  r = m_zFar/m_zNear, about log2(r) bits
* of depth buffer precision are lost. Because r approaches infinity as
* m_zNear approaches 0, m_zNear must never be set to 0.
*
* @param fieldOfViewY
* @param aspectRatio
* @param zNear - distance to near z clipping plane (strictly positive i.e. always > 0).
* @param zFar - distance to far z clipping plane (striclty positive i.e. always > 0).
*/
Frustum::Frustum(float fieldOfViewY, float aspectRatio, float zNear, float zFar):
	m_vFOV(fieldOfViewY),
	m_aspectRatio(aspectRatio),
	m_viewWidth(-1.f),
	m_viewHeight(-1.f),
	m_zNear(zNear),
	m_zFar(zFar),
	m_recalcPerspectiveMatrix(false),
	m_projType(PERS),
	m_projectionMatrix(glm::perspective(glm::radians(m_vFOV), m_aspectRatio, m_zNear, m_zFar))
{}


//----------------------------------------------------------------------------------------
/**
* Constructs a perspective projection view \c Frustum with maintained aspected ratio
*
* @param fieldOfViewY vertical field of view in degrees
* @param viewWidth
* @param viewHeight (aspectRatio = viewWidth / viewHeight)
* @param m_zNear - distance to near z clipping plane (strictly positive i.e. always > 0).
* @param m_zFar - distance to far z clipping plane (strictly positive i.e. always > 0).
*/
Frustum::Frustum(float fieldOfViewY, float viewWidth, float viewHeight, float zNear, float zFar):
			m_vFOV(fieldOfViewY),
			m_aspectRatio(viewWidth / viewHeight),
			m_viewWidth(viewWidth),
			m_viewHeight(viewHeight),
			m_zNear(zNear),
			m_zFar(zFar),
			m_recalcPerspectiveMatrix(false),
			m_projType(PFOV),
			m_projectionMatrix(glm::perspectiveFov(glm::radians(m_vFOV), viewWidth, viewHeight, m_zNear, m_zFar))
{}

//----------------------------------------------------------------------------------------
glm::mat4 Frustum::getProjectionMatrix() const
{
	if (m_recalcPerspectiveMatrix) {
		switch(m_projType){
		case PERS:
			m_projectionMatrix = glm::perspective(glm::radians(m_vFOV), m_aspectRatio, m_zNear, m_zFar);
			break;
		case PFOV:
			m_projectionMatrix = glm::perspectiveFov(glm::radians(m_vFOV), m_viewWidth, m_viewHeight, m_zNear, m_zFar);
			break;
		case ORTHO:
			break; //no projection Matrix
		}
		m_recalcPerspectiveMatrix = false;
	}
	return m_projectionMatrix;
}

//----------------------------------------------------------------------------------------
float Frustum::getFieldOfViewY() const
{
	return m_vFOV;
}

//----------------------------------------------------------------------------------------
float Frustum::getAspectRatio() const
{
	return m_aspectRatio;
}

int Frustum::getViewWidth() const
{
	return static_cast<int>(m_viewWidth);
}

int Frustum::getViewHeight() const
{
	return static_cast<int>(m_viewHeight);
}

//----------------------------------------------------------------------------------------
float Frustum::getNearZDistance() const
{
	return m_zNear;
}

//----------------------------------------------------------------------------------------
float Frustum::getFarZDistance() const
{
	return m_zFar;
}

//----------------------------------------------------------------------------------------
bool Frustum::isPerspective() const
{
	if(m_projType == PERS || m_projType == PFOV){
		return true;
	}else{
		return false;
	}
}

//----------------------------------------------------------------------------------------
bool Frustum::isOrthographic() const
{
	if (m_projType == ORTHO){
		return true;
	}else{
		return false;
	}
}

//----------------------------------------------------------------------------------------
void Frustum::setFieldOfViewY(float fieldOfViewY)
{
	//account for negative values
	if (fieldOfViewY < 0.0f){
		fieldOfViewY = -fieldOfViewY;
	}

	//account for nonesensical values
	if (fieldOfViewY > 180.0f){
		fieldOfViewY = 180.0f;
	}

	this->m_vFOV = fieldOfViewY;
	m_recalcPerspectiveMatrix = true;
}

//----------------------------------------------------------------------------------------
void Frustum::setAspectRatio(float aspectRatio)
{
	this->m_aspectRatio = aspectRatio;
	m_recalcPerspectiveMatrix = true;
}

//----------------------------------------------------------------------------------------
void Frustum::setViewWidth(float width)
{
	this->m_viewWidth = width;
	m_recalcPerspectiveMatrix = true;
}

//----------------------------------------------------------------------------------------
void Frustum::setViewHeight(float height)
{
	this->m_viewHeight = height;
	m_recalcPerspectiveMatrix = true;
}

//----------------------------------------------------------------------------------------
void Frustum::setNearZDistance(float m_zNear)
{
	this->m_zNear = m_zNear;
	m_recalcPerspectiveMatrix = true;
}

//----------------------------------------------------------------------------------------
void Frustum::setFarZDistance(float m_zFar)
{
	this->m_zFar = m_zFar;
	m_recalcPerspectiveMatrix = true;
}

//----------------------------------------------------------------------------------------
void Frustum::setProjectionMatrix(const glm::mat4 & projectionMatrix)
{
	this->m_projectionMatrix = projectionMatrix;
}
