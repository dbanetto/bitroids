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

		GameWindow gm;
		gm.init();
		gm.start();
	}

    return 0;
}
