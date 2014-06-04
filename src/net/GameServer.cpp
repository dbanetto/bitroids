#include "GameServer.h"

GameServer::GameServer()
{

}

GameServer::~GameServer()
{

}

int GameServer::init(Uint16 port)
{
	this->server_sock = SDLNet_UDP_Open(port);
	if (this->server_sock == NULL)
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "Failed create server UPD socket %s" , SDL_GetError() );
		return -1;
	}

	return 0;
}

void GameServer::host()
{

}
