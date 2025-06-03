# MPU9250 Telemetry via UDP on ESP32

## 📌 Project Objective

To reliably retrieve raw **IMU (accelerometer + gyroscope)** data from the **MPU9250** sensor using **ESP32** over **UDP**, especially in **high EMI (Electromagnetic Interference)** environments.

### Goals:
1. **Minimum corruption** during transmission.
2. **Graceful recovery** in case of corrupted packets.

---

## 🧰 Components Used

| Component       | Description                               |
|----------------|-------------------------------------------|
| ESP32 DevKit V1| Wi-Fi enabled microcontroller             |
| MPU9250        | 9-axis IMU (accelerometer + gyroscope + magnetometer) |
| Jumper wires   | For connections                           |
| Breadboard     | For easy prototyping                      |
| Power Supply   | 3.3V from ESP32 or external regulated source |

---

## 🔌 Connections (SPI Mode)

| ESP32 Pin | MPU9250 Pin |
|-----------|-------------|
| 5         | CS (Chip Select) |
| 18        | SCK(SCL) |
| 19        | MISO(SD0/AD0) |
| 23        | MOSI(SDA) |
| 3.3V      | VCC |
| GND       | GND |
| FSYNC     | GND |

> Ensure MPU9250 is operating at **3.3V logic levels**.

---

## 🧠 Technical Implementation

- The MPU9250 is interfaced using **SPI** with the `MPU9250_WE` library.
- UDP packets are sent from ESP32 to a local receiver (laptop/server).
- **`udp.beginPacket()`** is used **once only**.
- The UDP data is streamed **inside a forever `while(1)` loop**.
- **No `udp.endPacket()` is used**, which allows faster continuous streaming.

### 🚨 Trick:
> To maximize packet throughput, **DO NOT call `endPacket()`** inside the loop. Just use `beginPacket()` once and stream forever. This unconventional trick improves real-time performance.

---

## 📝 Additional Notes

- Best tested with **ESP32 Board Package v1.0.4** in Arduino.
    - This version offers **significantly higher transmission speeds**.
    - Newer versions (e.g., 3.x.x) may throttle or delay transmission.
- IMU data is filtered using:
  - `setGyrDLPF(MPU9250_DLPF_6)`
  - `setAccDLPF(MPU9250_DLPF_6)`
  - `setGyrRange(MPU9250_GYRO_RANGE_250)`
  - `setAccRange(MPU9250_ACC_RANGE_4G)`

---

## 📎 Project Setup & Usage (Arduino + Telemetry)

### 🔧 Steps to Run the Project

1. Open the provided file: `6CH_MPU_UDP.ino` in **Arduino IDE**.
2. Set the **Board** as: `ESP32 Dev Module`.
3. Change the following in the Arduino sketch:
   - Replace Wi-Fi SSID and password:
     ```cpp
     const char* ssid = "YOUR_WIFI_NAME";
     const char* pwd  = "YOUR_WIFI_PASSWORD";
     ```
   - Set the **receiver IP address** of your telemetry device:
     ```cpp
     const char* udpAddress = "192.168.x.x"; // replace with your PC or NUC IP
     ```
   - Optionally modify the **telemetry port**:
     ```cpp
     const int udpPort = 8080; // or any open port
     ```
4. Upload the code to your ESP32 and open the Serial Monitor to confirm Wi-Fi connection and IP.

---

### 📡 Telemetry Configuration Steps

1. Open your **telemetry software** (e.g., custom Python/C++ receiver or SerialPlot, etc.).
2. Set it to **binary receive mode**.
3. Create a **settings file** or manually configure:
   - **6 channels**.
   - **Data type**: `int16`.
   - **Byte order**: Little Endian (if prompted).
   - **Sample format**: Continuous binary stream.
4. Connect the telemetry tool to the same Wi-Fi router used by the ESP32.
5. Start receiving data from ESP32 on the specified port.
6. Use **"Time Elapsed Mode"** in the telemetry software to calculate frequency.

---

## 🚀 Sample Output Performance

- In testing with 6 `int16` channels (3-axis accelerometer + 3-axis gyroscope):
  - Achieved **21.1 kHz** sample rate (samples per second).
  - Each sample is **12 bytes** (6 channels × 2 bytes).

> Performance may vary depending on Wi-Fi conditions, router quality, and interference levels.

---

## 🧪 Debug Mode

- The code also prints IMU readings to Serial Monitor:
- A: -64, -7724, 2260 | G: 416, -534, 290

This is helpful to cross-check readings and validate correct data streaming.

---

## 📡 Receiving Data

You can receive and visualize the transmitted IMU data via:

- **Telemetry over Wi-Fi** using the **same router** connected to your laptop.
- A simple **Python or C++ UDP server** listening on the specified port (e.g., `8080`).
- Tools like `netcat` or custom **telemetry visualizer scripts**.

---

## Author

Pranshu Kumar  
IoT Research Intern, Machine Intelligence Program  
GitHub: https://github.com/sudo-pranshu


