#include <Camera.hpp>

Camera::Camera(float left, float right, float bottom, float top, float zNear, float zFar) :
				Frustum(left, right, bottom, top, zNear, zFar),
				m_eye(0.f),
				m_up(0.f, 1.f, 0.f),
				m_forward(0.f, 0.f, -1.f),
				m_left(glm::normalize(glm::cross(m_up, m_forward))),
				m_orientation(),
				m_recalcViewMatrix(false),
				m_viewMatrix(),
				m_rotationHitCount(0)
{
	this->calcOrientation();
	this->normaliseCamera();
}


Camera::Camera(	float vFOV, float viewWidth, float viewHeight, float zNear, float zFar,
				const glm::vec3& eye,
				const glm::vec3& up,
				const glm::vec3& forward) :
				Frustum(vFOV, viewWidth, viewHeight, zNear, zFar), 
				m_eye(eye),
				m_up(up),
				m_forward(forward),
				m_left(glm::normalize(glm::cross(m_up, m_forward))),
				m_orientation(),
				m_recalcViewMatrix(false),
				m_viewMatrix(glm::lookAt(eye, eye + forward, up)),
				m_rotationHitCount(0)
{
	this->calcOrientation(); 
	this->normaliseCamera();
}

Camera::Camera(	float vFOV, float aspectRatio, float zNear, float zFar,
				const glm::vec3& eye,
				const glm::vec3& up,
				const glm::vec3& forward) :
				Frustum(vFOV, aspectRatio, zNear, zFar),
				m_eye(eye),
				m_up(up),
				m_forward(forward),
				m_left(glm::normalize(glm::cross(m_up, m_forward))),
				m_orientation(),
				m_recalcViewMatrix(false),
				m_viewMatrix(glm::lookAt(eye, eye + forward, up)),
				m_rotationHitCount(0)
{
	this->calcOrientation();
	this->normaliseCamera();
}

void Camera::calcOrientation()
{
	// Compute orientation from 3x3 change of basis matrix whose columns are the
	// world basis vectors given in Camera space coordinates.
	glm::mat3 m;
	m[0] = -1.0f * m_left;		// first column, representing the x-axis orientation
	m[1] = m_up;				// second column, representing the y-axis orientation
	m[2] = -1.0f * m_forward;	// third column, representing the z-axis orientation
	m_orientation = glm::quat_cast(m);

	countRotation();
	m_recalcViewMatrix = true;
}

glm::mat4 Camera::getViewMatrix() const
{
	if(m_recalcViewMatrix){
		m_viewMatrix = glm::translate(glm::mat4_cast(glm::inverse(m_orientation)), -m_eye);
		m_recalcViewMatrix = false;
	}

	return m_viewMatrix;
}

void Camera::normaliseCamera()
{
	m_left = glm::normalize(m_left);
	m_up = glm::normalize(m_up);
	m_forward = glm::normalize(m_forward); 
	m_orientation = glm::normalize(m_orientation);

	m_left = glm::cross(m_up, m_forward);
	m_up = glm::cross(m_forward, m_left);
}

void Camera::countRotation()
{
	if(++m_rotationHitCount > ms_rotationHitCountMax){		
		m_rotationHitCount = 0;
		normaliseCamera();
	}
}

void Camera::roll(float angle) 
{
	glm::quat q = glm::angleAxis(angle, m_forward);

	m_up = glm::rotate(q, m_up);
	m_left = glm::rotate(q, m_left);

	m_orientation = q * m_orientation;

	countRotation();
	m_recalcViewMatrix = true;
}

void Camera::pitch(float angle) 
{
	glm::quat q = glm::angleAxis(angle, -m_left);

	m_up = glm::rotate(q, m_up);
	m_forward = glm::rotate(q, m_forward);

	m_orientation = q * m_orientation;

	countRotation();
	m_recalcViewMatrix = true;
}

void Camera::yaw(float angle) 
{
	glm::quat q = glm::angleAxis(angle, m_up);

	m_left = glm::rotate(q, m_left);
	m_forward = glm::rotate(q, m_forward);

	m_orientation = q * m_orientation;

	countRotation();
	m_recalcViewMatrix = true;
}

void Camera::rotate(float angle, const glm::vec3 & axis) 
{
	glm::vec3 n = glm::normalize(axis);
	glm::quat q = angleAxis(angle, n);

	m_left = glm::rotate(q, m_left);
	m_up = glm::rotate(q, m_up);
	m_forward = glm::rotate(q, m_forward);

	m_orientation = q * m_orientation;

	countRotation();
	m_recalcViewMatrix = true;
}

void Camera::translate(const glm::vec3& v)
{
	m_eye += v;
	m_recalcViewMatrix = true;
}

void Camera::translateLocal(float left, float up, float forward)
{
	m_eye += left * m_left;
	m_eye += up * m_up;
	m_eye += forward * m_forward;
	m_recalcViewMatrix = true;
}

void Camera::translateLocal(const glm::vec3& v)
{
	this->translateLocal(v.x, v.y, v.z);
}

void Camera::lookAt(const glm::vec3& target) 
{
	//this is okay so long as the target does not lie on the line the current up vector describes. 
	// one solution may be to do the calculation in two stages if the target lies close to the up vector line.
	m_forward = glm::normalize(target - m_eye);
	m_left = glm::normalize(cross(m_up, m_forward));
	m_up = glm::cross(m_forward, m_left);

	this->calcOrientation();
}

void Camera::firstPersonlookAt(float angle)
{
	//change the orientation based on the pitch angle (i.e. look up or down)
	//but do not change the camera local vectors 

	m_orientation = glm::angleAxis(angle, -m_left) * m_orientation;
	//rotations are counted when yaw is applied

	//remove any ambient roll after N rotations
	if(m_rotationHitCount >= ms_rotationHitCountMax) {
		m_left.y = m_forward.y = m_up.x = m_up.z = 0.f; 
	}

	m_recalcViewMatrix = true;
}