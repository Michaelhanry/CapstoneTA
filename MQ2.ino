#include <MQUnifiedsensor.h>

#define PIN_ANALOG 2 // Pin analog untuk membaca sensor MQ-2 (GIO2)
#define PIN_DIGITAL 26 // Pin digital untuk membaca output digital dari sensor MQ-2 (GIO26)
#define RLOAD 10 // Resistance load pada sensor (ohm)
#define MQ2_MODEL "MQ-2" // Model sensor MQ-2

MQUnifiedsensor MQ2("ESP32", 5, 10, PIN_ANALOG, "MQ-2");

void setup() {
  Serial.begin(9600); // Mulai komunikasi serial
}

void loop() {
  float ppm = MQ2.readSensor(); // Membaca nilai ppm (parts per million) gas metana
  
  Serial.print("PPM gas metana: ");
  Serial.println(ppm); // Tampilkan nilai ppm pada serial monitor
  
  delay(2000); // Tunggu 1 detik sebelum membaca sensor lagi
}
