#ifndef TGA_H
#define TGA_H

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32
#include <gl\GL.h>

class TGA
{
private:
	GLuint textureHandle; //uchwyt do tekstury w OpenGL

public:
	TGA(char* imagePath); //zbudowanie i zaladownie tekstury TGA do OpenGL z danej sciezki pliku
	GLuint getTextureHandle(void); //zwraca uchwyt do struktury utworzonej z obrazu w celu powiaznia z OpenGL
};
#endif // !TGA_H
