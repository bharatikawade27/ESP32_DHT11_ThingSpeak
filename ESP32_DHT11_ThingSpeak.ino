#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// -----------------------------
// DHT11 Configuration
// -----------------------------
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// -----------------------------
// Wi-Fi Configuration
// -----------------------------
const char* ssid = "ABC123";
const char* password = "1234";

// -----------------------------
// ThingSpeak Configuration
// -----------------------------
unsigned long channelID = 1234;

const char* writeAPIKey = "2ASD";

// -----------------------------
// WiFi Client
// -----------------------------
WiFiClient client;

void setup() {

  Serial.begin(115200);

  // Start DHT11
  dht.begin();

  Serial.println();
  Serial.println("DHT11 + ESP32 + ThingSpeak");
  Serial.println();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {

  // Read humidity
  float humidity = dht.readHumidity();

  // Read temperature
  float temperature = dht.readTemperature();

  // Check sensor readings
  if (isnan(humidity) || isnan(temperature)) {

    Serial.println("Failed to read from DHT11!");

    delay(2000);

    return;
  }

  // Display values on Serial Monitor
  Serial.println();
  Serial.println("Sensor Readings");
  Serial.println("-------------------------");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Send data to ThingSpeak

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int responseCode = ThingSpeak.writeFields(
    channelID,
    writeAPIKey
  );

  if (responseCode == 200) {

    Serial.println("Data sent successfully to ThingSpeak!");

  } else {

    Serial.print("Error sending data. HTTP response code: ");
    Serial.println(responseCode);
  }

  Serial.println("-------------------------");

  // Wait before next update
  delay(20000);
}