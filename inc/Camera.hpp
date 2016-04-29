#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Frustum.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

const float PI_f = 3.141593f;
const double PI_d = 3.14159265358979; 
//debugging parameters
//const int MARGIN = 10;
//const int MIDDLE = 100;

//@TODO: implement these overloads for debugging purposes
std::ostream& operator<< (std::ostream& os, const glm::vec3& v);
std::ostream& operator<< (std::ostream& os, const glm::quat& q);


class Camera : public Frustum
{
public:
	Camera(	float left, float right, float top, float bottom, float zNear, float zFar);

	Camera(	float vFOV, float viewWidth, float viewHeight, float zNear, float zFar,
			const glm::vec3& eye = glm::vec3(0.f),
			const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f),
			const glm::vec3& forward = glm::vec3(0.f, 0.f, -1.f));

	Camera(	float vFOV, float aspectRatio, float zNear, float zFar,			
			const glm::vec3& eye = glm::vec3(0.f),
			const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f),
			const glm::vec3& forward = glm::vec3(0.f, 0.f, -1.f));


	glm::mat4 getViewMatrix() const;

	glm::vec3 getCameraPos() const { return m_eye; }

	//provide target as a position vector in world coordinates
	void lookAt(const glm::vec3& target);

	//provide the angle in degrees. Clamps to the range -89 to +89 degrees to avoid screen inversion
	void firstPersonlookAt(float pitchAngle);

	//apply a roll rotation (about camera forward) by delta_angle degrees. 
	//Note the angle is relative to the previous orientation not an absolute value.
	void roll(float delta_angle);

	//apply a pitch rotation (about camera -left) by delta_angle degrees. 
	//Note the angle is relative to the previous orientation not an absolute value.
	void pitch(float delta_angle);

	//apply a yaw rotation (about camera up) by delta_angle degrees. 
	//Note the angle is relative to the previous orientation not an absolute value.
	void yaw(float delta_angle);

	//apply a general rotation about an arbitrary axis by delta_angle degrees. 
	//Note the angle is relative to the previous orientation not an absolute value.
	void rotate(float delta_angle, const glm::vec3& axis);

	//translate to a new world coordinate position vector v
	void translate(const glm::vec3& v);
	//translate to a new position using the camera's current local left, up, and forward vectors.
	//the values specify how far to move in each direction, respectively.
	void translateLocal(float left, float up, float forward);
	//translate to a new position using the camera's current local left, up, and forward vectors.
	//the values in the parameter v specifiy how far to move in each direction, respectively.
	void translateLocal(const glm::vec3 &v);
	
private:
	void calcOrientation();
	//count the number of rotations perfomered. For every N rotations normalise the camera.
	//This alleviates floating point precision error accumulation. 
	void countRotation();
	void normaliseCamera();
	 
private:

	glm::vec3 m_eye;					//!< Camera position in world space coordinates
	glm::quat m_orientation;			//!< A quaternion of the camera's current orientation in world space

	/*The following three vectors _SHOULD_ form an orthonormal set in 3D to within floating point precision at all times*/

	glm::vec3 m_up;						//!< Camera's current local up direction vector
	glm::vec3 m_forward;				//!< Camera's current local forward direction vector
	glm::vec3 m_left;					//!< Camera's current local left direction vector

	mutable bool m_recalcViewMatrix;	//!< Does the view matrix need recalculating for the next frame render? (i.e. have we moved the camera?)
	mutable glm::mat4 m_viewMatrix;		//!< The view matrix used to compute the world-view space 

	unsigned short m_rotationHitCount;	//!< the number of rotations applied to date.
	static const unsigned short ms_rotationHitCountMax = 1000; //!< the maximum number of rotations to apply before normalising the camera.
};


#endif