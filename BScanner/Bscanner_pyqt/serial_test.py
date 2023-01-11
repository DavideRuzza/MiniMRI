import serial
import time
from serial.tools import list_ports
from threading import Thread
import struct



# for com in list_ports.comports():
#     print(com.pid, com.vid, com.device)
ser = serial.Serial('COM12', 115200, timeout=5)
ser.reset_input_buffer()
ser.reset_output_buffer()
time.sleep(3)

# print("send")
# ser.write(b'\x02\x00\x00\x00\x00\n')
# while(ser.inWaiting()<1):
#     pass

# print(ser.read(1))




ser.write(b'\x01'+struct.pack(">f", -10)+b'\n')

while(ser.inWaiting()<1):
    pass
print(ser.read(1))
# print(ser.readline().decode())

# while(ser.inWaiting()<1):
#     pass

# print(ser.read(1))


# ser.write(b'\x07\x00\x00\x00\x00\n')
# while(ser.inWaiting()<1):
#     pass
# res = ser.read(1)

# if res == "y":
#     while(ser.inWaiting()<4):
#         pass

#     print(struct.unpack(">f", ser.read(4)))
#     time.sleep(0.5)
#     print(ser.readline().decode())

ser.close()


