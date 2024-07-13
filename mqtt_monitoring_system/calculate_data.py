import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import scipy
from math import pi
from scipy.signal import butter, filtfilt, freqz
from scipy.spatial import ConvexHull 
from filterpy.kalman import KalmanFilter
from scipy.fft import fft, fftfreq
from scipy import signal

def butter_lowpass_filter(data, cutoff, fs, order):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = filtfilt(b, a, data)
    return y

def calculate_lng(raw_data, dt):
    if len(raw_data) > 720:
        raw_data = raw_data[360:-360]
    else:
        print("Error: Data length is less than 720 samples.")
        return None
    
    # highpass filter
    b, a = scipy.signal.butter(4, 0.008, btype='highpass', analog=False)
    acc_y_filtered = scipy.signal.filtfilt(b, a, raw_data[:, 1])
    
    # integrate acceleration to get velocity
    velocity = scipy.integrate.cumtrapz(acc_y_filtered, dx=dt, initial=0)

    # apply zero velocity constraint
    zero_velocity = detect_zero_velocity(acc_y_filtered)
    velocity = apply_zero_velocity_constraint(velocity, zero_velocity)
    
    # integrate velocity to get displacement
    displacement = scipy.integrate.cumtrapz(np.abs(velocity), dx=dt, initial=0)

    # 這兩個是統計上的去雜訊，啊我覺得弄了後訊號有點奇怪，所以就不套了
    #displacement = smooth_data(displacement, window_size=5)

    # plt.plot(displacement)
    # plt.title("Displacement")
    # plt.xlabel("Time")
    # plt.ylabel("Displacement")
    # plt.show()

    return displacement[-1]

def detect_zero_velocity(acc_data, threshold=0.1):
    zero_velocity = np.abs(acc_data) < threshold
    return zero_velocity

def apply_zero_velocity_constraint(velocity, zero_velocity):
    velocity[zero_velocity] = 0
    return velocity

# def calculate_ea(raw_data):
#     points = raw_data[:, 0:2]  
#     hull = ConvexHull(points)
#     return hull.area 

def calculate_rms(raw_data):
    return np.sqrt(np.mean(raw_data[:, 0]**2 + raw_data[:, 1]**2))

def smooth_data(data, window_size=5):
    return np.convolve(data, np.ones(window_size)/window_size, mode='same')

def format_data_to_2f(df):
    formatted_df = df.applymap(lambda x: f"{x:.2f}" if isinstance(x, (int, float)) else x)
    return formatted_df

def calculate_ea(raw_data, dt=0.01):
    # points = raw_data[:, :2]  
    # try:
    #     hull = ConvexHull(points)
    #     return hull.area
    # except:
    #     return 0
    
    # highpass filter
    b, a = scipy.signal.butter(4, 0.008, btype='highpass', analog=False)
    acc_x_filtered = scipy.signal.filtfilt(b, a, raw_data[:, 0])
    
    # integrate acceleration to get velocity
    velocity = scipy.integrate.cumtrapz(acc_x_filtered, dx=dt, initial=0)

    # apply zero velocity constraint
    zero_velocity = detect_zero_velocity(acc_x_filtered)
    velocity = apply_zero_velocity_constraint(velocity, zero_velocity)
    
    # integrate velocity to get displacement
    displacement_x = scipy.integrate.cumtrapz(np.abs(velocity), dx=dt, initial=0)
    
    
    # highpass filter
    b, a = scipy.signal.butter(4, 0.008, btype='highpass', analog=False)
    acc_y_filtered = scipy.signal.filtfilt(b, a, raw_data[:, 1])
    
    # integrate acceleration to get velocity
    velocity = scipy.integrate.cumtrapz(acc_y_filtered, dx=dt, initial=0)

    # apply zero velocity constraint
    zero_velocity = detect_zero_velocity(acc_y_filtered)
    velocity = apply_zero_velocity_constraint(velocity, zero_velocity)
    
    # integrate velocity to get displacement
    displacement_y = scipy.integrate.cumtrapz(np.abs(velocity), dx=dt, initial=0)
    
    min_x, max_x = np.min(displacement_x), np.max(displacement_x)
    min_y, max_y = np.min(displacement_y), np.max(displacement_y)
    
    return (max_x - min_x) * (max_y - min_y)