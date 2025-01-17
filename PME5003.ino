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
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

//file the "********" 
char ssid[] = "********";
char pass[] = "********";


// Both variable is used to count how often the sensor can't read the AQI.
int good = 0 ;
int bad = 0 ;

void setup() {
  // Wait for serial monitor to open
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  while (!Serial) delay(10);

  Serial.println("Adafruit PMSA003I Air Quality Sensor");
  // Wait one second for sensor to boot up!
  delay(1000);
  // If using serial, initialize it and set baudrate before starting!
  // Uncomment one of the following
  //Serial1.begin(9600);
  pmSerial.begin(9600);
  // There are 3 options for connectivity!
  //if (! aqi.begin_I2C()) { // connect to the sensor over I2C
  //if (! aqi.begin_UART(&Serial1)) { // connect to the sensor over hardware serial


  if (!aqi.begin_UART(&pmSerial)) {  // connect to the sensor over software serial
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }
  Serial.println("PM25 found!");
}

void loop() {
  Blynk.run();
  PM25_AQI_Data data;
  if (!aqi.read(&data)) {
    // could if sesor can't read
    Serial.println("Could not read from AQI");
    delay(500);  // try again in a bit!
    bad++;
    return;
  } else {
    // Serial.println("AQI reading success");
    // Serial.println();
    // Serial.println(F("---------------------------------------"));
    // Serial.println(F("Concentration Units (standard)"));
    // Serial.println(F("---------------------------------------"));
    // Serial.print(F("PM 1.0: "));
    // Serial.print(data.pm10_standard);
    // Serial.print(F("\t\tPM 2.5: "));
    // Serial.print(data.pm25_standard);
    // Serial.print(F("\t\tPM 10: "));
    // Serial.println(data.pm100_standard);
    Serial.println(F("Concentration Units (environmental)"));
    Serial.println(F("---------------------------------------"));
    Serial.print(F("PM 1.0: "));
    Serial.print(int(data.pm10_env / 1.65));
    Blynk.virtualWrite(V0, int(data.pm10_env / 1.65));
    Serial.print(F("\t\tPM 2.5: "));
    Serial.print(int(data.pm25_env / 1.65));
    Blynk.virtualWrite(V1, int(data.pm25_env / 1.65));
    Serial.print(F("\t\tPM 10: "));
    Serial.println(int(data.pm100_env / 1.65));
    Blynk.virtualWrite(V2, int(data.pm100_env / 1.65));
    Serial.println(F("---------------------------------------"));
    // Serial.print(F("Particles > 0.3um / 0.1L air:"));
    // Serial.println(data.particles_03um);
    // Serial.print(F("Particles > 0.5um / 0.1L air:"));
    // Serial.println(data.particles_05um);
    // Serial.print(F("Particles > 1.0um / 0.1L air:"));
    // Serial.println(data.particles_10um);
    // Serial.print(F("Particles > 2.5um / 0.1L air:"));
    // Serial.println(data.particles_25um);
    // Serial.print(F("Particles > 5.0um / 0.1L air:"));
    // Serial.println(data.particles_50um);
    // Serial.print(F("Particles > 10 um / 0.1L air:"));
    // Serial.println(data.particles_100um);
    good++;
    Serial.print("good = ");
    Serial.println(good);
    Serial.print("bad = ");
    Serial.println(bad);
    Serial.println(F("---------------------------------------"));
  }
  delay(1000);
}
// อ่าน 3 ค่าแล้วทำค่k เฉลี่ย