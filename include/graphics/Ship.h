#ifndef SHIP_H_
#define SHIP_H_

#include "graphics/Drawable.h"
#include "graphics/Bullet.h"

class Ship : public Drawable
{
	public:
		Ship(SDL_Point pos, double rot, int id);
		Bullet fire(int id);
};

#endif // SHIP_H_
