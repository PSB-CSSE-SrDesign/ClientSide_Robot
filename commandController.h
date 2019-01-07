#include "motorController.h"

/*
	Commands are eight (8) characters long. The first four (4) correspond to the direction and the last four (4)
	correspond to a four digit number with two decimal precision.
*/

void acceptCommand(char[] command)
{
	int direction = -1;
	
	//Parsing Direction Command
	if (command[0] == 'F' && command[1] == 'O' && command[2] == 'R' && command[3] == 'W')
	{
		direction = 0;
	}
	else if (command[0] == 'B' && command[1] == 'A' && command[2] == 'C' && command[3] == 'K')
	{
		direction = 1;
	}
	else if (command[0] == 'L' && command[1] == 'E' && command[2] == 'F' && command[3] == 'T')
	{
		direction = 2;
	}
	else if (command[0] == 'R' && command[1] == 'I' && command[2] == 'G' && command[3] == 'T')
	{
		direction = 3;
	}
	
	//Parsing Distance/Angle
	float distance = 0;
	distance += (command[4] - 48) * 10.0;
	distance += (command[5] - 48) * 1.0;
	distance += (command[6] - 48) / 10.0;
	distance += (command[7] - 48) / 100.0;
	
	move(direction, distance);
}