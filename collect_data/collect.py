import os
import keyboard
from sercomm import ser, ser_prepare
import numpy as np
import time

path_name = './data'
data_name = 'data_{}.csv' 

if not os.path.exists(path_name):
    os.mkdir(path_name)

# Check the smallest serial number
for i in range(1, 100):
    if not os.path.exists(os.path.join(path_name, data_name.format(i))):
        serial_number = i
        break

data_name = data_name.format(serial_number)

data_name = 'yan_standing.csv'

f = open(os.path.join(path_name, data_name), 'a')

ser_prepare()

status = None

idx = 0

try:
    while True:      
        # 再看要用程式寫 分檔案 還是寫死
        # status = 1 if keyboard.is_pressed('q') else 0
        
        if ser.in_waiting:          
            data = ser.readline().decode().strip() # + f', {scratching:d}'
            # data += f', {status}'
            print(data)
            
            f.write(data + '\n')
            # if idx == 0:
            
            # idx += 1
            # if idx == 10:
            #     idx = 0
            
        elif keyboard.is_pressed('F9'): 
            break
        
except Exception as e:
    ...
    
ser.close() 
f.close()