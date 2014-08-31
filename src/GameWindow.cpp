#include "GameWindow.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "net/GamePacket.h"

#include <sstream>
#include <iostream>

int client_thread_starter(void* data)
{
	auto client = (GameClient*)data;
	return client->start();
}

GameWindow::GameWindow()
{
	this->window = nullptr;
	this->renderer = nullptr;
	this->quit = true;
	this->lastframe = 0;
	this->client_thread = nullptr;
	this->player = nullptr;
	this->bullet_id = 0;
	this->ship_id = 0;
	this->asteroid_id = 0;
}

GameWindow::~GameWindow()
{
	if(window != nullptr || window != NULL) {
		SDL_DestroyWindow(window);
	}

	if(renderer != nullptr || renderer != NULL) {
		SDL_DestroyRenderer(renderer);
	}

	SDLNet_Quit();
	SDL_Quit();
}


void GameWindow::init()
{
	//Not init'ing all sub-systems resolves a mem-leak detected by valgrind
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "SDL2 failed to init %s" , SDL_GetError());
	if(SDLNet_Init() != 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "SDL_net failed to init %s" , SDL_GetError());

	this->window = SDL_CreateWindow("Bitroids"
	                                ,SDL_WINDOWPOS_CENTERED
	                                ,SDL_WINDOWPOS_CENTERED
	                                ,800
	                                ,600
	                                ,SDL_WINDOW_OPENGL);

	if(this->window == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM,"Could not create window: %s\n", SDL_GetError());
		return;
	}

	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED||SDL_RENDERER_PRESENTVSYNC);
	if(this->renderer == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM,"Could not create renderer: %s\n", SDL_GetError());
		return;
	}
	this->quit = false;
}

void GameWindow::start()
{
	//Network
	this->client.connect("localhost" , 6554);
	this->client.send("This is a test message using only ASCII Characters", 0x80);
	client_thread = SDL_CreateThread(client_thread_starter, "Client Thread", (void *)(&client));
	SDL_Point pt;
	pt.x = 0;
	pt.y = 0;
	this->player = new Ship(pt,0.0f,ship_id++);
	this->ships.insert(std::pair<int,Ship>(player->getID(), *player));

	auto time = Time();
	while(!quit) {
		time.delta = SDL_TICKS_PASSED(SDL_GetTicks(),this->lastframe);
		time.current = SDL_GetTicks();
		//Events
		SDL_PumpEvents();
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			this->event(time, event);
		}

		//Update
		this->update(time);

		//Render
		SDL_SetRenderDrawColor(renderer, 5, 5, 5,255);
		if(SDL_RenderClear(this->renderer) == 0) {
			this->render(time,this->renderer);
			SDL_RenderPresent(this->renderer);
		} else {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Failed to clear renderer: %s\n", SDL_GetError());
			quit = true;
		}
		this->lastframe = SDL_GetTicks();
		std::stringstream ss;
		ss << "Bitroids (" << 1.0 / ((double)(time.delta) + 0.0) << ')';
		SDL_SetWindowTitle(this->window, ss.str().c_str());
	}

	this->client.disconnect();
	SDL_DetachThread(client_thread);
	delete this->player;
	this->player = nullptr;
}

void GameWindow::update(Time time)
{
	if(player != nullptr) {
		player->update(time);
	}

	for(auto& bullet : this->bullets) {
		bullet.second.update(time);
	}

	for(auto& aster : this->asteroids) {
		aster.second.update(time);
	}
}

void GameWindow::render(Time time, SDL_Renderer* renderer)
{
	if(player != nullptr)
		player->render(time, renderer);

	for(auto& bullet : this->bullets) {
		bullet.second.render(time, renderer);
	}

	for(auto& aster : this->asteroids) {
		aster.second.render(time, renderer);
	}
}

void GameWindow::event(Time time, SDL_Event& e)
{
	const char* error = SDL_GetError();
	if(!error) {
		SDL_Log("Uncaught error : %s" , error);
		SDL_ClearError();
	}

	//Window Events
	if(e.type == SDL_WINDOWEVENT) {
		switch(e.window.event) {
			case SDL_WINDOWEVENT_CLOSE:
				this->quit = true;
				break;
		}
	}

	// ESC key
	if(e.type == SDL_KEYDOWN) {
		switch(e.key.keysym.sym) {
			case(SDLK_ESCAPE):
				this->quit = true;
				break;
			case(SDLK_p):
				this->asteroids.insert(std::pair<int,Asteroid>(asteroid_id , Asteroid(player->getPos(), 0.0 , asteroid_id)));
				asteroid_id++;
				break;
			case(SDLK_SPACE):
				// FIXME : Limit the number of bullets that can be fired per ship
				if(player != nullptr) {
					this->bullets.insert(std::pair<int,Bullet>(bullet_id , player->fire(bullet_id)));
					bullet_id++;
					this->client.send("FIRE_MISSILE!");
				}
				break;
		}
	}

	// Player controls
	if(player != nullptr) {
		const Uint8* keystates = SDL_GetKeyboardState(NULL);

		if(keystates[SDL_SCANCODE_W]) {
			player->changeVel((100.0*time.delta / 100.0) , player->getRot());
		} else if(keystates[SDL_SCANCODE_S]) {
			player->changeVel(-(100.0*time.delta / 100.0) , player->getRot());
		}

		if(keystates[SDL_SCANCODE_A]) {
			player->setRot(player->getRot() - (0.1/time.delta));
		} else if(keystates[SDL_SCANCODE_D]) {
			player->setRot(player->getRot() + (0.1/time.delta));
		}
	}

	//Quit event
	if(e.type == SDL_QUIT) {
		this->quit = true;
	}

	// Network Events
	if(e.type == this->client.event_type) {
		GamePacket* packet = (GamePacket*)(e.user.data1);
		SDL_Log("Revieced Packet type:%d crc:%d len:%d" , packet->type, packet->checksum , packet->len);

		switch(packet->type) {
			case 0x80:
				//this->client.send((char*)packet->data, 0x80 , packet->len);
				break;
		}

		delete packet->data;
		delete packet;
	}
}
