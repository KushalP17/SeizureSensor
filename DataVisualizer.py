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

def connect_mqtt():
    # def on_connect(client, userdata, flags, rc):
    # For paho-mqtt 2.0.0, you need to add the properties parameter.
    def on_connect(client, userdata, flags, rc, properties):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)
    # Set Connecting Client ID
    # client = mqtt_client.Client(client_id)

    # For paho-mqtt 2.0.0, you need to set callback_api_version.
    client = mqtt_client.Client(client_id=client_id, callback_api_version=mqtt_client.CallbackAPIVersion.VERSION2)

    client.on_connect = on_connect
    client.username_pw_set(username, password)
    client.connect_async(broker, port)
    return client


def subscribe(client: mqtt_client, topic):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}`")
        try:
            msgNum = int(msg.payload.decode())
            if msgNum < 150:
                bpmData.append(msgNum)
            elif msgNum > 200:
                emgData.append(msgNum)
        except:
            print("bad raw data")


    client.subscribe(topic)
    client.on_message = on_message



client = connect_mqtt()
topic = [(topicBPM, 2), (topicEMG, 2)]
subscribe(client, topic=topic)
client.loop_start()
# client.loop_forever()

bpmData = [0]
emgData = [0]

fig, (bpm, emg) = plt.subplots(1, 2)
# bpm = fig.add_subplot(121)
# emg = fig.add_subplot(122)
graphBPM = bpm.plot(range(len(bpmData)), bpmData)[0]
graphEMG = emg.plot(range(len(emgData)), emgData)[0]

def update(frame):
    global graphBPM
    global graphEMG

    # bpmData.append(newData)
    graphBPM.set_xdata(range(len(bpmData)))
    # graphBPM.append
    graphBPM.set_ydata(bpmData)
    # plt.xlim(0, len(bpmData))
    # emgData.append(newData)
    graphEMG.set_xdata(range(len(emgData)))
    graphEMG.set_ydata(emgData)
    # plt.xlim(0, len(emgData))


# anim = FuncAnimation(fig, update, frames=None)
client.loop_forever()
# plt.show()  



