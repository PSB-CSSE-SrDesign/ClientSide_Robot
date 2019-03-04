#include "pythonSetup.h"
#include "connection.h"
#include "commandController.h"
#include "cameraController.h"
#include "radarController.h"

char serverAddress[] = "192.168.86.26";

int main(int argc, char *argv[])
{
	setupInterpreter(argv);
	connectionSetup(DEFAULT_CONNECTION);
	startRadar();
	
	int i = 1;
	
	while (getRun() == 1)
	{
		printf("\n%i:\n", i);
		float rangeRate = readRadarData();
		printf("%f\n", rangeRate);
		takePicture();
		
		FILE *file = fopen("/home/pi/Documents/image.jpg", "rb");
		
		struct Packet pack = {pack.image = file, pack.range = rangeRate};
		sendData(&pack);
		
		fclose(file);
		
		char buffer[9];
		buffer[8] = '\0';
		receiveData(&buffer);
		
		if (i == 20)
		{
			acceptCommand("SHUTDOWN");
		}
		else
		{
			i++;
			acceptCommand(buffer);
		}
	}
	
	shutdownInterpreter();
	radarCleanUp();
	return 0;
}
