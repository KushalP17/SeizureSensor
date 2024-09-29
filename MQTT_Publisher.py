import paho.mqtt.client as mqtt

broker_address = "localhost"
broker_port = 1883
username = "SeizureSensor"
password = "subpublol"
topic = "motionStatus"

client = mqtt.Client()

client.username_pw_set(username, password)

try:
    client.connect(broker_address, broker_port)
    print("Connected to broker")
except Exception as e:
    print(f"Failed to connect to broker: {e}")
    
def publish(msg):
    client.publish(topic, msg)
    print(f"Published '{msg}' to topic '{topic}'")
