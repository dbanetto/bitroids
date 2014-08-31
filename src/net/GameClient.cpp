#include "net/GameClient.h"
#include <SDL2/SDL.h>
#include "net/GamePacket.h"

#include <iostream>

GameClient::GameClient()
{
	this->channel = -1;
	this->connected = false;
	this->packet = nullptr;
	this->udpsock = nullptr;
	event_type = SDL_RegisterEvents(1);
}

GameClient::~GameClient()
{
	if(this->connected)
		this->disconnect();
	if(this->udpsock != nullptr)
		SDLNet_UDP_Close(this->udpsock);
}

/**
 * \brief Connect to server
 * \param IP address or hostname
 * \param Port number to connect to
 */
int GameClient::connect(std::string host , Uint16 port)
{
	if((this->udpsock = SDLNet_UDP_Open(0)) == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "Failed create client UPD socket %s" , SDL_GetError());
		return -1;
	}

	if(SDLNet_ResolveHost(&(this->server_addr) , host.c_str() , port) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "Failed to resolve %s:%d" , host.c_str() , port);
		return -1;
	}

	this->channel = SDLNet_UDP_Bind(this->udpsock , -1 , &(this->server_addr));
	if(this->channel == -1) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "Failed to bind UPD socket to %s:%d %s" , host.c_str() , port , SDL_GetError());
		return -1;
	}

	this->connected = true;

	SDL_Log("Started client socket on channel %d" , this->channel);
	SDL_Log("Connected to  %s:%d (%d:%d)",  host.c_str() , port , this->server_addr.host, this->server_addr.port);
	return 0;
}

/** \brief Disconnect from the current server
 *	Can reconnect to another server
 */
void GameClient::disconnect()
{
	if(!this->connected)
		return;

	if(this->packet != nullptr)


		SDLNet_UDP_Unbind(this->udpsock , this->channel);
	this->connected = false;
}

/** \brief Sends a message to the connected server
 *
 * \param message to be send
 * \return -1 on failure see log output. Returns status of UDPpacket on success
 */
int GameClient::send(const char* msg)
{
	return this->send(msg , 255 , strlen((char *)msg) + 1);
}

int GameClient::send(const char* msg , Uint8 type)
{
	return this->send(msg , type , strlen((char *)msg) + 1);
}

int GameClient::send(const char* msg , Uint8 type , Uint16 length)
{
	int len = length;

	buildpacket((Uint8*)(msg) , len, type, 0 , &packet);

	//Set up the packet data
	packet->address.host = this->server_addr.host;
	packet->address.port = this->server_addr.port;

	//Send the packet and check for an error
	if(SDLNet_UDP_Send(this->udpsock , this->channel , packet) == 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "Failed to send UPD packet to %d:%d %s" , packet->address.host , packet->address.port , SDL_GetError());
		return -1;
	}
	int status = packet->status;
	SDLNet_FreePacket(this->packet); //FREEDOM!

	return status;
}

/** \brief
 *
 * \param
 * \param
 * \return
 *
 */
int GameClient::recv(Uint8** msg)
{
	UDPpacket packet;
	packet.maxlen = 512;
	packet.data = new Uint8[packet.maxlen];

	int numrecv = SDLNet_UDP_Recv(udpsock, &packet);
	if(numrecv == 1) {
		*msg = (packet.data);
		SDL_Log("Received (%d/%d) from %d:%d" , packet.len , packet.maxlen , packet.address);
		return packet.len;
	}

	//No packets to received, give back the memory
	delete packet.data;
	if(numrecv == -1) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "Failed to receive UPD packet %s" , SDL_GetError());
		return -1;
	}

	return 0;
}

int GameClient::start()
{
	while(this->connected) {
		Uint8* rec = nullptr;
		if(this->recv(&rec) > 0) {
			GamePacket* pk = new GamePacket();
			unpackpacket(&rec, pk);

			SDL_Event event;
			SDL_zero(event);
			event.type = this->event_type;
			event.user.code = pk->type;
			event.user.data1 = (void*)(pk);
			event.user.data2 = nullptr;
			SDL_PushEvent(&event);

			delete rec;
		}
	}
	return 0;
}
