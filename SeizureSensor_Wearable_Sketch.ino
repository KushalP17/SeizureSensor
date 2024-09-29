#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const int emgPin = 33;
const char* ssid = "G34";
const char* password = "designthinking";
const char *mqtt_broker = "192.168.1.12";
const char *emgrawtopic = "emgRaw";
const char *emgstatustopic = "emgStatus";
const char *motionstatustopic = "wearablemotionStatus";
const char *wearablebpmstatustopic = "wearablebpmStatus";
const char *wearablebpmrawtopic = "wearablebpmRaw";
const char *mqtt_username = "SeizureSensor";
const char *mqtt_password = "subpublol";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int upBound = 1800;
const int lowBound = 1600;
const int sampleTime = 400;
volatile int emgValue = 0;
bool spike = false;

// New variables for seizure detection
const int EMG_THRESHOLD = 1200;
const int SPIKE_COUNT_THRESHOLD = 5;
const unsigned long DETECTION_WINDOW = 5000; // 5 seconds in milliseconds
const unsigned long RESET_INTERVAL = 300000; // 5 minutes in milliseconds

int spikeCount = 0;
bool emgSeizureStatus = false;
unsigned long lastSpikeTime = 0;
unsigned long lastResetTime = 0;


//Accel init
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(12121);

const float ACCELERATION_THRESHOLD = 5.0;
const int REPETITIVE_MOTION_COUNT = 5;
const unsigned long DETECTION_WINDOw = 3000; // 3 seconds in milliseconds
const unsigned long STATUS_DURATION = 900000; // 15 minutes in milliseconds

int repetitiveMotionCounter = 0;
bool seizureDetected = false;
unsigned long lastMotionTime = 0;
unsigned long seizureDetectionTime = 0;

float lastAccelX = 0, lastAccelY = 0, lastAccelZ = 0;


//BPM init
#define PULSE_SENSOR_PIN 32

// Constants
const int GLOBAL_THRESH = 525;  // Adjusted for Arduino's 10-bit ADC

// Variables for pulse detection
unsigned long sampleCounter = 0;
unsigned long lastBeatTime = 0;
int IBI = 600;
bool Pulse = false;
bool QS = false;
int P = 512;
int T = 512;
int thresh = GLOBAL_THRESH;
int amp = 0;
bool firstBeat = true;
bool secondBeat = false;
int rate[10];
int BPM = 0;
int BPMPrev = 50;

bool calibrated = false;
int arrBPM[10]; 
int running_average = 50;
int bpmFlag = 0;
String bpmSeizure = "no";
int i = 0;

void setup() {
  Serial.begin(115200);
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

  if (!accel.begin()) {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }

  accel.setRange(LSM303_RANGE_16G);
  Serial.println("LSM303 accel initialized");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 
  emgValue = analogRead(emgPin);
  Serial.println(emgValue);
 
  // EMG seizure detection logic
  unsigned long currentTime = millis();
 
  if (emgValue > EMG_THRESHOLD) {
    if (currentTime - lastSpikeTime <= DETECTION_WINDOW) {
      spikeCount++;
      if (spikeCount >= SPIKE_COUNT_THRESHOLD) {
        emgSeizureStatus = true;
        Serial.println("EMG Seizure detected!");
      }
    } else {
      spikeCount = 1;
    }
    lastSpikeTime = currentTime;
  }
 
  // Reset logic
  if (currentTime - lastResetTime >= RESET_INTERVAL) {
    spikeCount = 0;
    lastResetTime = currentTime;
    //Serial.println("EMG Spike count reset");
  }
 
  //Publish EMG value
  char tempString[8];
  dtostrf(emgValue, 1, 0, tempString);
  client.publish(emgrawtopic, tempString);
 
  // Publish spike status (original logic)
  if(emgValue > upBound || emgValue < lowBound){
    spike = true;
    //client.publish(emgstatustopic, "true");
  }
  else{
    spike = false;
    //client.publish(emgstatustopic, "false");
  }
 
  // You might want to publish the new emgSeizureStatus as well
  if(emgSeizureStatus){
    client.publish(emgstatustopic, "yes");
  }
  
  //------------------------------------------------------------------
  sensors_event_t event;
  accel.getEvent(&event);
 
  float accelX = event.acceleration.x;
  float accelY = event.acceleration.y;
  float accelZ = event.acceleration.z;
 
  float deltaX = abs(accelX - lastAccelX);
  float deltaY = abs(accelY - lastAccelY);
  float deltaZ = abs(accelZ - lastAccelZ);
 
  if (deltaX > ACCELERATION_THRESHOLD || deltaY > ACCELERATION_THRESHOLD || deltaZ > ACCELERATION_THRESHOLD) {
    unsigned long currentTime = millis();
   
    if (currentTime - lastMotionTime <= DETECTION_WINDOw) {
      repetitiveMotionCounter++;
     
      if (repetitiveMotionCounter >= REPETITIVE_MOTION_COUNT) {
        seizureDetected = true;
        seizureDetectionTime = currentTime;
        Serial.println("Seizure detected!");
        if(seizureDetected){
          client.publish(motionstatustopic, "yes");
        }
        repetitiveMotionCounter = 0; // Reset the counter
      }
    } else {
      repetitiveMotionCounter = 1; // Reset counter but count this motion
    }
   
    lastMotionTime = currentTime;
  }
 
  // Check if 15 minutes have passed since seizure detection
  if (seizureDetected && (millis() - seizureDetectionTime > STATUS_DURATION)) {
    seizureDetected = false;
    //emgSeizureStatus = false;
    //Serial.println("Seizure status reset");
  }
 
  // Update last acceleration values
  lastAccelX = accelX;
  lastAccelY = accelY;
  lastAccelZ = accelZ;
 
  // Print acceleration values and seizure status
  //Serial.print("X: "); Serial.print(accelX);
  //Serial.print(" Y: "); Serial.print(accelY);
  //Serial.print(" Z: "); Serial.print(accelZ);
  //Serial.print(" m/s^2 | Seizure Status: ");
  //Serial.println(seizureDetected ? "TRUE" : "FALSE");

  //------------------------------------------------------------------------------

  int Signal = analogRead(PULSE_SENSOR_PIN);
 
  sampleCounter += 2;
  int N = sampleCounter - lastBeatTime;
 
  if (Signal < thresh && N > (IBI / 5) * 3) {
    if (Signal < T) {
      T = Signal;
    }
  }
 
  if (Signal > thresh && Signal > P) {
    P = Signal;
  }
 
  if (N > 250) {  // avoid high frequency noise
    if ((Signal > thresh) && (Pulse == false) && (N > (IBI / 5) * 3)) {
      Pulse = true;
      
     
      IBI = sampleCounter - lastBeatTime;
      lastBeatTime = sampleCounter;
     
      if (firstBeat) {
        firstBeat = false;
        secondBeat = true;
      } else if (secondBeat) {
        secondBeat = false;
        for (int i = 0; i < 10; i++) {
          rate[i] = IBI;
        }
      } else {
        // Keep a running total of the last 10 IBI values
        unsigned long runningTotal = 0;
        for (int i = 0; i < 9; i++) {
          rate[i] = rate[i + 1];
          runningTotal += rate[i];
        }
        rate[9] = IBI;
        runningTotal += rate[9];
        runningTotal /= 10;
        BPM = 60000 / runningTotal;
       
        Serial.print("BPM: ");
        Serial.println(BPM);

        if(!calibrated && BPM>50){
          calibrated = true;
        }


        if(calibrated){
          
          if(BPM > 1.2*BPMPrev || BPM > (20+BPMPrev)){
            bpmFlag++;
          }
          else if(bpmFlag > 0){
            if(BPM > .9*BPMPrev){
              bpmFlag++;
            }else{
              bpmFlag = 0;
            }
          }

          char tempString[8];
          dtostrf(BPM, 1, 0, tempString);
          client.publish(wearablebpmrawtopic, tempString);
          Serial.println(tempString);

          Serial.print("BPM FLAG: ");
          Serial.println(bpmFlag);
          if(bpmFlag>3){
            bpmSeizure = "yes";
            //Serial.println(bpmSeizure);
            
            

            if(bpmSeizure == "yes"){
              client.publish(wearablebpmstatustopic, "yes");
            }
            
            bpmFlag = 0;
          }


          BPMPrev = BPM;
        }

      }
    }
  }
 
  if (Signal < thresh && Pulse == true) {
    //digitalWrite(LED_PIN, LOW);  // Turn off LED
    Pulse = false;
   
    amp = P - T;
    thresh = amp / 2 + T;
    P = thresh;
    T = thresh;
  }
 
  if (N > 2500) {
    thresh = GLOBAL_THRESH;
    P = 512;
    T = 512;
    lastBeatTime = sampleCounter;
    firstBeat = true;
    secondBeat = false;
  }

 
  delay(sampleTime);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    String client_id = "wearable";
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
  // Callback function remains empty as per original code
}
