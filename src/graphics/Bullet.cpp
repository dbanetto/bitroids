#include "graphics/Bullet.h"
#include <iostream>

Bullet::Bullet(SDL_Point pos, double velocity, double rot, int id)
	: Drawable(id)
{
	this->setRot(rot);
	this->setVel(velocity);
	this->setPos(pos);

	SDL_Point pt;
	pt.x = 0; pt.y = 0;
	this->base_points.push_back(pt);

	pt.x = 0; pt.y = 4;
	this->base_points.push_back(pt);

	pt.x = 8; pt.y = 4;
	this->base_points.push_back(pt);

	pt.x = 8; pt.y = 0;
	this->base_points.push_back(pt);
	
	pt.x = 0; pt.y = 0;
	this->base_points.push_back(pt);

	this->cor.x = this->area.w / 2;
	this->cor.y = this->area.h / 2;
	this->update_points();

	std::cout << this->getID() << " : " << this->getVel() << " " << this->getRot() << std::endl;
}
