import os
import keyboard
from sercomm import ser, ser_prepare
import numpy as np
import time

path_name = './data_new'
data_name = 'data_{}.csv' 

if not os.path.exists(path_name):
    os.mkdir(path_name)

# # Check the smallest serial number
# for i in range(1, 100):
#     if not os.path.exists(os.path.join(path_name, data_name.format(i))):
#         serial_number = i
#         break

# data_name = data_name.format(serial_number)

data_name = 'czl_backward.csv'

f = open(os.path.join(path_name, data_name), 'a')

ser_prepare()

status = None

idx = 0

try:
    while True:      
        if ser.in_waiting:          
            data = ser.readline().decode().strip()
            print(data)
            
            f.write(data + '\n')
            
        elif keyboard.is_pressed('F9'): 
            break
        
except Exception as e:
    ...
    
ser.close() 
f.close()