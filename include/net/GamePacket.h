#ifndef GAMEPACKET_H
#define GAMEPACKET_H

#include <SDL2/SDL_net.h>

struct GamePacket {
	Uint16 checksum; //Basic checksum
	long unsigned int timestamp; // game time stamp
	Uint8 type;
	unsigned int len; //Length of the data
	Uint8* data;
};

int GamePacketToUDPpacket ( const GamePacket gpacket, UDPpacket* packet );
int UDPpacketToGamePacket ( const UDPpacket packet, GamePacket* gpacket );

int buildpacket ( Uint8* data , unsigned int len , UDPpacket** packet);

Uint16 checksumCRC16 (Uint8* data , unsigned int len);

#endif // GAMEPACKET_H
