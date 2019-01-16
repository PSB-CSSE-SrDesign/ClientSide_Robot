#include "pythonSetup.h"
#include "connection.h"
#include "commandController.h"

char serverAddress[] = "192.168.86.26";

int main(int argc, char *argv[])
{
	setupInterpreter(argv);
	connectionSetup(DEFAULT_CONNECTION);
	
	FILE *file = fopen("test.jpg", "r");
	
	struct Packet pack = {pack.image = file, pack.range = 10.0};
	sendData(&pack);
	
	char buffer[8];
	receiveData(&buffer);
	
	acceptCommand(buffer);
	
	/*
	printf("Beginning Move\n");
	moveForward();
	usleep(250000);
	
	printf("Stopping Move\n");
	stopMoving();
	*/
	
	shutdownInterpreter();
	return 0;
}
