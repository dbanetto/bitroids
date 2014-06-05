#include <SDL2/SDL.h>
#include "GameWindow.h"
#include <iostream>

#include "net/GameServer.h"
#include "net/GameClient.h"

#include <string>
#include <vector>

int main(int argc , char** args)
{
	std::vector<std::string> arg = std::vector<std::string>(argc);
	for (int i = 0;  i < argc; i++)
	{
		arg[i] = std::string(args[i]);
	}

	if ( arg.size() > 1 && arg[1] == "--server")
	{
		GameServer gs;
		gs.init(6554);
		gs.host();
	} else {
		GameClient gc;
		gc.connect("localhost" , 6554);
		gc.send( (arg.size() > 1 ? arg[1] : arg[0] ).c_str() );
		SDL_Delay(100);
		char* recv = nullptr;
		if ( gc.recv(&recv) > 0 )
		{
			std::cout << recv << std::endl;
			delete recv;
		}


		GameWindow gm;
		gm.init();
	}

    return 0;
}
