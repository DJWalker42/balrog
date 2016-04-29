#ifndef DJW_FRUSTRUM_H
#define DJW_FRUSTRUM_H

#include <glm/glm.hpp>

class Frustum {
public:
	Frustum();

	Frustum(const Frustum & frustum);  // Copy Constructor

	//Constructor for orthographic projection
	Frustum(float left, float right, float bottom, float top, float zNear, float zFar); 

	//Constructor for perspective projection (Aspect ratio of objects not maintained; uses glm::perspective())
	Frustum(float fieldOfViewY, float aspectRatio, float zNear, float zFar); 

	//Constructor for perspective projection (Aspect ratio of objects maintianed; uses glm::perpectiveFov())
	Frustum(float fieldOfViewY, float viewWidth, float viewHeight, float zNear, float zFar); 

	virtual ~Frustum();

	glm::mat4 getProjectionMatrix() const;
	float getFieldOfViewY() const;
	float getAspectRatio() const;
	int getViewWidth() const;
	int getViewHeight() const;
	float getNearZDistance() const;
	float getFarZDistance() const;

	void setFieldOfViewY(float fieldOfViewY);
	void setAspectRatio(float aspectRatio);
	void setViewWidth(float viewWidth);
	void setViewHeight(float viewHeight);
	void setNearZDistance(float zNear);
	void setFarZDistance(float zFar);
	void setProjectionMatrix(const glm::mat4 & projectionMatrix);

	bool isPerspective() const;
	bool isOrthographic() const;

private:
	float m_vFOV;							//!< vertical field of view desired
	float m_aspectRatio;					//!< aspect ratio desired == viewWidth / viewHeight
	float m_viewWidth;						//!< horizontal length of the view port
	float m_viewHeight;						//!< vertical length of the view port
	float m_zNear;							//!< the near depth clipping plane position
	float m_zFar;							//!< the far depth clipping plane position
	mutable bool m_recalcPerspectiveMatrix;	//!< does the matrix require recalculation? 
	//(mutable as it is used in the getProjectionMatrix() function that is declared const).

	enum projectionType {ORTHO, PERS, PFOV} m_projType; //!< the type of projection the frustrum represents

	mutable glm::mat4 m_projectionMatrix; //the projection matrix (mutable as it is potentially recalulated in the getProjectionMatrix() function).

	//const static unsigned short m_floatTolerance = 5; //unused: number of sig figs to which to except a floating point number as constant.
};

#endif