#include <SDL2/SDL.h>
#include "GameWindow.h"
#include <iostream>


#include "net/GameClient.h"

int main(int argc , char** args)
{
	for (int ar = 1;  ar < argc; ar++)
	{
		std::cout << "Argument #" << ar << std::endl;
		for ( char* arg = args[ar]; *arg != '\0' ; arg++ )
		{
			std::cout << *arg;
		}
		std::cout << std::endl;
	}

	GameClient gc;
	gc.connect("localhost" , 6554);

	GameWindow gm;
	gm.init();

    return 0;
}
