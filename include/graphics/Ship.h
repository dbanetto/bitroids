#ifndef SHIP_H_
#define SHIP_H_

#include "graphics/Drawable.h"

class Ship : public Drawable
{
	public:
		Ship(SDL_Point pos, double rot);
};

#endif // SHIP_H_
