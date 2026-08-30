# Comprehensive ECG & Stress Monitor with ESP32-S3 and AD8232 🫀

A complete, real-time Electrocardiogram (ECG) monitoring system built with the **ESP32-S3-WROOM-1**, **AD8232 ECG Sensor**, and an **SH1106 OLED Display**. This project not only displays real-time heart data on a screen but also streams raw data for advanced Python-based stress analysis.

---

## 🌟 Features
*   **High-Speed Sampling:** Precise 250Hz non-blocking sampling using `micros()` for accurate signal recreation.
*   **Real-Time OLED Dashboard:** Displays current BPM, R-R interval (ms), raw sensor values, and total sample count at 5Hz.
*   **Visual Heartbeat:** A dynamic pulsing circle on the screen that beats with your heart.
*   **Lead-Off Detection:** Automatically detects and alerts you if the electrode pads (LO+ / LO-) fall off the patient.
*   **Advanced Python Analysis:** Scripts to filter noise, calculate exact BPM from raw data, and dynamically detect physical or emotional **Stress Zones**.

---

## 📖 Part 1: What is an ECG?
An ECG (electrocardiogram) is a quick and painless test that records the electrical activity of your heart, including its rate and rhythm. 
ECG test results can help diagnose:
1. Irregular heartbeats (arrhythmias).
2. A previous heart attack.
3. The cause of chest pain (e.g., signs of blocked or narrowed heart arteries).
It is also used to assess the overall cardiovascular baseline of a patient.

---

## 🛠️ Part 2: Hardware & Wiring

### Components Needed:
*   ESP32-S3-WROOM-1 Microcontroller
*   AD8232 ECG Sensor Module (with 3-lead ECG cables and electrode pads)
*   SH1106 1.3" OLED Display (I2C)
*   Jumper Wires & Breadboard

### Pin Configuration:
Connect your components according to this table based on the provided code:

| Component | Pin Name | ESP32-S3 Pin | Note |
| :--- | :--- | :--- | :--- |
| **AD8232** | Output (Signal) | `GPIO 9` | Analog Input |
| **AD8232** | LO+ | `GPIO 20` | Leads-off Detection |
| **AD8232** | LO- | `GPIO 21` | Leads-off Detection |
| **AD8232** | 3.3V | `3.3V` | Power |
| **AD8232** | GND | `GND` | Ground |
| **OLED (I2C)**| SDA | `GPIO 6` | I2C Data |
| **OLED (I2C)**| SCL | `GPIO 5` | I2C Clock |
| **OLED (I2C)**| VCC & GND | `3.3V` / `GND` | Power |

![wiring](https://github.com/TOXIC-MM/ECG-With-ESP-Final/blob/main/wiring.png?raw=true)

---

## 💻 Part 3: Software & Code Overview

### Prerequisites
Before uploading the code, ensure you have the following installed in your Arduino IDE:
*   **ESP32 Board Package:** (by Espressif)
*   **U8g2 Library:** For the SH1106 OLED Display (Install via Arduino Library Manager).

### How the Code Works
1.  **Non-Blocking Logic:** Instead of using `delay()`, the code uses `micros()` to ensure the ECG is sampled exactly 250 times a second without freezing the processor.
2.  **Threshold-based BPM:** A simple threshold (`THRESHOLD = 1500`) detects the R-peak of the QRS complex. The time between peaks (beat interval) is used to calculate the real-time BPM shown on the OLED.
3.  **Serial Streaming:** The raw analog data is pushed via `Serial.println(rawValue);` at 9600 baud rate so it can be captured by PC software.

---

## 🧪 Part 4: Testing the Project

### 1. Patient Preparation
Ensure the skin is clean before applying the electrode pads (usually Right Arm, Left Arm, and Right Leg). 

### 2. Live Monitoring
Once the pads are connected, look at the OLED screen. You should see the Leads status change to "OK" and the BPM start calculating. 
To view the live graph on your PC, connect the ESP32 and open the [Better Serial Plotter App](https://github.com/nathandunk/BetterSerialPlotter/releases).

### 3. Advanced Data Analysis (Stress Detection)
The raw data recorded via the Serial port can be processed to extract deep insights. Our Python analysis filters out noise and detects **Stress Zones** (highlighted in red) when the heart rate deviates significantly from the patient's moving average.

**Example 1: Normal vs. Stressed State**
Below is a graph recorded during testing. The first section shows the patient relaxed, while the red peaks show where the algorithm detected a stress-induced heart rate spike.
![Analysis on Person 1-1](https://github.com/Mabini-AIO/ECG-With-ESP-V2/blob/main/Processed/1/1.png)
![Analysis on Person 1-2](https://github.com/Mabini-AIO/ECG-With-ESP-V2/blob/main/Processed/1/2.png)
![Analysis on Person 1-3](https://github.com/Mabini-AIO/ECG-With-ESP-V2/blob/main/Processed/1/3.png)
![Analysis on Person 1-4](https://github.com/Mabini-AIO/ECG-With-ESP-V2/blob/main/Processed/1/4.png)
![Analysis on Person 2-1](https://github.com/Mabini-AIO/ECG-With-ESP-V2/blob/main/Processed/2/1.png)
![Analysis on Person 2-2](https://github.com/Mabini-AIO/ECG-With-ESP-V2/blob/main/Processed/2/2.png)


---
*Created by Mabini.AIO*
