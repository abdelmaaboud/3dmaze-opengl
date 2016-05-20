#include "FPCamera.h"

FPCamera::FPCamera(void)
{
	this->Reset(0, 0, 0,
		0, 0, -1,
		0, 1, 0);

	SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}

FPCamera::~FPCamera(void)
{
}

glm::vec3 FPCamera::GetLookDirection()
{
	return -mDirection;
}

/// <summary>
/// Rotates the vector v about the axis axisVec the specified angle (in degrees),
/// in a CCW, and returns the resulting vector.
/// </summary>
void FPCamera::RotateAround(glm::vec3 &v, float angle, glm::vec3 &axisVec)
{
	glm::vec3 retval;
	const float PI = 3.1415926535897932384626433832795;

	angle *= -(float)PI / 180;
	axisVec = glm::normalize(axisVec);

	float c = (float)cos(angle);
	float s = (float)sin(angle);
	float t = 1 - c;


	retval.x = (t * (axisVec.x*axisVec.x) + c) * v.x;
	retval.x += (t * axisVec.x * axisVec.y + s * axisVec.z) * v.y;
	retval.x += (t * axisVec.x * axisVec.z - s * axisVec.y) * v.z;

	retval.y = (t * axisVec.x * axisVec.y - s * axisVec.z) * v.x;
	retval.y += (t * (axisVec.y*axisVec.y) + c) * v.y;
	retval.y += (t * axisVec.y * axisVec.z + s * axisVec.x) * v.z;

	retval.z = (t * axisVec.x * axisVec.y + s * axisVec.y) * v.x;
	retval.z += (t * axisVec.y * axisVec.z - s * axisVec.x) * v.y;
	retval.z += (t * (axisVec.z*axisVec.z) + c) * v.z;

	v.x = retval.x;
	v.y = retval.y;
	v.z = retval.z;
}



void FPCamera::Reset(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	glm::vec3 eyePt(eyeX, eyeY, eyeZ);
	glm::vec3 centerPt(centerX, centerY, centerZ);
	glm::vec3 upVec(upX, upY, upZ);
	Reset(eyePt, centerPt, upVec);
}

void FPCamera::Reset(const glm::vec3 &eye, const glm::vec3 &center, glm::vec3 &up)
{
	mPosition = eye;
	//still needs normalization
	mDirection = eye - center;
	//i = j x k
	mRight = glm::cross(up, mDirection);
	//j = k x i
	mUp = glm::cross(mDirection, mRight);
	//normalize all
	mUp = glm::normalize(mUp);
	mRight = glm::normalize(mRight);
	mDirection = glm::normalize(mDirection);
	UpdateViewMatrix();
}

glm::mat4 FPCamera::GetViewMatrix()
{
	return mViewMatrix;
}

void FPCamera::UpdateViewMatrix()
{
	glm::vec3 center = mPosition + this->GetLookDirection();
	mViewMatrix = glm::lookAt(mPosition, center, mUp);
}

glm::mat4 FPCamera::GetProjectionMatrix()
{
	return mProjectionMatrix;
}

void FPCamera::SetPerspectiveProjection(float FOV, float aspectRatio, float near, float far)
{
	mProjectionMatrix = glm::perspective(FOV, aspectRatio, near, far);
}

void FPCamera::Slide(float stepR, float stepU, float stepD)
{
	mPosition += stepR *mRight;
	mPosition += stepU * mUp;
	//next transition needs to be tested!!.
	mPosition += stepD * mDirection;
}


void FPCamera::Yaw(float angleDegrees)
{
	mDirection = glm::rotate(mDirection, angleDegrees, mUp);
	mRight = glm::rotate(mRight, angleDegrees, mUp);

	////OR:
	//RotateAround(mDirection, angleDegrees, mUp);
	//RotateAround(mRight, angleDegrees, mUp);
	//mDirection = glm::normalize(mDirection);
	//mRight = glm::normalize(mRight);
}

void FPCamera::Pitch(float angleDegrees)
{

	mUp = glm::rotate(mUp, angleDegrees, mRight);
	mDirection = glm::rotate(mDirection, angleDegrees, mRight);

	////OR:
	//RotateAround(mDirection, angleDegrees, mRight);
	//RotateAround(mUp, angleDegrees, mRight);
	//mUp = glm::normalize(mUp);
	//mDirection = glm::normalize(mDirection);
}

void FPCamera::Roll(float angleDegrees)
{
	mRight = glm::rotate(mRight, angleDegrees, mDirection);
	mUp = glm::rotate(mUp, angleDegrees, mDirection);
}

void FPCamera::Walk(float dist)
{
	mPosition -= dist * mDirection;
}

void FPCamera::Strafe(float dist)
{
	mPosition += dist *mRight;
}

void FPCamera::Fly(float dist)
{
	mPosition += dist * mUp;
}


