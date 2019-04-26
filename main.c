#include "pythonSetup.h"
#include "connection.h"
#include "commandController.h"
#include "cameraController.h"
#include "radarController.h"
#include <stdio.h>
#include <time.h>

void sendThread();
void receiveThread();
void fileCleanUp();

char serverAddress[] = "192.168.86.26";
int pictures = 0;

int main(int argc, char *argv[])
{
	freopen("/home/pi/Documents/Logs/mainLog.txt", "w", stdout);

	//printf("%i\n", time(NULL));
	
	setupInterpreter(argv);
	connectionSetup();
	startRadar();
	
	pthread_t send;
	pthread_t receive;
	printf("1\n");
	pthread_create(&send, NULL, sendThread, NULL);
	printf("2\n");
	pthread_create(&receive, NULL, receiveThread, NULL);
	pthread_join(receive, NULL);
	
	int status = pthread_cancel(send);
	if (status == 0)
	{
		printf("Send thread canceled successfully\n");
	}
	else
	{
		printf("Failed to cancel send thread\n");
	}
	
	shutdownInterpreter();
	radarCleanUp();
	fileCleanUp();
	freopen("/dev/stdout", "w", stdout);
	return 0;
}

void sendThread()
{
	char* fileNameBegin = "/home/pi/Documents/Images/image_";
	char* fileNameExten = ".jpg";
	
	while(1)
	{
		float rangeRate;
		do
		{
			rangeRate = readRadarData();
		} while (rangeRate == 0);
		char* fileName;
		
		int num = 1;
		int tempPic = pictures;
		while (tempPic > 0)
		{
			tempPic /= 10;
			num += 1;
		}
		
		char numPictures[5];
		sprintf(numPictures, "%i", pictures);
		fileName = malloc(strlen(fileNameBegin)+strlen(fileNameExten)+num);
		strcpy(fileName, fileNameBegin);
		strcat(fileName, numPictures);
		strcat(fileName, fileNameExten);
		printf("%s\n", fileName);
		FILE *file = NULL;
		int i = 0;
		while (access(fileName, F_OK) == -1)
		{
			i += 1;
		}
		file = fopen(fileName, "rb");
		struct Packet pack = {pack.image = file, pack.range = rangeRate};
		sendData(&pack);
		int close = fclose(file);
		if (close != 0)
		{
			printf("Error closing file\n");
		}
		remove(fileName);
		pictures += 1;
	}
}

void receiveThread()
{
	int command = 0;
	while (getRun() == 1)
	{
		char buffer[9];
		buffer[8] = '\0';
		receiveData(&buffer);
		printf("Command %i\n", command);
		printf("\n%s\n", buffer);
		command = command + 1;
		acceptCommand(buffer);
	}
	
	printf("Received SHUTDOWN 2: Electric Boogaloo\n");
	write(sock, "CONFIRM!", 8);
}

void fileCleanUp()
{
	int status = 0;
	
	do
	{
		char* fileNameBegin = "/home/pi/Documents/Images/image_";
		char* fileNameExten = ".jpg";
		char* imageName;
		
		int num = 0;
		int tempPic = pictures;
		while (tempPic > 0)
		{
			tempPic /= 10;
			num += 1;
		}
		
		char numPictures[4];
		sprintf(numPictures, "%i", pictures);
		
		imageName = malloc(strlen(fileNameBegin)+strlen(fileNameExten)+num);
		strcpy(imageName, fileNameBegin);
		strcat(imageName, numPictures);
		strcat(imageName, fileNameExten);
		
		status = remove(imageName);
	} while (status == 0);
	
	printf("File cleanup successful\n");
}
