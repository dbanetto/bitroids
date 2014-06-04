#include "GameWindow.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

GameWindow::GameWindow()
{

}

GameWindow::~GameWindow()
{
	SDLNet_Quit();
	SDL_Quit();
}


void GameWindow::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "SDL2 failed to init %s" , SDL_GetError() );
	if (SDLNet_Init() != 0)
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "SDL_net failed to init %s" , SDL_GetError() );
}
