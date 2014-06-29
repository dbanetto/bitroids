#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL2/SDL.h>
#include "timer.h"

class GameWindow
{
	public:
		GameWindow();
		virtual ~GameWindow();

		void init();
		void start();

		void update(Time delta);
		void render(Time delta, SDL_Renderer* renderer);
		void event (SDL_Event& event);
	protected:
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		bool quit;
		Uint32 lastframe;
};

#endif // GAMEWINDOW_H
