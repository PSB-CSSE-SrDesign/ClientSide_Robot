#include "pythonSetup.h"
#include "radarController.h"

int main(int argc, char *argv[])
{
	//freopen("/home/pi/Documents/Logs/radarLog.txt", "w", stdout);
	
	setupInterpreter(argv);
	startRadarThread();
	
	//freopen("/dev/stdout", "w", stdout);
	
	return 0;
}
