#include <MPU9250_WE.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

/* WiFi credentials */
const char* ssid = "JioFi_2126455";
const char* pwd  = "im4g8qhryi";

/* UDP target */
const char* udpAddress = "192.168.1.100"; // PC/server IP
const int udpPort = 8080;                 // Listening port on  PC/server

WiFiUDP udp;

// MPU9250 settings
#define MPU_CS_PIN 5
const int8_t SYNC_BYTE = 0xAA;

int16_t Ax{0}, Ay{0}, Az{0};
int16_t Gx{0}, Gy{0}, Gz{0};

// Create MPU object using SPI
MPU9250_WE myMPU9250 = MPU9250_WE(&SPI, MPU_CS_PIN, true); // true = use SPI

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting serial debug...");

  // Initialize MPU9250
  Serial.println("Initializing MPU...");
  myMPU9250.init();
  delay(1000);

  myMPU9250.autoOffsets(); // auto-calibrate offsets

  myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);   // Lower range = higher precision
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_4G);     // Moderate range for stability

  myMPU9250.setGyrDLPF(MPU9250_DLPF_6); // Enables + sets gyro digital low-pass filter
  myMPU9250.setAccDLPF(MPU9250_DLPF_6); // Enables + sets accel low-pass filter

  myMPU9250.setMagOpMode(AK8963_CONT_MODE_100HZ);  // Magnetometer sampling rate

  delay(1000);
  Serial.println("MPU initialized.");

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, pwd);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - start > 10000) {
      Serial.println("\nWiFi connection failed.");
      return;
    }
  }
  Serial.println("\nWiFi connected.");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  // Start UDP
  udp.begin(udpPort);
  Serial.println("UDP started.");
}

void loop() {
  // Start the UDP packet (your professor's style: call only once)
  udp.beginPacket(udpAddress, udpPort);
  Serial.println("Sending IMU data forever...");

  while (true) {
    xyzFloat accValue = myMPU9250.getAccRawValues();
    xyzFloat gyrValue = myMPU9250.getGyrRawValues();

    Ax = accValue.x;
    Ay = accValue.y;
    Az = accValue.z;
    Gx = gyrValue.x;
    Gy = gyrValue.y;
    Gz = gyrValue.z;

    // Serial debug
    Serial.print("A: ");
    Serial.print(Ax); Serial.print(", ");
    Serial.print(Ay); Serial.print(", ");
    Serial.print(Az); Serial.print(" | G: ");
    Serial.print(Gx); Serial.print(", ");
    Serial.print(Gy); Serial.print(", ");
    Serial.println(Gz);

    // Send data over UDP (raw bytes)
    udp.write(SYNC_BYTE);  // Sync start
    udp.write((uint8_t*)&Ax, sizeof(Ax));
    udp.write((uint8_t*)&Ay, sizeof(Ay));
    udp.write((uint8_t*)&Az, sizeof(Az));
    udp.write((uint8_t*)&Gx, sizeof(Gx));
    udp.write((uint8_t*)&Gy, sizeof(Gy));
    udp.write((uint8_t*)&Gz, sizeof(Gz));
  }

}