import os
import serial
import struct
from picamera import PiCamera
import time
from blinkstick import blinkstick
import logging
import socket
import errno

logging.basicConfig(filename='radarLog.txt', level=logging.DEBUG)
# Ops241A module settings:  ftps, dir off, 5Ksps, min -9dB pwr, squelch 5000
Ops241A_Speed_Output_Units = 'UM'
Ops241A_Direction_Control = 'Od'
Ops241A_Sampling_Frequency = 'SV'
Ops241A_Transmit_Power = 'PD'    # miD power
Ops241A_Threshold_Control = 'QX' # 1000 magnitude-square.  10 as reported
Ops241A_Module_Information = '??'

# Initialize the USB port to read from the OPS-241A module
ser=serial.Serial(
    port = '/dev/ttyACM0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1,
    writeTimeout = 2
)
ser.flushInput()
ser.flushOutput()

# sendSerialCommand: function for sending commands to the OPS-241A module
def send_serial_cmd(print_prefix, command) :
    data_for_send_str = command
    data_for_send_bytes = str.encode(data_for_send_str)
    logging.debug(print_prefix+command)
    ser.write(data_for_send_bytes)
    # Initialize message verify checking
    ser_message_start = '{'
    ser_write_verify = False
    # Print out module response to command string
    while not ser_write_verify :
        data_rx_bytes = ser.readline()
        data_rx_length = len(data_rx_bytes)
        if (data_rx_length != 0) :
            data_rx_str = str(data_rx_bytes)
            if data_rx_str.find(ser_message_start) :
                ser_write_verify = True
            
# Initialize and query Ops241A Module
logging.debug('\nInitializing Ops241A Module')
send_serial_cmd('\nSet Speed Output Units: ', Ops241A_Speed_Output_Units)
send_serial_cmd('\nSet Direction Control: ', Ops241A_Direction_Control)
send_serial_cmd('\nSet Sampling Frequency: ', Ops241A_Sampling_Frequency)
send_serial_cmd('\nSet Transmit Power: ', Ops241A_Transmit_Power)
send_serial_cmd('\nSet Threshold Control: ', Ops241A_Threshold_Control)
send_serial_cmd('\nModule Information: ', Ops241A_Module_Information)

ser=serial.Serial(
    port = '/dev/ttyACM0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 0.01,
    writeTimeout = 2
    )

# Main Loop
logging.debug('\nStarting Loop\n')
done = False
radarPipe = os.open('/home/pi/Documents/radarPipe.fifo', os.O_WRONLY)
count = 0
lastTime = 0
bstick = blinkstick.find_first()
lastFloat = 0.0
print(bstick.get_serial())
while not done:
    speed_available = False
    Ops241_rx_bytes = ser.readline()
    # Check for speed information from OPS241-A
    Ops241_rx_bytes_length = len(Ops241_rx_bytes)
    if (Ops241_rx_bytes_length != 0) :
        Ops241_rx_str = str(Ops241_rx_bytes)
        if Ops241_rx_str.find('{') == -1 :
            # Speed data found
            Ops241_float = float(Ops241_rx_str)
            logging.debug(str(Ops241_float))
            if (Ops241_float > 0):
                bstick.set_color(0, 0, 255, 255, 0)
            else:
                bstick.set_color(0, 0, 255, 0, 255)
            if (Ops241_float > 1):
                Ops241_float = 1.0
            elif (Ops241_float < -1):
                Ops241_float = -1.0
            difference = 0.0
            if (Ops241_float-lastFloat > 1):
                difference = 1.0
            elif (Ops241_float-lastFloat < -1):
                difference = -1.0
            else:
                difference = Ops241_float-lastFloat
            bstick.set_color(0, 2, 100, abs(int(255*Ops241_float)), 100)
            bstick.set_color(0, 5, 100, abs(int(255*(difference))), 0)
            lastFloat = Ops241_float
            currentTime = time.time()
            if ((currentTime - lastTime) > .25) :
                try:
                    lastTime = currentTime
                    camera = PiCamera()
                    camera.awb_mode = 'fluorescent'
                    camera.capture('/home/pi/Documents/Images/image_'+str(count)+'.jpg', 'jpeg', 1)
                    camera.close()
                    logging.debug(str(count))
                    count = count + 1
                    logging.debug(str(Ops241_rx_bytes))
                    os.write(radarPipe, Ops241_rx_bytes)
                    logging.debug(str(currentTime))
                except Exception:
                    print('Broken Pipe Exception')
                    #os.remove('/home/pi/Documents/Images/image_'+str(count-1)+'.jpg')
                    #exit()
                    os.system('shutdown -h -t 0')