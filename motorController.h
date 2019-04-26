#include <Python.h>
#include <string.h>
#include <stdio.h>

void moveForward()
{
	PyRun_SimpleString("from adafruit_motorkit import MotorKit\n"
	"kit = MotorKit()\n"
	"kit.motor1.throttle = .5\n"
	"kit.motor2.throttle = .5\n"
	"kit.motor3.throttle = .5\n"
	"kit.motor4.throttle = .5\n");
}

void moveBackward()
{
	PyRun_SimpleString("from adafruit_motorkit import MotorKit\n"
	"kit = MotorKit()\n"
	"kit.motor1.throttle = -.5\n"
	"kit.motor2.throttle = -.5\n"
	"kit.motor3.throttle = -.5\n"
	"kit.motor4.throttle = -.5\n");
}

void stopMoving()
{
	PyRun_SimpleString("from adafruit_motorkit import MotorKit\n"
	"kit = MotorKit()\n"
	"kit.motor1.throttle = 0\n"
	"kit.motor2.throttle = 0\n"
	"kit.motor3.throttle = 0\n"
	"kit.motor4.throttle = 0\n");
}

void turnLeft(float angle)
{
	char num[5];
	gcvt(angle, 4, num);
	
	const char *beginning = "import time\n"
	"from adafruit_motorkit import MotorKit\n"
	"factor = 180.0\n"
	"kit = MotorKit()\n"
	"kit.motor1.throttle = 1\n"
	"kit.motor2.throttle = 1\n"
	"kit.motor3.throttle = -1\n"
	"kit.motor4.throttle = -1\n"
	"time.sleep(";
	
	const char *ending = " / factor)\n"
	"kit.motor1.throttle = 0\n"
	"kit.motor2.throttle = 0\n"
	"kit.motor3.throttle = 0\n"
	"kit.motor4.throttle = 0\n";
	
	char *total;
	total = malloc(strlen(beginning)+strlen(ending)+5);
	strcpy(total, beginning);
	strcat(total, num);
	strcat(total, ending);
	
	PyRun_SimpleString(total);
}

void turnRight(float angle)
{
	char num[5];
	gcvt(angle, 4, num);
	
	const char *beginning = "import time\n"
	"from adafruit_motorkit import MotorKit\n"
	"factor = 180.0\n"
	"kit = MotorKit()\n"
	"kit.motor1.throttle = -1\n"
	"kit.motor2.throttle = -1\n"
	"kit.motor3.throttle = 1\n"
	"kit.motor4.throttle = 1\n"
	"time.sleep(";
	
	const char *ending = " / factor)\n"
	"kit.motor1.throttle = 0\n"
	"kit.motor2.throttle = 0\n"
	"kit.motor3.throttle = 0\n"
	"kit.motor4.throttle = 0\n";
	
	char *total;
	total = malloc(strlen(beginning)+strlen(ending)+5);
	strcpy(total, beginning);
	strcat(total, num);
	strcat(total, ending);
	
	PyRun_SimpleString(total);
}

void move(int direction, float distance)
{
	if (direction != -1)
	{
		if (direction == 0)
		{
			moveForward();
			
			//CHANGE ONCE DISTANCE CONVERSION IS DETERMINED!!!!!!!!!!!!!!
			usleep(100000);
			
			stopMoving();
		}
		else if (direction == 1)
		{
			moveBackward();
			
			//CHANGE ONCE DISTANCE CONVERSION IS DETERMINED!!!!!!!!!!!!!!
			usleep(100000);
			
			stopMoving();
		}
		else if (direction == 2)
		{
			turnLeft(distance);
			
			stopMoving();
		}
		else if (direction == 3)
		{
			turnRight(distance);
			
			stopMoving();
		}
		
		printf("Done Moving\n");
	}
}
