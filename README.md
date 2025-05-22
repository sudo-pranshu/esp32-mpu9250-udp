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
| ESP32 Dev Board| Wi-Fi enabled microcontroller             |
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

## 📡 Receiving Data

You can receive and visualize the transmitted IMU data via:

- **Telemetry over Wi-Fi** using the **same router** connected to your laptop.
- A simple **Python or C++ UDP server** listening on the specified port (e.g., `8080`).
- Tools like `netcat` or custom **telemetry visualizer scripts**.

---
