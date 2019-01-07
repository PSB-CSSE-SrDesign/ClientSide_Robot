#include "commandController.h"
#include "connectionController.h"
#include "pythonSetup.h"

int main(int argc, char *argv[]) {
	setupInterpreter(argv[]);
	
	int connectionCheck;
	do
	{
		connectionCheck = connectionSetup(DEFAULT_CONNECTION);
	} while (connectionCheck < 0);
	
	
	
	shutdownInterpreter();
	return 0;
}

