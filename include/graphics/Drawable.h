#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <SDL2/SDL.h>
#include <vector>
#include "timer.h"

struct motion
{
	Uint32 start_time;
	double gradient; //units per millsecond
	int constant;
};

class Drawable
{
	public:
		Drawable();
		virtual ~Drawable();

		virtual void render(Time delta, SDL_Renderer* renderer);
		virtual void update(Time delta);

		SDL_Rect getRect();
		SDL_Point getPos();
		double getRot();

		void setPos(SDL_Point pos);
		void setRot(double rot);

	protected:
		motion x, y;

		//Center of rotation
		SDL_Point cor;
		//Rotation in degs
		double rotation;

		SDL_Rect area;
		SDL_Point position;

		std::vector<SDL_Point> draw_points;
		std::vector<SDL_Point> base_points;
		bool update_draw_points;

		void update_points(void);
};

std::vector<SDL_Point> translate(const std::vector<SDL_Point>& points, const SDL_Point& center , double angle, SDL_Point offset);

#endif // DRAWABLE_H_
