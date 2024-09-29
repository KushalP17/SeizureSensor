#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// WiFi and MQTT Configuration
const char* ssid = "G34";
const char* password = "designthinking";
const char *mqtt_broker = "192.168.1.12";
const char *emgtopic = "emgStatus";
const char *motiontopic = "motionStatus";
const char *wearablemotiontopic = "wearablemotionStatus";
const char *wearablebpmtopic = "wearablebpmStatus";
const char *mqtt_username = "SeizureSensor";
const char *mqtt_password = "subpublol";
const int mqtt_port = 1883;

// Pin Configuration
const int buzzerPin = 5;     // GPIO pin connected to the buzzer
const int neoPixelPin = 12;  // GPIO pin connected to the NeoPixel ring's data input
const int numPixels = 12;    // Number of pixels

// Alarm Configuration
const int frequency = 1000;    // Frequency of the tone in Hz
const int duration = 500;      // Duration of each beep and color change in milliseconds
const int pauseDuration = 500; // Pause between beeps and color changes in milliseconds

// Initialize WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// Initialize the NeoPixel ring
Adafruit_NeoPixel pixels(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

// Global variables to store status updates
struct Status {
  bool emg = false;
  bool motion = false;
  bool wearablemotion = false;
  bool wearablebpm = false;
};

Status status;

void setup() {
  Serial.begin(115200);
  while (!Serial); // optionally wait for serial terminal to open
  Serial.println("Alarm On");
  
  pinMode(buzzerPin, OUTPUT);
  pixels.begin();
  pixels.show();  // Turn off all pixels to start
  
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

  if (at_least_three_conditions_true()) {
    // Red color and beep
    setColor(255, 0, 0); // Red
    tone(buzzerPin, frequency, duration);
    delay(duration);
    
    // Pause
    noTone(buzzerPin);
    pixels.clear();
    pixels.show();
    delay(pauseDuration);

    // Green color and beep
    setColor(255,0, 0); // Green
    tone(buzzerPin, frequency, duration);
    delay(duration);
    
    // Pause
    noTone(buzzerPin);
    pixels.clear();
    pixels.show();
    delay(pauseDuration);

    // Blue color and beep
    setColor(255, 0, 0); // Blue
    tone(buzzerPin, frequency, duration);
    delay(duration);
    
    // Pause
    noTone(buzzerPin);
    pixels.clear();
    pixels.show();
    delay(pauseDuration);
  } else {
    // Turn off alarm
    noTone(buzzerPin);
    pixels.clear();
    pixels.show();
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    String client_id = "joseph-jostar";
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Mosquitto MQTT broker connected");
      // Subscribe to topics
      client.subscribe(emgtopic);
      client.subscribe(motiontopic);
      client.subscribe(wearablemotiontopic);
      client.subscribe(wearablebpmtopic);
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;
 
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
 
  if (String(topic) == emgtopic) {
    status.emg = (messageTemp == "yes");
  }
  else if(String(topic) == motiontopic){
    status.motion = (messageTemp == "yes");
  }
  else if(String(topic) == wearablemotiontopic){
    status.wearablemotion = (messageTemp == "yes");
  }
  else if(String(topic) == wearablebpmtopic){
    status.wearablebpm = (messageTemp == "yes");
  }

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  Serial.println(messageTemp);
  Serial.println("Current status:");
  Serial.print("EMG: ");
  Serial.print(status.emg);
  Serial.print(", Motion: ");
  Serial.print(status.motion);
  Serial.print(", wearable Motion: ");
  Serial.print(status.wearablemotion);
  Serial.print(", wearable BPM: ");
  Serial.println(status.wearablebpm);
}

bool at_least_three_conditions_true() {
  int count = 0;
  if (status.emg) count++;
  if (status.motion) count++;
  if (status.wearablemotion) count++;
  if (status.wearablebpm) count++;
  return count >= 3;
}

// Function to set all pixels to a specific color
void setColor(int red, int green, int blue) {
  for(int i = 0; i < numPixels; i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  pixels.show();
}
