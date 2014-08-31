#include "graphics/Ship.h"

Ship::Ship(SDL_Point pos, double rot, int id)
	: Drawable(id)
{
	this->position = pos;

	SDL_Point pt;
	pt.x = 0;
	pt.y = 0;
	this->base_points.push_back(pt);

	pt.x = 12;
	pt.y = 16;
	this->base_points.push_back(pt);

	pt.x = 0;
	pt.y = 32;
	this->base_points.push_back(pt);

	pt.x = 32;
	pt.y = 16;
	this->base_points.push_back(pt);

	pt.x = 0;
	pt.y = 0;
	this->base_points.push_back(pt);

	this->update_points();
	this->cor.x = this->area.w / 2;
	this->cor.y = this->area.h / 2;

	this->setVel(0);
}

Bullet Ship::fire(int id)
{
	return Bullet(this->position,this->getVel() + 500.0 , this->rotation, id);
}
