#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <string>
#include <SDL2/SDL_net.h>

class GameClient
{
	public:
		GameClient();
		virtual ~GameClient();

		int connect(std::string host , Uint16 port);
		void disconnect();

		int send (const char* msg);
		int recv (char** msg);
	protected:
	private:
		IPaddress server_addr;
		UDPsocket udpsock;
		int channel;
		bool connected;
		UDPpacket* packet;
};

#endif // GAMECLIENT_H
