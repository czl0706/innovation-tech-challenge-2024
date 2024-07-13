import paho.mqtt.client as mqtt
import sqlite3
import time
from collections import defaultdict
import numpy as np
from calculate_data import *

# 數據庫設置
DB_NAME = "mqtt_data.db"

# MQTT設置
broker_address = "192.168.137.1"
broker_port = 1883
topics = [f"raw_ch{i}_part{j}" for i in range(1, 2 + 1) for j in range(1, 10 + 1)]
topics += [f"pred_{i}" for i in range(1, 2 + 1)]
# topics = [f"raw_ch{i}_part{j}" for i in range(1, 6 + 1) for j in range(1, 2 + 1)]

# 用於跟踪每個 topic 的最新消息
latest_messages = defaultdict(str)

def init_db():
    conn = sqlite3.connect(DB_NAME)
    c = conn.cursor()
    c.execute('''CREATE TABLE IF NOT EXISTS batch_data
                 (batch_id INTEGER PRIMARY KEY AUTOINCREMENT,
                  timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
                  raw_data TEXT,
                  pred_data TEXT,
                  lng REAL,
                  ea REAL,
                  rms REAL)''')
    conn.commit()
    conn.close()

def insert_batch_data(raw_data, pred_data, lng, ea, rms):
    conn = sqlite3.connect(DB_NAME)
    c = conn.cursor()
    c.execute("""
        INSERT INTO batch_data (raw_data, pred_data, lng, ea, rms)
        VALUES (?, ?, ?, ?, ?)
    """, (str(raw_data.tolist()), str(pred_data.tolist()), lng, ea, str(rms)))
    conn.commit()
    conn.close()

# def process_batch():
#     print("Processing batch:")
#     for topic in topics:
#         print(f"{topic}")
    
#     # 處理原始數據
#     raw_data = np.zeros((600, 6))
#     for i in range(1, 7):
#         channel_data = latest_messages[f'raw_ch{i}_part1'] + ',' + latest_messages[f'raw_ch{i}_part2']
#         raw_data[:, i-1] = np.array(list(map(float, channel_data.split(',')))).astype(float)
    
#     # 應用濾波器
#     for col in range(6):
#         raw_data[:, col] = butter_lowpass_filter(raw_data[:, col], cutoff=4, fs=20, order=8)
    
#     # 處理預測數據
#     pred = np.zeros((18, 2))
#     pred[:, 0] = np.array(list(map(int, latest_messages['pred_1'].split(',')))).astype(int)
#     pred[:, 1] = np.array(list(map(int, latest_messages['pred_2'].split(',')))).astype(int)
    
#     print(pred)
    
#     # 計算指標
#     lng = calculate_lng(raw_data)
#     ea = calculate_ea(raw_data)
#     rms = calculate_rms(raw_data)
    
#     print(f"Total Sway Length (LNG): {lng} (m)")
#     print(f"Environmental Area (EA): {ea} (m^2)")
#     print(f"Root Mean Square (RMS): {rms}")
    
#     # 將數據存儲到數據庫
#     insert_batch_data(raw_data, pred, lng, ea, rms)
    
#     print("Batch processing complete and data stored in database.")

def process_batch():
    print("Processing batch:")
    for topic in topics:
        print(f"{topic}")
    
    # 處理原始數據
    raw_data = np.zeros((3000, 2))
    for i in range(1, 2+1):
        channel_data = ','.join(latest_messages[f'raw_ch{i}_part{j}'] for j in range(1, 10+1))
        channel_data = list(map(float, channel_data.split(',')))
        print(len(channel_data))
        raw_data[:, i-1] = np.array(channel_data).astype(float)

    raw_data = raw_data * 9.81
    
    for col in range(2):
        raw_data[:, col] = butter_lowpass_filter(raw_data[:, col], cutoff=2, fs=20, order=4)
        
    lng = calculate_lng(raw_data, 0.01)
    ea = calculate_ea(raw_data)
    rms = calculate_rms(raw_data)
    
    print(f"Total Sway Length (LNG) by lng: {lng} (m)")
    print(f"Environmental Area (EA): {ea} (m^2)")
    print(f"Root Mean Square (RMS): {rms}")
    
    # 處理預測數據
    pred = np.zeros((24, 2))
    pred[:, 0] = np.array(list(map(int, latest_messages['pred_1'].split(',')))).astype(int)
    pred[:, 1] = np.array(list(map(int, latest_messages['pred_2'].split(',')))).astype(int)
    
    # 將數據存儲到數據庫
    insert_batch_data(raw_data, pred, lng, ea, rms)
    
    print("Batch processing complete and data stored in database.")

def on_message(client, userdata, message):
    topic = message.topic
    payload = message.payload.decode()
    latest_messages[topic] = payload
    print(f"Received message on topic {topic}")
    
    # 檢查是否所有 topic 都有數據
    if all(topic in latest_messages for topic in topics):
        process_batch()
        # 清空 latest_messages 以準備下一批數據
        latest_messages.clear()

# 設置MQTT客戶端
client = mqtt.Client()
client.on_message = on_message
client.connect(broker_address, port=broker_port)

for topic in topics:
    client.subscribe(topic)
    
# 初始化數據庫
init_db()

# 開始MQTT循環
print("Starting MQTT client...")
client.loop_forever()