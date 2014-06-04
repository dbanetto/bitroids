#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <SDL2/SDL_net.h>

class GameServer
{
	public:
		GameServer();
		virtual ~GameServer();

		int init (Uint16 port);
		void host();
	protected:
	private:
		UDPsocket server_sock;
};

#endif // GAMESERVER_H
