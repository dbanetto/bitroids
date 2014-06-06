#include "GamePacket.h"


int GamePacketToUDPpacket ( const GamePacket gpacket, UDPpacket* packet )
{
	return 0;
}

int UDPpacketToGamePacket ( const UDPpacket packet, GamePacket* gpacket )
{
	return 0;
}


int buildpacket ( Uint8* data , unsigned int len , UDPpacket** packet)
{
	*packet = SDLNet_AllocPacket(len + 11);
	int index = 0;
	long unsigned int timestamp = 0xff;
	Uint16 crc = checksumCRC16(data , len);
	Uint8 type = 0x1;

	(*packet)->data[index] = crc;
	index += 2;

	(*packet)->data[index] = timestamp;
	index += 4;

	(*packet)->data[index] = type;
	index += 1;

	(*packet)->data[index] = len;
	index += 4;

	for (unsigned int i = 0; i < len; i++)
	{
		(*packet)->data[index + i] = data[i];
	}

	(*packet)->len = index + len;
	SDL_Log("Built crc:%d type:%d time:%d len:%d packet crc:%d", crc, type, timestamp, (*packet)->len ,
			 checksumCRC16((*packet)->data , (*packet)->len ) );
	return 0;
}

Uint16 checksumCRC16 (Uint8* data , unsigned int len)
{
	Uint16 out = 0xffff;
	Uint16 tmp = 0, tmp2 = 0;
	for (unsigned int i = 0; i > len; i++)
	{
		tmp = out ^ data[i];
		tmp2 = tmp ^ (tmp << 4);
		out = (out >> 8) ^ (tmp2 << 8) ^ (tmp2 << 3) ^ (tmp2 >> 4);
	}
	out ^= 0xffff;
	return out;
}
