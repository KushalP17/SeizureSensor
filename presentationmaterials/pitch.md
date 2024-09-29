# Seizure Sensor: 5-Minute Presentation Pitch Outline

## Hook (30 seconds):
Imagine waking up every morning, not knowing if your loved one made it through the night. For millions affected by epilepsy, including my own brother, this is our daily reality. Epilepsy, a chronic brain disorder characterized by recurrent seizures, affects 5 million new people each year. Nearly 80% live in low to middle-income countries, where access to care is limited.
These seizures can strike at any time, even during sleep, potentially proving fatal without immediate attention. I've witnessed my parents' sleepless nights, their worry tangible as they've tried everything from camera systems to motion-tracking watches to keep my brother safe.
But what if we could change this? What if we could create a guardian that never sleeps, a vigilant protector that watches over your loved ones when you can't? Ladies and gentlemen, we present to you Seizure Sensor – the future of epilepsy monitoring, designed to bring peace of mind to millions of families like mine around the world.

2. Our Solution: Seizure Sensor (60 seconds)
   - Let’s break down how this works: the Seizure Sensor is a device that uses biometric data as well as motion data to help detect seizure-like activity at night. Our device collects emg data to identify cases of muscle contraction using a wearable muscle sensor with emg electrodes connected to the arm, as well as a wearable heart rate monitor on the index finger. Additionally, we have incorporated a wearable accelerometer to detect repetitive motion as well as an external camera using a CV algorithm to detect at least 8 seconds of captured motion from a mounted perspective. 
   - All of these metrics are used collectively to decide whether an alarm is triggered to alert the existence of seizure like conditions. Both the wearable as well as the CV algorithm use MQTT to publish their data to the alarm system. We set the alarm to only trigger if 3 out of the 4 following conditions were met:
	 EMG: Detect 5 impulses above a chosen threshold
	BPM: Detect a 20% increase across 5 seconds
	Accel: Detects 5 instances of repetitive motion within 5 seconds
	ComV: Detects 8 seconds of shifting motion

3. Development Journey (45 seconds)
   - We initially planned to implement our wearable device as an app on Apple watch, but we were met with a financial barrier requiring us to pay $99 for an Apple Developers license, so we pivoted to a wearable system, that actually serves the use case better as it is a significantly more affordable system which could greatly aid the low to middle income class communities.
   - Current prototype capabilities (Idk if we want to talk about 60 to 80% accuracy, we don’t really have a metric to define that)

4. Market Potential and Impact (45 seconds)
   - Size of the target market
   - Comparison with existing solutions
   - Potential to save lives and improve quality of life

5. Next Steps (45 seconds)
   - Given more time, we would really love the chance to test and refine the CV algorithm with an actual use case, however this would require IRB approval which was not in the scope of a 24-hour project. Additionally adding another biometric sensor like a galvanometer could further improve the accuracy of detecting seizure-like behavior.

## Outro Statement (30 seconds):
Our project demonstrates the potential for interdisciplinary approaches in addressing complex healthcare challenges. By combining expertise in hardware development, software engineering, and data analysis, we've created a system that has the potential to significantly improve the lives of individuals with epilepsy and their caregivers.
