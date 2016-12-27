#ifndef MOON_H
#define MOON_H

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32
#include <gl\GL.h>

class Moon
{
private:
	float distanceFromPlanet; //odleglosc od jego planety
	float orbitTime; //czas potrzebny na okrazenie orbity
	float rotationTime; //czas potrzebny na obrót 360stopni
	float radius; //promien samego ksiezyca
	
	GLuint textureHandle; //tekstura uzyta do renderingu
	float position[3]; //polozenie ksiezyca w przestrzeni 3D w stosunku do planety
	float rotation; //obrot ksiezyca wokol wlasnej osi

public:
	//odleglosc - jednostka [km], czas - jednostka [dni ziemskie]
	Moon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	//oblicza pozycje ksiezyca w przestrzeni 3D w stosunku do planety na orbicie za pomoca okreslonej wartosci czasu
	void calculatePosition(float time);
	//rendering na ekran
	void render(void);
	//rendering kragu orbity ksiezycow
	void renderOrbit(void);
};
#endif // !MOON_H
