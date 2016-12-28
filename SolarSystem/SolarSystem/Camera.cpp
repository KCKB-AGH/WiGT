#include "stdafx.h"
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32
#include <gl/GL.h>
#include <gl\GLU.h>
#include <cmath>
#include "Camera.h"

void vectorSet(float* vector, float x, float y, float z)
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
}

void vectorAdd(float* vector1, float* vector2)
{
	vector1[0] += vector2[0];
	vector1[1] += vector2[1];
	vector1[2] += vector2[2];
}

void vectorCopy(float* vector1, float* vector2)
{
	vector1[0] = vector2[0];
	vector1[1] = vector2[1];
	vector1[2] = vector2[2];
}

void vectorMul(float* vector, float scalar)
{
	vector[0] *= scalar;
	vector[1] *= scalar;
	vector[2] *= scalar;
}

float lengthOfVector(float* vector)
{
	return sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

void normaliseVector(float* vector)
{
	vectorMul(vector, 1 / lengthOfVector(vector));
}

void rotationMatrix(float* matrix, float* axis, float angle)
{
	float cos1 = cos(angle);
	float cos2 = 1 - cos1;
	float sin1 = sin(angle);

	matrix[0] = cos1 + axis[0] * axis[0] * cos2;
	matrix[1] = axis[0] * axis[1] * cos2 - axis[2] * sin1;
	matrix[2] = axis[0] * axis[2] * cos2 + axis[1] * sin1;

	matrix[3] = axis[1] * axis[0] * cos2 + axis[2] * sin1;
	matrix[4] = cos1 + axis[1] * axis[1] * cos2;
	matrix[5] = axis[1] * axis[2] * cos2 - axis[0] * sin1;

	matrix[6] = axis[2] * axis[0] * cos2 - axis[1] * sin1;
	matrix[7] = axis[2] * axis[1] * cos2 + axis[0] * sin1;
	matrix[8] = cos1 + axis[2] * axis[2] * cos2;
}


void mulVecBy(float* vector1, float* matrix, float* vector2)
{
	vector2[0] = vector1[0] * matrix[0] + vector1[1] * matrix[1] + vector1[2] * matrix[2];
	vector2[1] = vector1[0] * matrix[3] + vector1[1] * matrix[4] + vector1[2] * matrix[5];
	vector2[2] = vector1[0] * matrix[6] + vector1[1] * matrix[7] + vector1[2] * matrix[8];
}

void rotateAroundVector(float* vector1, float* vector2, float angle, float* vector3)
{
	float matrix[16];
	rotationMatrix(matrix, vector2, angle);

	mulVecBy(vector1, matrix, vector3);
}

Camera::Camera(void)
{
	cameraSpeed = 0.005f;
	cameraTurnSpeed = 0.01f;

	vectorSet(position, 0.764331460f, -1.66760659f, 0.642456770);
	vectorSet(forwardVector, -0.398769796f, 0.763009906f, -0.508720219f);
	vectorSet(rightVector, 0.886262059f, 0.463184059f, 0.000000000f);
	vectorSet(upVector, -0.235630989f, 0.450859368f, 0.860931039f);
}

void Camera::transformOrientation(void)
{
	gluLookAt(0, 0, 0, forwardVector[0], forwardVector[1], forwardVector[2], upVector[0], upVector[1], upVector[2]);
}

void Camera::transformTranslation(void)
{
	glTranslatef(-position[0], -position[1], -position[2]);
}

void Camera::pointAt(float* targetVector)
{
	float tempVector[3];
	float up[3] = { 0.0f, 0.0f, 1.0f };

	forwardVector[0] = targetVector[0] - position[0];
	forwardVector[1] = targetVector[1] - position[2];
	forwardVector[2] = targetVector[1] - position[2];

	normaliseVector(forwardVector);

	rotateAroundVector(forwardVector, up, -1.57079632679f, tempVector);

	tempVector[2] = 0;
	normaliseVector(tempVector);
	vectorCopy(rightVector, tempVector);

	rotateAroundVector(forwardVector, rightVector, 1.57079632679f, tempVector);
	vectorCopy(upVector, tempVector);
}

void Camera::speedUp(void)
{
	if (cameraSpeed < 1.0f)
		cameraSpeed *= 2;
}

void Camera::slowDown(void)
{
	if (cameraSpeed > 0.000001f)
		cameraSpeed /= 2;
}

void Camera::forward(void)
{
	float vector[3];
	vectorCopy(vector, forwardVector);
	vectorMul(vector, cameraSpeed);
	vectorAdd(position, vector);
}

void Camera::backward(void)
{
	float vector[3];
	vectorCopy(vector, forwardVector);
	vectorMul(vector, -cameraSpeed);

	vectorAdd(position, vector);
}

void Camera::left(void)
{
	float vector[3];
	vectorCopy(vector, rightVector);
	vectorMul(vector, -cameraSpeed);

	vectorAdd(position, vector);
}

void Camera::right(void)
{
	float vector[3];
	vectorCopy(vector, rightVector);
	vectorMul(vector, cameraSpeed);

	vectorAdd(position, vector);
}

void Camera::rollRight(void)
{
	float tempVector[3];
	rotateAroundVector(upVector, forwardVector, cameraTurnSpeed, tempVector);
	vectorCopy(upVector, tempVector);
	rotateAroundVector(rightVector, forwardVector, cameraTurnSpeed, tempVector);
	vectorCopy(rightVector, tempVector);
}

void Camera::rollLeft(void)
{
	float tempVector[3];
	rotateAroundVector(upVector, forwardVector, -cameraTurnSpeed, tempVector);
	vectorCopy(upVector, tempVector);
	rotateAroundVector(rightVector, forwardVector, -cameraTurnSpeed, tempVector);
	vectorCopy(rightVector, tempVector);
}

void Camera::pitchDown(void)
{
	float tempVector[3];
	rotateAroundVector(forwardVector, rightVector, -cameraTurnSpeed, tempVector);
	vectorCopy(forwardVector, tempVector);
	rotateAroundVector(upVector, rightVector, -cameraTurnSpeed, tempVector);
	vectorCopy(upVector, tempVector);
}

void Camera::pitchUp(void)
{
	float tempVector[3];
	rotateAroundVector(forwardVector, rightVector, cameraTurnSpeed, tempVector);
	vectorCopy(forwardVector, tempVector);
	rotateAroundVector(upVector, rightVector, cameraTurnSpeed, tempVector);
	vectorCopy(upVector, tempVector);
}

void Camera::yawLeft(void)
{
	float tempVector[3];
	rotateAroundVector(forwardVector, upVector, cameraTurnSpeed, tempVector);
	vectorCopy(forwardVector, tempVector);
	rotateAroundVector(rightVector, upVector, cameraTurnSpeed, tempVector);
	vectorCopy(rightVector, tempVector);
}

void Camera::yawRight(void)
{
	float tempVector[3];
	rotateAroundVector(forwardVector, upVector, -cameraTurnSpeed, tempVector);
	vectorCopy(forwardVector, tempVector);
	rotateAroundVector(rightVector, upVector, -cameraTurnSpeed, tempVector);
	vectorCopy(rightVector, tempVector);
}