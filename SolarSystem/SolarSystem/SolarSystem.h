#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32
#include <gl\GL.h>
#include <vector>
#include "Planet.h"
using namespace std;

class SolarSystem
{
private:
	vector<Planet> planets;

public:
	SolarSystem();
	//obliczenie polozenia i logiki wszystkich planet
	void calculatePositions(float time);
	//dodanie planet z ponizszymi parametrami
	void addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	//dodanie ksiezycow do danej planety
	void addMoon(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	//rendering planet z OpenGL
	void render();
	//rendering rysowania orbit
	void renderOrbits();
	//uzyskanie pozycji w przestrzeni 3D danej planety (okreslonej indeksem na liscie) oraz umieszczenie jej w wektorze 3D
	void getPlanetPosition(int index, float* vector);
	//uzyskanie promienia planety z danym indeksem z listy planet
	float getRadiusOfPlanet(int index);
};
#endif // !SOLARSYSTEM_H
