#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_BMP085.h>
#include <AM2302-Sensor.h>
#include <MQUnifiedsensor.h>

// WiFi credentials
const char* ssid = "Barudak Iqbal";
const char* password = "kngr2424xyz";

// Telegram Bot token
const char* botToken = "7193886690:AAFu7Cw5hAKsXvnE9pPho5tQBTuCpYOxNsI";
// Telegram chat ID
const char* chatId = "7022717726";

// Define pins for sensors
#define SENSOR_PIN_DHT22 16 // Pin for DHT22 sensor
#define PIN_ANALOG_MQ2 2 // Analog pin for MQ-2 sensor
#define PIN_DIGITAL_MQ2 26 // Digital pin for MQ-2 sensor

// Define duration for mixer activation (in milliseconds)
#define MIXER_DURATION 60000 // 1 minute in milliseconds

// Initialize objects for sensors
Adafruit_BMP085 bmp;
AM2302::AM2302_Sensor dht22(SENSOR_PIN_DHT22);
MQUnifiedsensor mq2("ESP32", 5, 10, PIN_ANALOG_MQ2, "MQ-2");

WiFiClient client;
HTTPClient http;

void kirim_pesan(String isipesan){
  // Construct the URL for the Telegram Bot API
  String url = "https://api.telegram.org/bot";
  url += botToken;
  url += "/sendMessage?chat_id=";
  url += chatId;
  url += "&text=";
  url += isipesan;

  // Begin the HTTP request
  http.begin(client, url);

  // Send the HTTP request and get the response
  int httpResponseCode = http.GET();

  // Close the connection
  http.end();

  if (httpResponseCode == 200){
    Serial.println("Pesan Terkirim");
  }
}

void nyalakan_pengaduk() {
  // Logika untuk mengaktifkan pengaduk
  digitalWrite(PIN_DIGITAL_MQ2, HIGH);
  delay(MIXER_DURATION); // Tunggu selama 1 menit (60000 ms)
  digitalWrite(PIN_DIGITAL_MQ2, LOW); // Matikan pengaduk setelah 1 menit
}

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize sensors
  dht22.begin();
  bmp.begin();
  mq2.init();

  // Set pin mode for controlling the mixer
  pinMode(PIN_DIGITAL_MQ2, OUTPUT);
}

void loop() {
  // Read sensor data
  float temperature = dht22.get_Temperature();
  float pressure = bmp.readPressure();
  float ppm = mq2.readSensor();

  // Check water level and send Telegram message
  if (WiFi.status() == WL_CONNECTED) {
    String message = "Pilih opsi di bawah ini:\n- Cek Suhu\n- Cek Tekanan\n- Cek Kandungan Gas Metana\n- Nyalakan Pengaduk";
    kirim_pesan(message);
  } else {
    Serial.println("Koneksi terputus");
  }

  delay(5000);
}
