#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
private:

	float forwardVector[3]; //wektor skierowany w kierunku w którym obserwator stoi
	float rightVector[3]; //wektor skierowany w prawo od miejsca w którym stoi obserwator (opisuje orientacje)
	float upVector[3]; //wektor skierowany w górę, z miejsca w którym stoi obserwator
	float position[3]; //wektor opisujący pozycję kamery

	float cameraSpeed; //określenie prędkości kamery
	float cameraTurnSpeed; //określenie prędkości skrętu kamery

public:

	Camera(void);

	void transformOrientation(void); //przekształcenie macierzy OpenGL na macierze orientacji
	void transformTranslation(void); //przekształcenie macierzy OpenGL na macierze translacji
	void pointAt(float* targetVector); //kamera w danym punkcie w przestrzeni 3D
	void speedUp(void); //zwiększenie prędkości kamery
	void slowDown(void); //zmniejszenie prędkości kamery
	void forward(void); //przesunięcie kamery do przodu
	void left(void); // przesunięce w lewo
	void right(void); //przesunięcie w prawo
	void backward(void); //przesunięcie kamery do tyłu
	void rollRight(void); //obrót kamery w prawo
	void rollLeft(void); //obrót kamery w lewo
	void pitchUp(void); //podniesienie kamery
	void pitchDown(void); //obniżenie kamery
	void yawLeft(void); //odchylenie kamery w lewo
	void yawRight(void); //odchylenie kamery w prawo
};

#endif