#include "stdafx.h"
#include "Planet.h"
#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32
#include <gl\GL.h>
#include <gl\GLU.h>
#include "glut.h"
#include "Globals.h"


float planetSizeScale = 0.000005f;

Planet::Planet(float _distanceFromSun, float _orbitTime, float _rotationTime, float _radius, GLuint _textureHandle)
{
	this->distanceFromSun = _distanceFromSun;
	this->orbitTime = _orbitTime;
	this->rotationTime = _rotationTime;
	this->radius = _radius;
	this->textureHandle = _textureHandle;
}

//oblicza pozycje planety w przestrzeni 3D na orbicie przy uzyciu okreslonej wartosci czasu
void Planet::calculatePosition(float time)
{
	//wyznaczenie kata orientacji obrity wokol slonca
	float angle = time * 3.1419f / orbitTime;

	//uzycie triggera w celu znalezienia pozycji w przestrzeni wzgledem planety
	position[0] = sin(angle) * distanceFromSun;
	position[1] = cos(angle) * distanceFromSun;
	position[2] = 0;

	//wyznaczenie obrotu planety wokol wlasnej osi
	rotation = time * 360 / rotationTime;

	//obliczenie pozycji ksiezycow
	for (int i = 0; i < moons.size(); i++)
	{
		moons[i].calculatePosition(time);
	}
}

//rendering na ekran
void Planet::render(void)
{
	glPushMatrix();

	//przesuniecie do wlasciwiej pozycji
	glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);

	//rysowanie ksiezycow
	for (int i = 0; i < moons.size(); i++)
	{
		moons[i].render();
	}

	//obrot w celu krecenia sie planet
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, textureHandle); //bindowanie tekstur planet
	GLUquadricObj* quadric = gluNewQuadric(); //rendering obiektu jako GLU sphere quadric
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	//jezeli dana planeta jest slonce -> wylaczenie swiatla
	if (distanceFromSun < 0.001f)
	{
		float radiusScaled = radius * planetSizeScale;
		if (radiusScaled > 0.5f) radiusScaled = 0.5f;

		glDisable(GL_LIGHTING);
		gluSphere(quadric, radiusScaled, 30, 30);
		glEnable(GL_LIGHTING);
	}
	else
	{
		gluSphere(quadric, radius*planetSizeScale, 30, 30);
	}
	glPopMatrix();
}

//rendering orbit planet
void Planet::renderOrbit(void)
{
	glBegin(GL_LINE_STRIP);

	//petla od 0 do 2PI - obrysowanie promienia orbity przy uzyciu trygonometrii
	for (float angle = 0.0f; angle < 6.283185307f; angle += 0.05f)
	{
		glVertex3f(sin(angle)*distanceFromSun*distanceScale, cos(angle)*distanceFromSun*distanceScale, 0.0f);
	}
	glVertex3f(0.0f, distanceFromSun*distanceScale, 0.0f);
	glEnd();
	
	//rendering orbity ksiezyca
	glPushMatrix();

	//przesuniecie do srodka danej planety w celu narysowania orbity ksiezyca wokol niego
	glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);
	
	//rysowanie orbit wszystkich ksiezycow
	for (int i = 0; i < moons.size(); i++)
	{
		moons[i].renderOrbit();
	}
	glPopMatrix();
}

//odczytuje pozycje planety w przestrzeni 3D (po przeskalowaniu) i umieszczenie w wektorze
void Planet::getPosition(float* vector)
{
	vector[0] = position[0] * distanceScale;
	vector[1] = position[1] * distanceScale;
	vector[2] = position[2] * distanceScale;
}

//uzyskanie promienia planety
float Planet::getRadius(void)
{
	return radius;
}

//dodawanie ksiezyca do planety
void Planet::addMoon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	moons.push_back(Moon(distanceFromPlanet, orbitTime, rotationTime, radius, textureHandle));
}