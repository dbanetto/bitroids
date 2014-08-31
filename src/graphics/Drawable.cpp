#include "graphics/Drawable.h"
#define _USE_MATH_DEFINES
#include <cmath>

Drawable::Drawable(int id)
{
	draw_points = std::vector<SDL_Point>(5);
	base_points = std::vector<SDL_Point>(5);
	this->update_draw_points = true;
	this->id = id;
	this->rotation = 0;
	this->position.x = 0;
	this->position.y = 0;
}

Drawable::~Drawable()
{
}


void Drawable::render(Time delta, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255,255,255,255);
	SDL_RenderDrawLines(renderer , &(this->draw_points[0]), this->draw_points.size());

#ifdef VISUAL_DEBUG
	SDL_RenderDrawRect(renderer, &(this->area));
#endif
}

void Drawable::update(Time delta)
{
	this->posd.x += veld.x*delta.delta/1000.0;
	this->posd.y += veld.y*delta.delta/1000.0;

	if((int)(this->posd.x) != this->position.x || (int)(this->posd.y) != this->position.y) {
		this->position.x = (int)(this->posd.x);
		this->position.y = (int)(this->posd.y);
		update_draw_points = true;
	}

	if(this->update_draw_points) {
		this->update_points();
		this->update_draw_points = false;
	}
}

void Drawable::update_points()
{
	this->draw_points = translate(this->base_points, this->cor, this->rotation, this->position);
	SDL_EnclosePoints(&(this->draw_points[0]), this->draw_points.size(), NULL , &(this->area));
}

//Getters and Setters
SDL_Rect Drawable::getRect()
{
	return this->area;
}
SDL_Point Drawable::getPos()
{
	return this->position;
}
double Drawable::getRot()
{
	return this->rotation;
}

double Drawable::getVel()
{
	return SDL_sqrt(SDL_pow(this->veld.x , 2) + SDL_pow(this->veld.y , 2));
}

int Drawable::getID()
{
	return this->id;
}

void Drawable::setPos(SDL_Point pos)
{
	this->update_draw_points = true;
	this->position = pos;
	this->posd.x = pos.x;
	this->posd.y = pos.y;
}
void Drawable::setRot(double rot)
{
	this->update_draw_points = true;
	this->rotation = rot;
}

void Drawable::setVel(double vel)
{
	this->veld.x = std::cos(this->rotation)*vel;
	this->veld.y = std::sin(this->rotation)*vel;
}

void Drawable::changeVel(double vel, double dir)
{
	this->veld.x += std::cos(dir)*vel;
	this->veld.y += std::sin(dir)*vel;
}

std::vector<SDL_Point> translate(const std::vector<SDL_Point>& points, const SDL_Point& center , double angle, SDL_Point offset)
{
	//Create output vector
	std::vector<SDL_Point> n_points;
	//Reserve the same amount of points
	n_points.reserve(points.size());
	//Pre-calculate sin and cos
	double a_sin = 0;
	double a_cos = 1;
	if(angle == M_PI) {
		a_cos = -1;
	} else if(angle != 0) {
		a_sin = std::sin(angle);
		a_cos = std::cos(angle);
	}

	for(unsigned int i = 0; i < points.size(); i++) {
		SDL_Point pt;
		//Offset so the center is the origin of the points
		pt.x = (points[i].x - center.x);
		pt.y = (points[i].y - center.y);
		//No rotational translation needed if angle = 0
		double py = pt.y;
		double px = pt.x;
		if(angle == M_PI) {
			//Rotate the point around the origin by angle degrees
			pt.x = round(-px - py * a_sin);
			pt.y = round(px * a_sin - py);
		} else if(angle != 0) {
			//Rotate the point around the origin by angle degrees

			pt.x = round(px * a_cos - py * a_sin);
			pt.y = round(px * a_sin + py * a_cos);
		}
		//Returning the points back to before the origin change
		pt.x += center.x + offset.x;
		pt.y += center.y + offset.y;

		//Add them to the output list
		n_points.push_back(pt);
	}
	return n_points;
}
