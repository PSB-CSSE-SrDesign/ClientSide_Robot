#include "pythonSetup.h"
#include "radarController.h"

int main(int argc, char *argv[])
{
	setupInterpreter(argv);
	startRadarThread();
	
	return 0;
}
