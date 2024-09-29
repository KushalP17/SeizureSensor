#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const int emgPin = 33;

const char* ssid = "G34";
const char* password = "designthinking";

const char *mqtt_broker = "192.168.1.12";
const char *rawtopic = "emgRaw";
const char *statustopic = "emgStatus";
const char *mqtt_username = "SeizureSensor";
const char *mqtt_password = "subpublol";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int upBound = 1900;
const int lowBound = 1600;
const int sampleTime = 50;

volatile int emgValue = 0;
bool spike = false;

void setup() {
  Serial.begin(115200);
  // while (!Serial); // optionally wait for serial terminal to open
  Serial.println("EMG On");

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  
}

void loop() {
  
   

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int emgValue = analogRead(33); 

  Serial.println(emgValue);
    
  delay(sampleTime); 
    
  char tempString[8];
  dtostrf(emgValue, 1, 0, tempString);
  client.publish(rawtopic, tempString);

  if(emgValue > upBound || emgValue < lowBound){
    spike = true;
    client.publish(statustopic, "true");
  }
  else{
    spike = false;
    client.publish(statustopic, "false");
  }
  

  

  delay(sampleTime); 
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    String client_id = "esp32-client";
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Mosquitto MQTT broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length){
  
}
