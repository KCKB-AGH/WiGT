#include "stdafx.h"
#include "Moon.h"
#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <gl\GL.h>
#include <gl\GLU.h>
#include "glut.h"
#include "Globals.h"

Moon::Moon(float _distanceFromPlanet, float _orbitTime, float _rotationTime, float _radius, GLuint _textureHandle)
{
	this->distanceFromPlanet = _distanceFromPlanet;
	this->orbitTime = _orbitTime;
	this->rotationTime = _rotationTime;
	this->radius = _radius;
	this->textureHandle = _textureHandle;
}
//oblicza pozycje ksiezyca w przestrzeni 3D w stosunku do planety na orbicie za pomoca okreslonej wartosci czasu
void Moon::calculatePosition(float time)
{
	//wyznaczenie kata orientacji obrity wokol planety
	float angle = time * 3.1419f / orbitTime;

	//użycie triggera w celu znalezienia pozycji w przestrzeni wzgledem planety
	position[0] = sin(angle) * distanceFromPlanet;
	position[1] = cos(angle) * distanceFromPlanet;
	position[2] = 0;

	//wyznaczenie obrotu ksiezyca wokol wlasnej osi
	rotation = time * 360 / rotationTime;
}

void Moon::render(void)
{
	glPushMatrix();
	
	glBindTexture(GL_TEXTURE_2D, textureHandle); //zbindowanie tekstury ksiezyca

	//przesuniecie do wlasciwiej pozycji i obrot dla krecenia sie ksiezyca
	glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);
	glRotatef(-rotation, 0.0f, 0.0f, 1.0f);

	//renderowanie jako object GLU - quadric sphere
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	gluSphere(quadric, radius*planetSizeScale, 30, 30);

	glPopMatrix();
}

void Moon::renderOrbit(void)
{
	//rysowanie za pomoca pasow linii
	glBegin(GL_LINE_STRIP);

	//petla od 0 do 2PI - obrysowanie promienia orbity przy uzyciu trygonometrii
	for (float angle = 0.0f; angle < 6.283185307f; angle += 0.1f)
	{
		glVertex3f(sin(angle)*distanceFromPlanet*distanceScale, cos(angle)*distanceFromPlanet*distanceScale, 0.0f);
	}
	glVertex3f(0.0f, distanceFromPlanet*distanceScale, 0.0f);
	glEnd();
}