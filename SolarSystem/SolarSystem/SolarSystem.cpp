#include "stdafx.h"
#include "SolarSystem.h"

SolarSystem::SolarSystem()
{

}

//obliczenie polozenia i logiki wszystkich planet
void SolarSystem::calculatePositions(float time)
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].calculatePosition(time);
	}
}

//dodanie planet z ponizszymi parametrami
void SolarSystem::addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	planets.push_back(Planet(distanceFromSun, orbitTime, rotationTime, radius, textureHandle));
}

//dodanie ksiezycow do danej planety
void SolarSystem::addMoon(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	planets[planetIndex].addMoon(distanceFromPlanet, orbitTime, rotationTime, radius, textureHandle);
}

//rendering planet z OpenGL
void SolarSystem::render()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].render();
	}
}

//rendering rysowania orbit
void SolarSystem::renderOrbits()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].renderOrbit();
	}
}

//uzyskanie pozycji w przestrzeni 3D danej planety (okreslonej indeksem na liscie) oraz umieszczenie jej w wektorze 3D
void SolarSystem::getPlanetPosition(int index, float* vector)
{
	planets[index].getPosition(vector);
}

//uzyskanie promienia planety z danym indeksem z listy planet
float SolarSystem::getRadiusOfPlanet(int index)
{
	return planets[index].getRadius();
}