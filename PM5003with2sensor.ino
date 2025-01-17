#define BLYNK_PRINT Serial
/* Fill in information from Blynk Device Info here */
// if want to use BLYNK uncomment below
// #define BLYNK_TEMPLATE_ID "******"
// #define BLYNK_TEMPLATE_NAME "******"
// #define BLYNK_AUTH_TOKEN "******"
// #include <BlynkSimpleEsp32.h>


/* Test sketch for Adafruit PM2.5 sensor with UART or I2C */
#include "Adafruit_PM25AQI.h"
#include <WiFi.h>
#include <WiFiClient.h>


// If your PM2.5 is UART only, for UNO and others (without hardware serial)
// we must use software serial...
// pin #2 is IN from sensor (TX pin on sensor), leave pin #3 disconnected
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
SoftwareSerial pmSerial(25, 26); //TX RX <------ port to connect (TX RX) ของ sensor ไม่ใข่ของ board ให้เอาไปต่อขา GPIO อะไรก็เเก่ได้เลย
SoftwareSerial pmSerial1(34, 35);
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
Adafruit_PM25AQI aqi1 = Adafruit_PM25AQI();

//file the "********" 
char ssid[] = "********";
char pass[] = "********";

// Both variable is used to count how often the sensor can't read the AQI.
int good = 0 ;
int bad = 0 ;

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);


  pmSerial.begin(9600);
  pmSerial1.begin(9600);

  if (!aqi.begin_UART(&pmSerial)) {
    Serial.println("Could not find PM 2.5 sensor 1!");
    while (1) delay(10);
  }

  if (!aqi1.begin_UART(&pmSerial1)) {
    Serial.println("Could not find PM 2.5 sensor 2!");
    while (1) delay(10);
  }

  Serial.println("PM25 sensors found!");
}

void loop() {
  Blynk.run();

  PM25_AQI_Data data;

  // merge with two soure
  // if two sensor can read will send data to could but not will throw
  // if (!aqi.read(&data) || !aqi1.read(&data)) {
  //   Serial.println("Could not read from AQI sensor 1");
  //   bad++;
  // } else {
  //   printPMData("Sensor 1", data);
  //   printPMData("Sensor 2", data);
  //   good++;
  // }
  
  if (!aqi.read(&data)) {
    Serial.println("Could not read from AQI sensor 1");
    bad++;

  } else {
    printPMData("Sensor 1", data);
  }

  if (!aqi1.read(&data)) {
    Serial.println("Could not read from AQI sensor 2");
    bad++;
  } else {
    printPMData("Sensor 2", data);
  }
  Serial.println(good);
  Serial.println(bad);
  delay(1000);
}

void printPMData(String sensorName, PM25_AQI_Data &data) {
  good++;
  Serial.println(sensorName + " Data:");
  Serial.print("PM 1.0: ");
  Serial.print(int(data.pm10_env / 1.65));
  Blynk.virtualWrite(V0, int(data.pm10_env / 1.65));

  Serial.print("\tPM 2.5: ");
  Serial.print(int(data.pm25_env / 1.65));
  Blynk.virtualWrite(V1, int(data.pm25_env / 1.65));

  Serial.print("\tPM 10: ");
  Serial.println(int(data.pm100_env / 1.65));
  Blynk.virtualWrite(V2, int(data.pm100_env / 1.65));

  Serial.println("------------------------------");
}