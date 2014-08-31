#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <SDL2/SDL.h>
#include <vector>
#include "timer.h"

struct Position {
	double x;
	double y;

	Position () {
		x = 0.0; y = 0.0;
	}
};

class Drawable
{
	public:
		Drawable(int id);
		virtual ~Drawable();

		virtual void render(Time delta, SDL_Renderer* renderer);
		virtual void update(Time delta);

		// Get Rectangle (Area)
		SDL_Rect getRect();
		//Get Position
		SDL_Point getPos();
		// Get Rotation
		double getRot();

		//Get Vecolicty
		double getVel();

		void setPos(SDL_Point pos);
		void setRot(double rot);
		void setVel(double vel);

		void changeVel(double vel, double dir);

		int getID();
	protected:
		int id;

		Position veld;
		Position posd;

		//Center of rotation
		SDL_Point cor;
		//Rotation in rads
		double rotation;

		SDL_Rect area;
		SDL_Point position;

		std::vector<SDL_Point> draw_points;
		std::vector<SDL_Point> base_points;
		bool update_draw_points;

		void update_points();
};

std::vector<SDL_Point> translate(const std::vector<SDL_Point>& points, const SDL_Point& center , double angle, SDL_Point offset);

#endif // DRAWABLE_H_
