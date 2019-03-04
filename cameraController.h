#include <Python.h>

void takePicture()
{
	PyRun_SimpleString("from picamera import PiCamera\n"
	"camera = PiCamera()\n"
	"camera.capture('/home/pi/Documents/image.jpg', 'jpeg', 1)\n"
	"camera.close()\n");
	printf("Picture Taken\n");
}
