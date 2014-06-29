#include "net/GamePacket.h"

int buildpacket ( Uint8* data , Uint16 len, Uint8 type , Uint32 timestamp , UDPpacket** packet)
{
	*packet = SDLNet_AllocPacket(len + sizeof(Uint32) + sizeof(Uint16) + sizeof(Uint16) + sizeof(Uint8));

	Uint16 crc = checksumCRC16(data , len);
	Uint16 index = 0;

	(*packet)->data[index] = type;
	index++;
	//Break down each into Uint8's
	for (int i = sizeof(Uint16) - 1; i >= 0; i--)
	{
		(*packet)->data[index] = crc >> 8*i;
		index++;
	}

	for (int i = sizeof(Uint32) - 1; i >= 0; i--)
	{
		(*packet)->data[index] = timestamp >> 8*i;
		index++;
	}

	for (int i = sizeof(Uint16) - 1; i >= 0; i--)
	{
		(*packet)->data[index] = len >> 8*i;
		index++;
	}

	for (Uint16 i = 0; i < len; i++)
	{
		(*packet)->data[index + i] = data[i];
	}

	(*packet)->len = index + len;
	SDL_Log("Built: Data crc:%d type:%d time:%d len:%d packet Packet crc:%d", crc, type, timestamp, len ,
			 checksumCRC16((*packet)->data , (*packet)->len ) );
	return 0;
}

int unpackpacket(UDPpacket** packet , GamePacket* out)
{
	Uint16 index = 0;
	out->checksum = 0;
	out->len = 0;
	out->timestamp = 0;

	out->type = (*packet)->data[index];
	index++;

	for (int i = sizeof(Uint16) - 1; i >= 0; i--)
	{
		out->checksum += (*packet)->data[index] << 8*i;
		index++;
	}

	for (int i = sizeof(Uint32) - 1; i >= 0; i--)
	{
		out->timestamp += (*packet)->data[index] << 8*i;
		index++;
	}

	for (int i = sizeof(Uint16) - 1; i >= 0; i--)
	{
		out->len += (*packet)->data[index] << 8*i;
		index++;
	}

	out->data = new Uint8[out->len];
	for (Uint16 i = 0; i < out->len; i++)
	{
		out->data[i] = (*packet)->data[index + i];
	}

	Uint16 crc = checksumCRC16( out->data , out->len);
	if (crc != out->checksum)
	{
		SDL_Log("Incorrect CRC16 in packet Expected %d Got %d", out->checksum , crc );
	}
	return 0;
}

Uint16 checksumCRC16 (Uint8* data , Uint16 len)
{
	Uint16 out = 0xffff;
	Uint16 tmp = 0, tmp2 = 0;
	for (Uint16 i = 0; i < len; i++)
	{
		tmp = out ^ data[i];
		tmp2 = tmp ^ (tmp << 4);
		out = (out >> 8) ^ (tmp2 << 8) ^ (tmp2 << 3) ^ (tmp2 >> 4);
	}
	out ^= 0xffff;
	return out;
}
