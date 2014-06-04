#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <string>
#include <SDL2/SDL_net.h>

class GameClient
{
	public:
		GameClient();
		virtual ~GameClient();

		int connect(std::string host , Uint8 port);
		void disconnect();
	protected:
	private:
		IPaddress server_addr;
		UDPsocket udpsock;
		int channel;
		bool connected;
};

#endif // GAMECLIENT_H
