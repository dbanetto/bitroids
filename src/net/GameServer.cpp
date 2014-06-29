#include "net/GameServer.h"

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
			SDL_Log("Packet Revived");
			SDL_Log("From : %d:%d" , packet->address.host , packet->address.port);
			SDL_Log("Data (%d/%d) CRC16: %d", packet->len , packet->maxlen , checksumCRC16( packet->data , packet->len));

			GamePacket pk = GamePacket();
			unpackpacket(&packet, &pk);
			SDL_Log("Packet Len:%d Type:%d Time:%d CRC:%d\n%s", pk.len , pk.type , pk.timestamp, pk.checksum , (char*)(pk.data));
			delete pk.data;
			if ( SDLNet_UDP_Send( this->server_sock , packet->channel, packet ) == 0 )
			{
				SDL_LogError( SDL_LOG_CATEGORY_APPLICATION , "Failed to send UPD packet to %d:%d %s" , packet->address.host , packet->address.port , SDL_GetError() );
			} else {
				SDL_Log("Sent packet to %d:%d" , packet->address.host , packet->address.port);
			}
		}
	}

	SDLNet_FreePacket(packet);
}
