#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 27015

int sock = 0;
char DEFAULT_CONNECTION[] = "192.168.86.3";

struct Packet
{
	FILE *image;
	float range;
};

void connectionSetup(char connectionAddr[20])
{
	struct sockaddr_in address;
	struct sockaddr_in serv_addr;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sock < 0)
	{
		printf("\nSocket Creation Error\n");
		return;
	}
	
	memset(&serv_addr, '0', sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	if (inet_pton(AF_INET, connectionAddr, &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid Address/Address Not Supported\n");
		return;
	}
	
	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed\n");
		return;
	}
}

unsigned char* serializeInt(unsigned char *buffer, int value)
{
	buffer[0] = value >> 24;
	buffer[1] = value >> 16;
	buffer[2] = value >> 8;
	buffer[3] = value;
	buffer += 4;
	return &buffer;
}

unsigned char* serializeFloat(unsigned char *buffer, float value)
{
	union
	{
		float a;
		unsigned char bytes[4];
	} b;
	b.a = value;
	
	buffer[0] = b.bytes[0];
	buffer[1] = b.bytes[1];
	buffer[2] = b.bytes[2];
	buffer[3] = b.bytes[3];
	return buffer + 4;
}

void sendData(struct Packet *packet)
{
	fseek(packet->image, 0L, SEEK_END);
	int imageSize = ftell(packet->image);
	rewind(packet->image);
	unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char)*imageSize + 8);
	buffer = serializeFloat(buffer, packet->range);
	buffer = serializeInt(buffer, imageSize);
	buffer -= (imageSize + 8);
	fread(buffer, 1, imageSize, packet->image);
	
	buffer -= 8;
	write(sock, buffer, sizeof(buffer));
}

void receiveData(char *buffer)
{
	int valread = read(sock, buffer, 8);
	printf("1\n");
	if (valread != 8)
	{
		printf("\nCommand Read Error\n");
		int i;
		for (i = 0; i < 8; i++)
		{
			buffer[i] = 0;
		}
	}
	
	return;
}
