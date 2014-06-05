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
	//Not init'ing all sub-systems resolves a mem-leak detected by valgrind
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0)
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "SDL2 failed to init %s" , SDL_GetError() );
	if (SDLNet_Init() != 0)
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "SDL_net failed to init %s" , SDL_GetError() );
}
