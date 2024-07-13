import subprocess
import sys
import time
import os

def run_script(script_name):
    return subprocess.Popen([sys.executable, script_name])

def run_command(command):
    return subprocess.Popen(command, shell=True)

def main():
    # 啟動 Mosquitto MQTT 伺服器
    print("Starting Mosquitto MQTT server...")
    mqtt_server = run_command("mosquitto -v -c ./mosquitto.conf")
    
    print("Waiting for MQTT server to initialize (5 seconds)...")
    time.sleep(5)  # 給MQTT伺服器一些時間來初始化
    
    print("Starting MQTT client...")
    mqtt_process = run_script("mqtt_client.py")
    
    print("Waiting for MQTT client to initialize (5 seconds)...")
    time.sleep(5)  # 給MQTT客戶端一些時間來初始化
    
    print("Starting Streamlit app...")
    streamlit_process = run_script("st_run.py")  # 使用 run.py 來啟動 Streamlit
    
    print("All applications are now running.")
    print("Press Ctrl+C to stop all applications.")
    
    try:
        # 保持主程序運行
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\nStopping applications...")
        mqtt_server.terminate()
        mqtt_process.terminate()
        streamlit_process.terminate()
        print("Applications stopped.")

if __name__ == "__main__":
    main()
    
    
    
    
    
    
    
    
    
    
# import subprocess
# import sys
# import time
# import os
# import threading

# def run_script(script_name, prefix):
#     process = subprocess.Popen([sys.executable, script_name], 
#                                stdout=subprocess.PIPE, 
#                                stderr=subprocess.PIPE, 
#                                text=True, 
#                                bufsize=1, 
#                                universal_newlines=True)
    
#     def print_output(stream, prefix):
#         for line in stream:
#             print(f"{prefix} {line.strip()}")
    
#     threading.Thread(target=print_output, args=(process.stdout, prefix), daemon=True).start()
#     threading.Thread(target=print_output, args=(process.stderr, prefix), daemon=True).start()
    
#     return process

# def run_command(command, prefix):
#     process = subprocess.Popen(command, 
#                                shell=True, 
#                                stdout=subprocess.PIPE, 
#                                stderr=subprocess.PIPE, 
#                                text=True, 
#                                bufsize=1, 
#                                universal_newlines=True)
    
#     def print_output(stream, prefix):
#         for line in stream:
#             print(f"{prefix} {line.strip()}")
    
#     threading.Thread(target=print_output, args=(process.stdout, prefix), daemon=True).start()
#     threading.Thread(target=print_output, args=(process.stderr, prefix), daemon=True).start()
    
#     return process

# def main():
#     # 啟動 Mosquitto MQTT 伺服器
#     print("Starting Mosquitto MQTT server...")
#     mqtt_server = run_command("mosquitto -v -c ./mosquitto.conf", "[MQTT Server]")
    
#     print("Waiting for MQTT server to initialize (5 seconds)...")
#     time.sleep(5)  # 給MQTT伺服器一些時間來初始化
    
#     print("Starting MQTT client...")
#     mqtt_process = run_script("mqtt_client.py", "[MQTT Client]")
    
#     print("Waiting for MQTT client to initialize (5 seconds)...")
#     time.sleep(5)  # 給MQTT客戶端一些時間來初始化
    
#     print("Starting Streamlit app...")
#     streamlit_process = run_script("st_run.py", "[Streamlit]")
    
#     print("All applications are now running.")
#     print("Press Ctrl+C to stop all applications.")
    
#     try:
#         # 保持主程序運行
#         while True:
#             time.sleep(1)
#     except KeyboardInterrupt:
#         print("\nStopping applications...")
#         mqtt_server.terminate()
#         mqtt_process.terminate()
#         streamlit_process.terminate()
#         print("Applications stopped.")

# if __name__ == "__main__":
#     main()