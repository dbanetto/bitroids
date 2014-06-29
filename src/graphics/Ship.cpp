#include "graphics/Ship.h"

Ship::Ship(SDL_Point pos, double rot)
	: Drawable()
{
	this->position = pos;

	SDL_Point pt;
	pt.x = 0; pt.y = 0;
	this->base_points.push_back(pt);

	pt.x = 12; pt.y = 16;
	this->base_points.push_back(pt);

	pt.x = 0; pt.y = 32;
	this->base_points.push_back(pt);

	pt.x = 32; pt.y = 16;
	this->base_points.push_back(pt);

	pt.x = 0; pt.y = 0;
	this->base_points.push_back(pt);

	this->update_points();
	this->cor.x = this->area.w / 2;
	this->cor.y = this->area.h / 2;

	this->x = motion();
	this->x.start_time = 0; this->x.gradient = -0.005; this->x.constant = pos.x;
	this->y = motion();
	this->y.start_time = 0; this->y.gradient = -0.009; this->y.constant = pos.y;
}