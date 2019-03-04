#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 27015

int sock = 0;
char DEFAULT_CONNECTION[] = "192.168.86.21";
//192.168.86.3

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
		exit(1);
	}
}

unsigned char* serializeInt(char *buffer, int value)
{
	union
	{
		int a;
		unsigned char bytes[4];
	} b;
	b.a = value;
	
	buffer[0] = b.bytes[0];
	buffer[1] = b.bytes[1];
	buffer[2] = b.bytes[2];
	buffer[3] = b.bytes[3];
	return buffer + 4;
}

unsigned char* serializeFloat(char *buffer, float value)
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
	char *buffer = (char*)malloc(sizeof(char)*(imageSize + 9));
	buffer = serializeFloat(buffer, packet->range);
	buffer = serializeInt(buffer, imageSize);
	char *fileBuffer = (char*)malloc(sizeof(char)*(imageSize+1));
	size_t result = fread(fileBuffer, 1, imageSize, packet->image);
	int i;
	for (i = 0; i < imageSize; i++)
	{
		buffer[i] = fileBuffer[i];
	}
	
	buffer -= 8;
	
	int sendResult = write(sock, buffer, imageSize+8);
	printf("%i\n", sendResult);
	
	/*
	if (sendResult > -1)
	{
		FILE* testImage = fopen("/home/pi/Documents/image2.jpg", "w");
		size_t saved = fwrite(fileBuffer, sizeof(char), imageSize, testImage);
		printf("Wrote bytes: %i\n", saved);
	}
	*/
}

void receiveData(char *buffer)
{
	int valread = read(sock, buffer, 8);
	if (valread != 8)
	{
		printf("Command Read Error: %i : %s\n", valread, buffer);
		int i;
		for (i = 0; i < 8; i++)
		{
			buffer[i] = 0;
		}
	}
	
	return;
}
