#include <Python.h>

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
	PyRun_SimpleString("from adafruit_motorkit import MotorKit"
	"kit = MotorKit()"
	"kit.motor1.throttle = -.5"
	"kit.motor2.throttle = -.5"
	"kit.motor3.throttle = -.5"
	"kit.motor4.throttle = -.5");
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

void move(int direction, float distance)
{
	if (direction != -1)
	{
		if (direction == 0)
		{
			moveForward();
			
			//CHANGE ONCE DISTANCE CONVERSION IS DETERMINED!!!!!!!!!!!!!!
			sleep(1);
			
			stopMoving();
		}
		else if (direction == 1)
		{
			moveBackward();
			
			//CHANGE ONCE DISTANCE CONVERSION IS DETERMINED!!!!!!!!!!!!!!
			sleep(1);
			
			stopMoving();
		}
		else if (direction == 2)
		{
			turn(distance);
			
			stopMoving();
		}
		else if (direction == 3)
		{
			direction *= -1;
			turn(distance);
			
			stopMoving();
		}
		
		printf("Done Moving\n");
	}
}

void turn(float angle)
{
	
}
