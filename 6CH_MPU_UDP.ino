//#include "esp_task_wdt.h"
#include <MPU9250_WE.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

/* WiFi network name and password */
const char* ssid       = "JioFi_2126455";
const char* pwd        = "im4g8qhryi";

const char* udpAddress = "192.168.1.102";   // NUC013
const int   udpPort    = 8080;            // server port

//create UDP instance
WiFiUDP udp;

/////////////MPU Setup/////////////
//VSPI
#define MPU_CS_PIN 5

const int8_t SYNC_BYTE = 0xAA;
int16_t Ax{ 0 };
int16_t Ay{ 0 };
int16_t Az{ 0 };

int16_t Gx{ 0 };
int16_t Gy{ 0 };
int16_t Gz{ 0 };

///////////////CONSTRUCTORS//////////////
MPU9250_WE myMPU9250 = MPU9250_WE(&SPI, MPU_CS_PIN, true);  //VSPI

void setup() {
//  Serial.begin(1500000);
    Serial.begin(115200);
  //esp_task_wdt_delete(NULL);

  myMPU9250.init();
  delay(1000);
  myMPU9250.autoOffsets();
  myMPU9250.disableGyrDLPF(MPU9250_BW_WO_DLPF_8800);  // bandwdith without DLPF
  // myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_500);
  myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_4G);
  myMPU9250.enableAccDLPF(false);
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);
  myMPU9250.setMagOpMode(AK8963_CONT_MODE_100HZ);
  delay(1000);

   //Connect to the WiFi network
  WiFi.begin(ssid, pwd);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //This initializes udp and transfer buffer
  udp.begin(udpPort);
  Serial.println("WiFi Connected - UDP Started");
}

void loop() {
  xyzFloat accValue = myMPU9250.getAccRawValues();
  xyzFloat gyr = myMPU9250.getGyrRawValues();
  udp.beginPacket(udpAddress, udpPort);
  for (;;) {
    //esp_task_wdt_deinit();

    accValue = myMPU9250.getAccRawValues();  // for g values use myMPU9250.getGValues()
    gyr = myMPU9250.getGyrRawValues();       // for gyro use myMPU9250.getGyrValues()
    Ax = accValue.x;
    Ay = accValue.y;
    Az = accValue.z;

    Gx = gyr.x;
    Gy = gyr.y;
    Gz = gyr.z;


    udp.write(SYNC_BYTE);  // Send the start/sync byte

    udp.write((uint8_t*)&(Ax), sizeof(Ax));
    udp.write((uint8_t*)&(Ay), sizeof(Ay));
    udp.write((uint8_t*)&(Az), sizeof(Az));

    udp.write((uint8_t*)&(Gx), sizeof(Gx));
    udp.write((uint8_t*)&(Gy), sizeof(Gy));
    udp.write((uint8_t*)&(Gz), sizeof(Gz));

  }
}
