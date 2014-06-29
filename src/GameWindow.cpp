#include "GameWindow.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "graphics/Ship.h"

Ship* test;

GameWindow::GameWindow()
{
	this->window = nullptr;
	this->renderer = nullptr;
	this->quit = true;
	this->lastframe = 0;
}

GameWindow::~GameWindow()
{
	if (window != nullptr || window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	if (renderer != nullptr || renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
	}

	SDLNet_Quit();
	SDL_Quit();
}


void GameWindow::init()
{
	//Not init'ing all sub-systems resolves a mem-leak detected by valgrind
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "SDL2 failed to init %s" , SDL_GetError());
	if(SDLNet_Init() != 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "SDL_net failed to init %s" , SDL_GetError());

	this->window = SDL_CreateWindow("Bitroids"
		,SDL_WINDOWPOS_CENTERED
		,SDL_WINDOWPOS_CENTERED
		,800
		,600
		,SDL_WINDOW_OPENGL);

	if (this->window == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM,"Could not create window: %s\n", SDL_GetError());
        return;
    }

	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED||SDL_RENDERER_PRESENTVSYNC );
	if (this->renderer == NULL)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM,"Could not create renderer: %s\n", SDL_GetError());
        return;
	}
	this->quit = false;
}

void GameWindow::start()
{
	SDL_Point pt;
	pt.x = 450, pt.y = 10;
	test = new Ship(pt, 0);
	auto time = Time();
	while(!quit)
	{
		time.delta = SDL_TICKS_PASSED(SDL_GetTicks(),this->lastframe);
		time.current = SDL_GetTicks();
		//Events
		SDL_PumpEvents();
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			this->event(event);
		}

		//Update
		this->update(time);

		//Render
		SDL_SetRenderDrawColor(renderer, 5, 5, 5,255);
		if (SDL_RenderClear(this->renderer) == 0)
		{
			this->render(time,this->renderer);
			SDL_RenderPresent(this->renderer);
		} else {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Failed to clear renderer: %s\n", SDL_GetError());
			quit = true;
		}
		this->lastframe = SDL_GetTicks();
	}
	delete test;
}

void GameWindow::update(Time time)
{
	test->setRot(test->getRot() + 1.0);
	test->update(time);
}

void GameWindow::render(Time time, SDL_Renderer* renderer)
{
	//Set Draw Colour
	test->render(time, renderer);

}

void GameWindow::event(SDL_Event& e)
{
	const char* error = SDL_GetError();
	if (!error) {
		SDL_Log( "Uncaught error : %s" , error);
		SDL_ClearError();
	}

	//Window Events
	if (e.type == SDL_WINDOWEVENT)
	{
		switch (e.window.event) {
			case SDL_WINDOWEVENT_CLOSE:
				this->quit = true;
				break;
		}
	}

	//Keyboard events
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
			case (SDLK_ESCAPE):
				this->quit = true;
				break;
		}
	}

	//Quit event
	if (e.type == SDL_QUIT)
	{
		this->quit = true;
	}
}
