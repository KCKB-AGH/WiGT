#include "stdafx.h"
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32
#include <gl/GL.h>
#include <gl\GLU.h>
#include <cmath>
#include "Camera.h"

//kamera sterowania jak statek kosmiczny
//za pomoca klawiatury bedzie mozna przesuwac, obracac itp 
//up, forward, right -> reprezentacja wektora orientacji na wszystkich osiach
//wektor position do reprezentowania translacji

//ustawienie wektora do x,y,z
void vectorSet(float* vector, float x, float y, float z)
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
}

//dodawanie dwoch wektorow
void vectorAdd(float* vector1, float* vector2)
{
	vector1[0] += vector2[0];
	vector1[1] += vector2[1];
	vector1[2] += vector2[2];
}

//kopiowanie wektora2 do wektora1
void vectorCopy(float* vector1, float* vector2)
{
	vector1[0] = vector2[0];
	vector1[1] = vector2[1];
	vector1[2] = vector2[2];
}

//mnozenie wektora przez skalar
void vectorMul(float* vector, float scalar)
{
	vector[0] *= scalar;
	vector[1] *= scalar;
	vector[2] *= scalar;
}

//obliczenie wielkosci wektora
float lengthOfVector(float* vector)
{
	return sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

//normalizacja wektora
void normaliseVector(float* vector)
{
	vectorMul(vector, 1 / lengthOfVector(vector));
}

//tworzenie macierzy obroty 3x3 na podstawie danego kata, osi i wskaznika do macierzy
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

//przemnozenie wektora 1 przez macierz oraz zapisanie wyniku dzialania w wektorze 2
void mulVecBy(float* vector1, float* matrix, float* vector2)
{
	vector2[0] = vector1[0] * matrix[0] + vector1[1] * matrix[1] + vector1[2] * matrix[2];
	vector2[1] = vector1[0] * matrix[3] + vector1[1] * matrix[4] + vector1[2] * matrix[5];
	vector2[2] = vector1[0] * matrix[6] + vector1[1] * matrix[7] + vector1[2] * matrix[8];
}

//obrot wektora1 wokol osi wektora2 o dany kat i zapis rezultatu dzialania do wektora3
void rotateAroundVector(float* vector1, float* vector2, float angle, float* vector3)
{
	//obrot macierzy
	float matrix[16];
	rotationMatrix(matrix, vector2, angle);
	//przemnozenie przez macierz
	mulVecBy(vector1, matrix, vector3);
}

Camera::Camera(void)
{
	cameraSpeed = 0.005f;
	cameraTurnSpeed = 0.01f;
	
	//ustawienie wartosci domyslnych wektora
	//wartosci otrzymane za posrednictwem debuggera
	/*
			forwardVector	0x00ab354c {-0.398769796, 0.763009906, -0.508720219}	float[3]
	+		upVector	0x00ab3564 {-0.235630989, 0.450859368, 0.860931039}	float[3]
	+		rightVector	0x00ab3558 {0.886262059, 0.463184059, 0.000000000}	float[3]
	+		position	0x00ab3570 {0.764331460, -1.66760659, 0.642456770}	float[3]
	*/
	vectorSet(position, 0.764331460f, -1.66760659f, 0.642456770);
	vectorSet(forwardVector, -0.398769796f, 0.763009906f, -0.508720219f);
	vectorSet(rightVector, 0.886262059f, 0.463184059f, 0.000000000f);
	vectorSet(upVector, -0.235630989f, 0.450859368f, 0.860931039f);
}

//przekształcenie macierzy OpenGL na macierze orientacji
void Camera::transformOrientation(void)
{
	//widok w kierunku wektorow orientacji
	gluLookAt(0, 0, 0, forwardVector[0], forwardVector[1], forwardVector[2], upVector[0], upVector[1], upVector[2]);
}

//przekształcenie macierzy OpenGL na macierze translacji
void Camera::transformTranslation(void)
{
	//przesuniecie w celu nasladowania pozycji kamery
	glTranslatef(-position[0], -position[1], -position[2]);
}

//kamera w danym punkcie w przestrzeni 3D
void Camera::pointAt(float* targetVector)
{
	float tempVector[3];
	float up[3] = { 0.0f, 0.0f, 1.0f };
	
	//po pierwsze stworzenie nowego wektora poprzez odjecie od pozycji docelowej pozycji kamery
	forwardVector[0] = targetVector[0] - position[0];
	forwardVector[1] = targetVector[1] - position[2];
	forwardVector[2] = targetVector[1] - position[2];

	//nastepnie normalizacja do dlugosci 1
	normaliseVector(forwardVector);

	//aby odnalezc rightVector nalezy obrocic forwardVector o polowe -PI wzgledem osi Z
	rotateAroundVector(forwardVector, up, -1.57079632679f, tempVector);

	//wyzerowanie skladnika Y, aby uczynic go plaskim
	tempVector[2] = 0;
	
	//normalizacja tempVector
	normaliseVector(tempVector);
	
	//przypisanie go do rightVector
	vectorCopy(rightVector, tempVector);

	//opracowanie upVector poprzez obrot forwardVector o polowe PI wokol rightVector
	rotateAroundVector(forwardVector, rightVector, 1.57079632679f, tempVector);
	vectorCopy(upVector, tempVector);
}

//zwiekszenie predkosci kamery
void Camera::speedUp(void)
{
	if (cameraSpeed < 1.0f)
		cameraSpeed *= 2;
}

//zmniejszenie predkosci kamery
void Camera::slowDown(void)
{
	if (cameraSpeed > 0.000001f)
		cameraSpeed /= 2;
}

//przesuniecie kamery do przodu
void Camera::forward(void)
{
	float vector[3];
	
	//dostowanie wektora poruszania sie do wlasciwej predkosci -> do przodu
	vectorCopy(vector, forwardVector);
	vectorMul(vector, cameraSpeed);

	//dodanie wektora poruszania sie do wektora pozycji
	vectorAdd(position, vector);
}

//przesuniecie kamery do tylu
void Camera::backward(void)
{
	float vector[3];
	
	//dostosowanie wektora poruszania sie do wlasciwej predkosci ruchu do tylu
	vectorCopy(vector, forwardVector);
	vectorMul(vector, -cameraSpeed); //-cameraSpeed, aby poruszac sie do tylu

	//dodanie wektora poruszania sie do wektora pozycji
	vectorAdd(position, vector);
}

//przesuniecie w lewo
void Camera::left(void)
{
	float vector[3];
	
	//podobnie jak poprzednio
	vectorCopy(vector, rightVector);
	vectorMul(vector, -cameraSpeed);

	vectorAdd(position, vector);
}

//przesuniecie w prawo
void Camera::right(void)
{
	float vector[3];
	
	//analogicznie jak poprzednio
	vectorCopy(vector, rightVector);
	vectorMul(vector, cameraSpeed);

	vectorAdd(position, vector);
}

//obrot kamery w prawo
void Camera::rollRight(void)
{
	float tempVector[3];
	
	//obrot wektorow up i right wokol osi wektora forward, w celu wykonania obrotu kamery
	rotateAroundVector(upVector, forwardVector, cameraTurnSpeed, tempVector);
	vectorCopy(upVector, tempVector);
	rotateAroundVector(rightVector, forwardVector, cameraTurnSpeed, tempVector);
	vectorCopy(rightVector, tempVector);
}

//obrot kamery w lewo
void Camera::rollLeft(void)
{
	float tempVector[3];

	//analogicznie jak wyzej
	rotateAroundVector(upVector, forwardVector, -cameraTurnSpeed, tempVector);
	vectorCopy(upVector, tempVector);
	rotateAroundVector(rightVector, forwardVector, -cameraTurnSpeed, tempVector);
	vectorCopy(rightVector, tempVector);
}

//obnizenie kamery
void Camera::pitchDown(void)
{
	float tempVector[3];

	//obrot wektor forward i up wokol osi wektora right
	rotateAroundVector(forwardVector, rightVector, -cameraTurnSpeed, tempVector);
	vectorCopy(forwardVector, tempVector);
	rotateAroundVector(upVector, rightVector, -cameraTurnSpeed, tempVector);
	vectorCopy(upVector, tempVector);
}

//podniesienie kamery
void Camera::pitchUp(void)
{
	float tempVector[3];

	//podobnie jak wyzej
	rotateAroundVector(forwardVector, rightVector, cameraTurnSpeed, tempVector);
	vectorCopy(forwardVector, tempVector);
	rotateAroundVector(upVector, rightVector, cameraTurnSpeed, tempVector);
	vectorCopy(upVector, tempVector);
}

//odchylenie w lewo
void Camera::yawLeft(void)
{
	float tempVector[3];

	//obrot wektorw forward i right wokol osi wektora up
	rotateAroundVector(forwardVector, upVector, cameraTurnSpeed, tempVector);
	vectorCopy(forwardVector, tempVector);
	rotateAroundVector(rightVector, upVector, cameraTurnSpeed, tempVector);
	vectorCopy(rightVector, tempVector);
}

//odchylenie w prawo
void Camera::yawRight(void)
{
	float tempVector[3];

	//analogicznie jak powyzej
	rotateAroundVector(forwardVector, upVector, -cameraTurnSpeed, tempVector);
	vectorCopy(forwardVector, tempVector);
	rotateAroundVector(rightVector, upVector, -cameraTurnSpeed, tempVector);
	vectorCopy(rightVector, tempVector);
}