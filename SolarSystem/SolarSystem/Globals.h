#ifndef GLOBALS_H
#define GLOBALS_H

//przeskalowanie odleglosci planet w symulacji.
//przykladowo gdy odlegosc planety od Slonca wynosi 149600000km, a skala jak ponizej to w przestrzeni 3D wartosc wyniesie 1,496 jednostek
const float distanceScale = 0.00000001f;
//przeskalowanie wielkosci planet w symulacji.
//przykladowo gdy promien planety wynosi 6,371km a skala np. 0,0001f, to promien planety w symulacji wyniesie 0,6371 jednostek
extern float planetSizeScale;
#endif // !GLOBALS_H
