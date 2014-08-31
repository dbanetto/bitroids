#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <map>
#include <SDL2/SDL.h>
#include "timer.h"
#include "net/GameClient.h"

#include "graphics/Ship.h"
#include "graphics/Bullet.h"
#include "graphics/Asteroid.h"

class GameWindow
{
	public:
		GameWindow();
		virtual ~GameWindow();

		void init();
		void start();

		void update(Time delta);
		void render(Time delta, SDL_Renderer* renderer);
		void event (Time delta, SDL_Event& event);
	protected:
	private:
		GameClient client;
		Ship* player;

		int bullet_id;
		int ship_id;
		int asteroid_id;

		std::map<int,Ship> ships;
		std::map<int,Asteroid> asteroids;
		std::map<int,Bullet> bullets;
		SDL_Thread* client_thread;
		SDL_Window* window;
		SDL_Renderer* renderer;
		bool quit;
		Uint32 lastframe;
};

#endif // GAMEWINDOW_H
