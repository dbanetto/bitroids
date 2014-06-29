#ifndef GAMEPACKET_H
#define GAMEPACKET_H

#include <SDL2/SDL_net.h>

struct GamePacket {
	Uint16 checksum; //Basic checksum
	Uint32 timestamp; // game time stamp
	Uint8 type;
	Uint16 len; //Length of the data capped to 4 Kilobyes
				//more than 512 becomes unrealiabe with UPD
	Uint8* data;
};

int buildpacket ( Uint8* data , Uint16 len, Uint8 type , Uint32 timestamp , UDPpacket** packet);
int unpackpacket(UDPpacket** packet , GamePacket* out);
Uint16 checksumCRC16 (Uint8* data , Uint16 len);

#endif // GAMEPACKET_H
