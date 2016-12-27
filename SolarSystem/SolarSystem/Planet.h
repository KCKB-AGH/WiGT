#ifndef PLANET_H
#define PLANET_H

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <gl\GL.h>
#include <vector>
#include "Moon.h"
using namespace std;
class Planet
{
private:
	float distanceFromSun; //odlegosc planety od Slonca
	float orbitTime; //czas potrzebny na okrazenie orbity
	float rotationTime; //czas potrzebny na obrot 360stopni
	float radius; //promien planety
	GLuint textureHandle; //tekstura uzyta do renderingu
	float position[3]; //pozycja w przestzeni 3D
	float rotation; //obrot wokol wlasnej osi
	vector<Moon> moons; //lista ksiezycow danej planety

public:
	//odleglosc w km, czas w dniach ziemskich
	Planet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	//oblicza pozycje planety w przestrzeni 3D na orbicie przy uzyciu okreslonej wartosci czasu
	void calculatePosition(float time);
	//rendering na ekran
	void render(void);
	//rendering orbit planet
	void renderOrbit(void);
	//odczytuje pozycje planety w realnej przestrzeni 3D (po przeskalowaniu) i umieszcza w wektorze
	void getPosition(float* vector);
	//uzyskanie promienia planety
	float getRadius(void);
	//dodanie ksiezyca do planety
	void addMoon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);

};
#endif // !PLANET_H
