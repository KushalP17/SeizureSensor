 parents relied on a basic webcam system to monitor him, often missing critical moments when seizures occurred. This pressing need for a more reliable monitoring system drove us to create Seizure Sensor.

## What it does

Seizure Sensor is an innovative monitoring system that combines:
- Wearable technology with EMG sensors, accelerometers, and heart rate monitors
- Computer vision using OpenCV for motion detection
- A central MQTT server on a Raspberry Pi for data integration
- An ESP32-powered visual and auditory alarm system

This integrated system provides accurate, timely detection of seizure activity and alerts caregivers promptly, significantly improving the monitoring process for people with epilepsy.

## How we built it

1. Initially, we attempted to develop an iOS app using Apple Watch for biometric data.
2. Due to challenges with the Apple development platform, we pivoted to creating our own wearable using ESP32 microcontrollers.
3. We developed custom hardware incorporating various sensors for comprehensive biometric data collection.
4. We set up an MQTT server on a Raspberry Pi to integrate and process all sensor inputs.
5. We implemented computer vision capabilities using OpenCV for motion detection.
6. Finally, we created an alert system using ESP32 to provide clear visual and auditory alarms when a seizure is detected.

## Challenges we ran into

1. Difficulties interfacing with the Apple development platform, leading to our pivot to custom hardware.
2. Integrating multiple sensory inputs (EMG, accelerometer, heart rate) into a cohesive system.
3. Implementing effective computer vision algorithms for seizure detection.
4. Ensuring the system's reliability and accuracy without inducing actual seizures for testing.

## Accomplishments that we're proud of

1. Successfully created a proof-of-concept system that meets and exceeds current FDA standards for seizure monitoring devices.
2. Developed a custom wearable device that effectively collects and transmits critical biometric data.
3. Integrated multiple technologies (wearables, computer vision, data processing) into a cohesive system.
4. Created a system that not only detects motion but also corroborates it with EMG and heart rate data for more accurate seizure detection.

## What we learned

1. The importance of flexibility in the development process, as demonstrated by our successful pivot from iOS to custom hardware.
2. Techniques for integrating multiple sensory readings into an MQTT server for accurate biometric monitoring.
3. The complexities and potential of computer vision in medical applications.
4. The critical balance between innovation and ethical considerations in medical technology development.

## What's next for Seizure Sensor

1. Further refine and enhance the computer vision model with specialized training on seizure footage.
2. Conduct more extensive testing to validate the system's efficacy and reliability.
3. Explore partnerships with medical institutions for clinical trials.
4. Investigate the potential for miniaturization and commercialization of the technology.
5. Develop a user-friendly interface for caregivers to easily monitor and respond to alerts.
