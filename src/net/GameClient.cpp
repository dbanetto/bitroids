#include "GameClient.h"
#include <SDL2/SDL.h>

GameClient::GameClient()
{
	this->channel = -1;
	this->connected = false;
}

GameClient::~GameClient()
{
	if (this->connected)
		this->disconnect();
}


int GameClient::connect (std::string host , Uint8 port)
{
	if (SDLNet_ResolveHost( &(this->server_addr) , host.c_str() , port ) != 0)
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "Failed to resolve %s:%d" , host.c_str() , port );
		return -1;
	}

	this->udpsock = SDLNet_UDP_Open(0);
	if ( udpsock == NULL)
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "Failed create client UPD socket %s" , SDL_GetError() );
		return -1;
	}

	this->channel = SDLNet_UDP_Bind( this->udpsock , -1 , &(this->server_addr) );
	if (this->channel == -1)
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "Failed to bind UPD socket to %s:%d %s" , host.c_str() , port , SDL_GetError() );
		return -1;
	}

	this->connected = true;
	return 0;
}

void GameClient::disconnect()
{
	if (!this->connected)
		return;

	SDLNet_UDP_Unbind (this->udpsock , this->channel);
	SDLNet_UDP_Close (this->udpsock);
}
