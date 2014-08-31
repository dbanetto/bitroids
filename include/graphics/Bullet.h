#ifndef BULLET_H_
#define BULLET_H_

#include "graphics/Drawable.h"

class Bullet : public Drawable
{
	public:
		Bullet(SDL_Point pos, double velocity, double rot, int id);
};

#endif // BULLET_H_
