import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation
from paho.mqtt import client as mqtt_client

broker = '192.168.1.12'
port = 1883
topicEMG = "emgRaw"
topicBPM = "wearablebpmRaw"
client_id = 'DataGraphs'
username = 'SeizureSensor'
password = 'subpublol'

bpmData = []
emgData = []

def connect_mqtt():
    def on_connect(client, userdata, flags, rc, properties=None):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print(f"Failed to connect, return code {rc}")

    # client = mqtt_client.Client(client_id=client_id, protocol=mqtt_client.MQTTv5)
    
    client = mqtt_client.Client(client_id=client_id, callback_api_version=mqtt_client.CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect
    client.username_pw_set(username, password)
    client.connect_async(broker, port)
    return client

def on_message(client, userdata, msg):
    global bpmData, emgData
    print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
    try:
        msgNum = int(msg.payload.decode())
        if msg.topic == topicBPM:
            bpmData.append(msgNum)
            if len(bpmData) > 100:  # Keep only last 100 points
                bpmData.pop(0)
        elif msg.topic == topicEMG:
            emgData.append(msgNum)
            if len(emgData) > 100:  # Keep only last 100 points
                emgData.pop(0)
    except ValueError:
        print("Bad raw data")

client = connect_mqtt()
client.on_message = on_message
client.subscribe([(topicBPM, 2), (topicEMG, 2)])
client.loop_start()

fig, (bpm_ax, emg_ax) = plt.subplots(1, 2, figsize=(12, 6))
bpm_line, = bpm_ax.plot([], [])
emg_line, = emg_ax.plot([], [])

bpm_ax.set_title("BPM Data")
bpm_ax.set_xlim(0, 100)
bpm_ax.set_ylim(0, 200)

emg_ax.set_title("EMG Data")
emg_ax.set_xlim(0, 100)
emg_ax.set_ylim(0, 1000)

def update(frame):
    bpm_line.set_data(range(len(bpmData)), bpmData)
    emg_line.set_data(range(len(emgData)), emgData)
    fig.canvas.draw()
    return bpm_line, emg_line

ani = FuncAnimation(fig, update, frames=None, interval=100, blit=True)
plt.show()