# Seizure Sensor: From Personal Need to Innovative Solution

The genesis of Seizure Sensor lies in a deeply personal experience. Kevork's brother, who suffers from epilepsy, faces the constant threat of life-threatening seizures during the night. Their parents, ever vigilant, relied on a basic webcam system to monitor him, forcing them to sleep lightly and often missing critical moments when seizures occurred.

Determined to create a better solution, we set out to integrate cutting-edge technology into a more reliable monitoring system. Our goal was to combine computer vision, wearable tech, and biometric sensors to provide accurate, timely detection and alerts.

## The Development Journey

1. **Initial Approach**: We began by developing an iOS app that would leverage Apple Watch's biometric capabilities. However, we encountered significant challenges interfacing with the Apple development platform, particularly in exporting data to our MQTT server.

2. **Pivot and Innovation**: Faced with these obstacles, we made a crucial decision to pivot. Instead of relying on existing consumer devices, we opted to create our own wearable technology using ESP32 microcontrollers. This shift allowed us greater control and flexibility in data collection and transmission.

3. **Custom Hardware**: Our custom wearable incorporated an EMG sensor, an accelerometer, and a heart rate monitor, providing a comprehensive set of biometric data.

4. **Data Integration**: We set up an MQTT server on a Raspberry Pi to integrate all sensor inputs effectively. This central hub became the brain of our operation, processing and analyzing the incoming data streams.

5. **Alert System**: We developed an ESP32-powered visual and auditory alarm system to clearly indicate when a seizure is detected, ensuring that caregivers are promptly alerted.

6. **Computer Vision Integration**: Utilizing OpenCV, we incorporated computer vision capabilities into our system. While we leveraged many built-in OpenCV algorithms, we recognized the need for more specialized training on seizure footage to enhance the model's robustness.

## Challenges and Learnings

- We gained valuable experience in integrating multiple sensory readings into an MQTT server, enabling accurate biometric monitoring.
- The project taught us the importance of flexibility in development, as evidenced by our successful pivot from iOS to custom hardware.
- We recognized the potential for further improvement in our computer vision model, particularly with specialized training data.

## Ethical Considerations and Results

It's important to note that our project is a proof of concept. We did not induce epilepsy or seizures in any individuals during our testing. Instead, we focused on simulating seizure activity to prove the efficacy of our system.

Our results are promising:
- While the FDA standard for seizure monitoring devices is about 15 seconds of recorded repetitive motion, our system not only meets this benchmark using the accelerometer but also corroborates it with EMG and heart rate data.

## Looking Forward

As we continue to refine Seizure Sensor, we're excited about its potential to significantly improve the lives of those affected by epilepsy and their caregivers. Our journey from a personal challenge to an innovative solution underscores the power of determination, technical skill, and the drive to make a positive impact in healthcare technology.
