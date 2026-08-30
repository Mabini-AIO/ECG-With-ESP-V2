#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

// Initialize U8g2 library for SH1106 128x64 I2C display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// === PINS ===
const int LO_PLUS = 20;
const int LO_MINUS = 21;
const int ECG_OUT = 9;  // Analog input from AD8232

// === HEARTBEAT VARIABLES ===
unsigned long lastBeatTime = 0;
unsigned long beatInterval = 0;
int bpm = 0;
bool beatDetected = false;
const int THRESHOLD = 1500;  // Adjust as needed

// === SAMPLING VARIABLES ===
unsigned long sampleCount = 0;
const unsigned long SAMPLE_RATE_HZ = 250;  // 250Hz sampling
const unsigned long SAMPLE_INTERVAL_US = 1000000 / SAMPLE_RATE_HZ;

void setup() {
  Serial.begin(9600);
  Wire.begin(6, 5);  // SDA, SCL pins for ESP32-S3
  
  u8g2.begin();
  
  pinMode(LO_PLUS, INPUT);
  pinMode(LO_MINUS, INPUT);
  pinMode(ECG_OUT, INPUT);

  // Show initialization message
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 12, "ECG Monitor");
  u8g2.drawStr(0, 30, "Initializing...");
  u8g2.drawStr(0, 48, "Sampling @ 250Hz");
  u8g2.sendBuffer();
  delay(1500);
}

void loop() {
  static unsigned long lastSampleTime = 0;
  static unsigned long lastDisplayUpdate = 0;
  unsigned long currentMicros = micros();
  unsigned long currentMillis = millis();

  // === Sample ECG signal at 250Hz ===
  if (currentMicros - lastSampleTime >= SAMPLE_INTERVAL_US) {
    lastSampleTime = currentMicros;
    int rawValue = analogRead(ECG_OUT);
    Serial.println(rawValue);
    sampleCount++;
    processHeartbeat(rawValue);
  }

  // === Update OLED display at 5Hz ===
  if (currentMillis - lastDisplayUpdate >= 200) {
    lastDisplayUpdate = currentMillis;
    updateDisplay();
  }
}

void processHeartbeat(int rawValue) {
  if (rawValue > THRESHOLD && !beatDetected) {
    beatDetected = true;
    unsigned long currentTime = millis();

    if (lastBeatTime > 0) {
      beatInterval = currentTime - lastBeatTime;
      bpm = 60000 / beatInterval;
    }

    lastBeatTime = currentTime;
  }

  if (rawValue < THRESHOLD) {
    beatDetected = false;
  }
}

void updateDisplay() {
  int rawValue = analogRead(ECG_OUT);
  bool leadsOff = digitalRead(LO_PLUS) || digitalRead(LO_MINUS);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  // Line 1: BPM and beat indicator
  u8g2.setCursor(0, 12);
  u8g2.print("BPM: ");
  u8g2.print(bpm);
  u8g2.print(" (");
  u8g2.print(beatInterval);
  u8g2.print("ms)");
  
  // Heartbeat indicator (pulsing circle)
  u8g2.drawCircle(120, 10, beatDetected ? 5 : 2, U8G2_DRAW_ALL);

  // Line 2: Raw ECG value
  u8g2.setCursor(0, 30);
  u8g2.print("Raw: ");
  u8g2.print(rawValue);

  // Line 3: Lead status
  u8g2.setCursor(0, 48);
  u8g2.print("Leads: ");
  u8g2.print(leadsOff ? "OFF!" : "OK");

  // Line 4: Sampling info
  u8g2.setCursor(0, 64);
  u8g2.print("Samples: ");
  u8g2.print(sampleCount);
  u8g2.print(" @");
  u8g2.print(SAMPLE_RATE_HZ);
  u8g2.print("Hz");

  u8g2.sendBuffer();
}