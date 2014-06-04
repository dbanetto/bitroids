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


int GameClient::connect (std::string host , Uint16 port)
{
	if ( (this->udpsock = SDLNet_UDP_Open(0)) == NULL)
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "Failed create client UPD socket %s" , SDL_GetError() );
		return -1;
	}

	if (SDLNet_ResolveHost( &(this->server_addr) , host.c_str() , port ) != 0)
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "Failed to resolve %s:%d" , host.c_str() , port );
		return -1;
	}

	this->channel = SDLNet_UDP_Bind( this->udpsock , -1 , &(this->server_addr) );
	if (this->channel == -1)
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "Failed to bind UPD socket to %s:%d %s" , host.c_str() , port , SDL_GetError() );
		return -1;
	}


	UDPpacket* packet;
	packet = SDLNet_AllocPacket(512);
	packet->data = (Uint8*)("Hello World");
	packet->len = strlen((char *)packet->data) + 1;
	packet->address.host = this->server_addr.host;
	packet->address.port = this->server_addr.port;
	if ( SDLNet_UDP_Send(this->udpsock , this->channel , packet ) == NULL )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "Failed to send UPD packet to %s:%d %s" , host.c_str() , port , SDL_GetError() );
		return -1;
	}
	//SDLNet_FreePacket(packet);
	this->connected = true;
	SDL_Log("Connected to  %s:%d",  host.c_str() , port);
	return 0;
}

void GameClient::disconnect()
{
	if (!this->connected)
		return;

	SDLNet_UDP_Unbind (this->udpsock , this->channel);
	SDLNet_UDP_Close (this->udpsock);
}
