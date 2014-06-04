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
	UDPpacket* packet = nullptr;
	if ( (packet = SDLNet_AllocPacket(512)) == NULL )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "Failed create server UPD packet %s" , SDL_GetError() );
		return;
	}
	bool quit = false;
	while (!quit)
	{
		if (SDLNet_UDP_Recv(this->server_sock, packet))
		{
			SDL_Log("UDP Packet Revived");
			SDL_Log("DATA : %s" , (char*)(packet->data));
			SDL_Log("From : %d:%d" , packet->address.host , packet->address.port );
		}
	}
}
