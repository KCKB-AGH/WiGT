#include "stdafx.h"
#include <cmath>
#include <cstdlib>
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32
#include <gl\GL.h>
#include <gl\GLU.h>
#include "glut.h"

#include "TGA.h"
#include "SolarSystem.h"
#include "Camera.h"
#include "Globals.h"

//wielkosc ekranu
int screenWidth, screenHeight;

//tekstury TGA -> okno pomocy, droga mleczna i ksiezyc
TGA* help, *stars, *moon;
//przelacznik, jesli okno pomocy jest wlaczone
bool helpDialogue = true;
//przelacznik, jesli orbity sa widoczne
bool showOrbits = true;
//przechowuje indeks ostatnio wybranej planety -> od 1 do 9 na klawiaturze
int planetSelected = 1;

//glowna instacja ukladu slonecznego
SolarSystem solarSystem;

//instancja kamery
Camera camera;

//zmienne kontrolujace symulacje czasu
double time;
double timeSpeed;

//przechowuje stan sterownikow kamery -> 
//jezeli prawda -> wartosc danej kontrolki oznacza ze jest wcisnieta
struct ControlStates
{
	bool forward, backward, left, right, yawLeft, yawRight, pitchUp,
		pitchDown, rollLeft, rollRight;
} controls;


// funkcja timera wywolywana co 10ms lub czesciej
void timer(int)
{
	glutPostRedisplay(); // post for display func
	glutTimerFunc(10, timer, 0); // limit rysowania do 100 fps
}

// tworzy liczbe losowa do okreslonej wartosci maksymalnej
float random(float max)
{
	return (float)(std::rand() % 1000) * max * 0.001;
}

// dodawanie ksiezyca do wybranej planety
void addMoon()
{
	// stworzenie ksiezyca za pomoca wartosci losowych
	solarSystem.addMoon(planetSelected,
		(500 + random(1500)) * solarSystem.getRadiusOfPlanet(planetSelected),
		10 + random(100), 0.5 + random(20),
		solarSystem.getRadiusOfPlanet(planetSelected) * (0.05f + random(0.2f)), moon->getTextureHandle());
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	//ustawienie oswietlenia
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat matAmbience[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat matShininess[] = { 20.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbience);

	GLfloat lightAmbient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	//zaladowanie wszystkich obrazow TGA
	help = new TGA("images/help.tga");
	stars = new TGA("images/stars.tga");
	moon = new TGA("images/moon.tga");

	TGA* sun = new TGA("images/sun.tga");
	TGA* mercury = new TGA("images/mercury.tga");
	TGA* venus = new TGA("images/venus.tga");
	TGA* earth = new TGA("images/earth.tga");
	TGA* mars = new TGA("images/mars.tga");
	TGA* jupiter = new TGA("images/jupiter.tga");
	TGA* saturn = new TGA("images/saturn.tga");
	TGA* uranus = new TGA("images/uranus.tga");
	TGA* neptune = new TGA("images/neptune.tga");
	TGA* pluto = new TGA("images/pluto.tga");

	// dodanie wszystkich planet z dokladnymi danymi. odleglosc -> km, czas -> dni ziemskie
	solarSystem.addPlanet(0, 1, 500, 695500, sun->getTextureHandle()); // slonce
	solarSystem.addPlanet(57910000, 88, 58.6, 2440, mercury->getTextureHandle()); // merkury
	solarSystem.addPlanet(108200000, 224.65, 243, 6052, venus->getTextureHandle()); // wenus
	solarSystem.addPlanet(149600000, 365, 1, 6371, earth->getTextureHandle()); // ziemia
	solarSystem.addPlanet(227939100, 686, 1.03f, 3389, mars->getTextureHandle()); // mars
	solarSystem.addPlanet(778500000, 4332, 0.4139, 69911, jupiter->getTextureHandle()); // jowisz
	solarSystem.addPlanet(1433000000, 10759, 0.44375, 58232, saturn->getTextureHandle()); // saturn
	solarSystem.addPlanet(2877000000, 30685, 0.718056, 25362, uranus->getTextureHandle()); // uran
	solarSystem.addPlanet(4503000000, 60188, 0.6713, 24622, neptune->getTextureHandle()); // neptun
	solarSystem.addPlanet(5906380000, 90616, 6.39, 1137, pluto->getTextureHandle()); // pluton

	solarSystem.addMoon(3, 7000000, 27.3, 27.3, 1738, moon->getTextureHandle()); // testowy ksiezyc dla ziemi

																				 // ustawienie czasu
	time = 2.552f;
	timeSpeed = 0.1f;

	//kontrolki resetu
	controls.forward = false;
	controls.backward = false;
	controls.left = false;
	controls.right = false;
	controls.rollRight = false;
	controls.rollLeft = false;
	controls.pitchDown = false;
	controls.pitchUp = false;
	controls.yawLeft = false;
	controls.yawRight = false;

	timer(0);
}

void drawCube(void);

void display(void)
{
	// aktualizacja logiki i symulacji
	time += timeSpeed;
	solarSystem.calculatePositions(time);

	if (controls.forward) camera.forward();		if (controls.backward) camera.backward();
	if (controls.left) camera.left();			if (controls.right) camera.right();
	if (controls.yawLeft) camera.yawLeft();		if (controls.yawRight) camera.yawRight();
	if (controls.rollLeft) camera.rollLeft();	if (controls.rollRight) camera.rollRight();
	if (controls.pitchUp) camera.pitchUp();		if (controls.pitchDown) camera.pitchDown();

	//czyszczenie buforow
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	//ustawienie macierzy perspektywy dla renderingu
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, (float)screenWidth / (float)screenHeight, 0.001f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// wykonanie przekształcenia macierzy OpenGL na macierze orientacji
	camera.transformOrientation();

	// rysowanie skyboxa
	glBindTexture(GL_TEXTURE_2D, stars->getTextureHandle());
	drawCube();

	// wykonanie przekształcenia macierzy OpenGL na macierze translacji
	camera.transformTranslation();



	GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//rendering ukladu slonecznego
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	solarSystem.render();
	glDisable(GL_LIGHTING);

	// rendering orbit
	if (showOrbits)
		solarSystem.renderOrbits();

	glDisable(GL_DEPTH_TEST);

	// skonfigurowanie macierzy do wyswietlania interfejsu uzytkownika (okna pomocy)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)screenWidth, (GLdouble)screenHeight, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//rysowanie okna pomocy
	if (helpDialogue)
	{
		glBindTexture(GL_TEXTURE_2D, help->getTextureHandle());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
		glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}

	glFlush();
	glutSwapBuffers();
}
void keyDown(unsigned char key, int x, int y)
{
	// sprawdzenie klawiszy numerycznych
	if (key > '0' && key <= '9')
	{
		// widok na wybrana planete
		float vec[3];
		solarSystem.getPlanetPosition(key - '0', vec);
		camera.pointAt(vec);

		//wybor planety
		planetSelected = key - '0';
	}
	switch (key)
	{
	case '-':
		timeSpeed /= 2.0f; // polowka tempa uplywu czasu
		break;
	case '=':
		timeSpeed *= 2.0f; // podwojenie szybkosci uplywu czasu
		break;
	case 'h':
		helpDialogue = !helpDialogue; // pokaz/ukryj okno pomocy
		break;
	case '[':
		planetSizeScale /= 1.2; // zmniejszenie skalowania planet
		break;
	case ']':
		planetSizeScale *= 1.2; // zwiekszenie skalowania planet
		break;
	case 'o':
		showOrbits = !showOrbits; // pokaz/ukryj orbity
		break;
	case 'm':
		addMoon(); // dodanie ksiezyca do wybranej planety
		break;
	case 'r':
		planetSizeScale = distanceScale;
		break;
	case ',':
		camera.slowDown(); // spowolnienie kamery
		break;
	case '.':
		camera.speedUp(); // przyspieszenie kamery
		break;
		//ponizej obsluga kamery
	case 'w':
		controls.forward = true;
		break;
	case 's':
		controls.backward = true;
		break;
	case 'a':
		controls.left = true;
		break;
	case 'd':
		controls.right = true;
		break;
	case 'l':
		controls.rollRight = true;
		break;
	case 'j':
		controls.rollLeft = true;
		break;
	case 'i':
		controls.pitchDown = true;
		break;
	case 'k':
		controls.pitchUp = true;
		break;
	case 'q':
		controls.yawLeft = true;
		break;
	case 'e':
		controls.yawRight = true;
		break;
	}

}

void keyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		controls.forward = false;
		break;
	case 's':
		controls.backward = false;
		break;
	case 'a':
		controls.left = false;
		break;
	case 'd':
		controls.right = false;
		break;
	case 'l':
		controls.rollRight = false;
		break;
	case 'j':
		controls.rollLeft = false;
		break;
	case 'i':
		controls.pitchDown = false;
		break;
	case 'k':
		controls.pitchUp = false;
		break;
	case 'q':
		controls.yawLeft = false;
		break;
	case 'e':
		controls.yawRight = false;
		break;
	}
}

void reshape(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 700);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMainLoop();
	return 0;
}

void drawCube(void)
{
	glBegin(GL_QUADS);
	//new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);
	//new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(1.0f, -1.0f, 1.0f);
	//new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(1.0f, -1.0f, -1.0f);
	//new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
	//new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);
	//new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();
}