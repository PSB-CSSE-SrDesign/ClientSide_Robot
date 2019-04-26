#ifndef RADARCONTROLLER
#define RADARCONTROLLER
#include <Python.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

FILE *radarPipe;
const char* pipeName = "/home/pi/Documents/radarPipe.fifo";
int readData = 0;

void startRadarThread()
{
	int status = system("python /home/pi/Documents/radar.py");
	status = status / 256;
	printf("Status: %i\n", status);
	
	/*
	PyRun_SimpleString("import os\n"
"import serial\n"
"import struct\n"
"from picamera import PiCamera\n"
"import time\n"
"Ops241A_Speed_Output_Units = 'UM'\n"
"Ops241A_Direction_Control = 'Od'\n"
"Ops241A_Sampling_Frequency = 'SV'\n"
"Ops241A_Transmit_Power = 'PD'    # miD power\n"
"Ops241A_Threshold_Control = 'QX' # 1000 magnitude-square.  10 as reported\n"
"Ops241A_Module_Information = '??'\n"
"ser=serial.Serial(\n"
"	port = '/dev/ttyACM0',\n"
"	baudrate = 9600,\n"
"	parity = serial.PARITY_NONE,\n"
"	stopbits = serial.STOPBITS_ONE,\n"
"	bytesize = serial.EIGHTBITS,\n"
"	timeout = 1,\n"
"	writeTimeout = 2\n"
")\n"
"ser.flushInput()\n"
"ser.flushOutput()\n"
"def send_serial_cmd(print_prefix, command) :\n"
"	data_for_send_str = command\n"
"	data_for_send_bytes = str.encode(data_for_send_str)\n"
"	print(print_prefix, command)\n"
"	ser.write(data_for_send_bytes)\n"
"	ser_message_start = '{'\n"
"	ser_write_verify = False\n"
"	while not ser_write_verify :\n"
"		data_rx_bytes = ser.readline()\n"
"		data_rx_length = len(data_rx_bytes)\n"
"		if (data_rx_length != 0) :\n"
"			data_rx_str = str(data_rx_bytes)\n"
"			if data_rx_str.find(ser_message_start) :\n"
"				ser_write_verify = True\n"
"send_serial_cmd(\"Set Speed Output Units: \", Ops241A_Speed_Output_Units)\n"
"send_serial_cmd(\"Set Direction Control: \", Ops241A_Direction_Control)\n"
"send_serial_cmd(\"Set Sampling Frequency: \", Ops241A_Sampling_Frequency)\n"
"send_serial_cmd(\"Set Transmit Power: \", Ops241A_Transmit_Power)\n"
"send_serial_cmd(\"Set Threshold Control: \", Ops241A_Threshold_Control)\n"
"send_serial_cmd(\"Module Information: \", Ops241A_Module_Information)\n"
"ser=serial.Serial(\n"
"	port = '/dev/ttyACM0',\n"
"	baudrate = 9600,\n"
"	parity = serial.PARITY_NONE,\n"
"	stopbits = serial.STOPBITS_ONE,\n"
"	bytesize = serial.EIGHTBITS,\n"
"	timeout = 0.01,\n"
"	writeTimeout = 2\n"
"	)\n"
"done = False\n"
"radarPipe = os.open('/home/pi/Documents/radarPipe.fifo', os.O_WRONLY)\n"
"count = 0\n"
"lastTime = 0\n"
"while not done:\n"
"	speed_available = False\n"
"	Ops241_rx_bytes = ser.readline()\n"
"	Ops241_rx_bytes_length = len(Ops241_rx_bytes)\n"
"	if (Ops241_rx_bytes_length != 0) :\n"
"		Ops241_rx_str = str(Ops241_rx_bytes)\n"
"		if Ops241_rx_str.find('{') == -1 :\n"
"			currentTime = time.time()\n"
"			if ((currentTime - lastTime) > .25) :\n"
"				lastTime = currentTime\n"
"				camera = PiCamera()\n"
"				camera.awb_mode = 'fluorescent'\n"
"				camera.capture('/home/pi/Documents/Images/image_'+str(count)+'.jpg', 'jpeg', 1)\n"
"				camera.close()\n"
"				print(count)\n"
"				print(currentTime)\n"
"				count = count + 1\n"
"				os.write(radarPipe, Ops241_rx_bytes)\n");
*/
}

void startRadar()
{
	mkfifo(pipeName, 0777);
	radarPipe = open(pipeName, O_RDONLY | O_NONBLOCK);
	printf("Pipe Made\n");
}

float readRadarData()
{
	char range[8];
	float rangeRate;
	do
	{
		read(radarPipe, range, 8);
		rangeRate = (float)atof(range);
		
		if (rangeRate != 0 && readData == 0)
		{
			readData = 1;
		}
		
	} while (!readData);
	
	return rangeRate;
}

void radarCleanUp()
{
	int status = remove(pipeName);
	if (status == 0)
	{
		printf("Pipe deleted successfully\n");
	}
	else
	{
		printf("Failed to delete pipe\n");
	}
}
#endif
