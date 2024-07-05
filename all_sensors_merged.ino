#define BLYNK_TEMPLATE_ID "TMPL3AwTXlPyC"
#define BLYNK_TEMPLATE_NAME "Hydrponics System"
#define BLYNK_AUTH_TOKEN "AJXwBqUNwdA9J7rUFt4dUWW1VVmDgQGf"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_Sensor.h>



const char* ssid = "MAKAUT_WB";
const char* password =  "Cisco@123";



const int tdsPin = A0; // Analog pin for TDS sensor
const int tempPin = D2; // digital pin for temperature sensor
const int ultraSonicTrig = D0; // Trigger pin for ultrasonic sensor
const int ultraSonicEcho = D1; // Echo pin for ultrasonic sensor
const int fanPin = D7; // Digital pin for fan
const int npumpPin = D3;  //Digital pin for nutrients pump(TDS)
const int wpumpPin = D5;  //Digital pin for water pump
const int airpumpPin = D6;  //Digital pin for water pump


#define DHTTYPE DHT11 
DHT dht(tempPin, DHTTYPE);
long duration;

const float thresholdTDS = 400; // Change this based on desired TDS level for pump activation
const int tempFanOn = 30; // Change this based on desired temperature for fan activation
const int tempFanOff = 20; // Change this based on desired temperature for fan deactivation
const int maxDistance = 35; // Change this based on desired distance threshold

unsigned long lastLoopTime = millis();
unsigned long currentMillis = millis();
const unsigned long inter = 5 * 60 * 60 * 1000; // 5 hours in milliseconds
const long interval = 60 * 60 * 1000;
const unsigned long npumpInterval = 300000;
unsigned long previousMillis = 0;
unsigned long preMillis = 0;
unsigned long prevMillis = 0;
bool airpumpState = LOW; // relay is initially off

BlynkTimer timer;
WiFiClient espClient;
//BlynkSimpleLibEsp8266 blynk(BLYNK_AUTH_TOKEN, espClient);


// Water pump control based on TDS
  void checkTDS() {
    unsigned long currMillis = millis();
    if (currMillis - prevMillis >= npumpInterval) {
      prevMillis = currMillis;
      int tdsValue = analogRead(tdsPin);
      float tdsvoltage = tdsValue * (3.3 / 1023.0);  
      float tds = (133.42 * tdsvoltage * tdsvoltage * tdsvoltage - 255.86 * tdsvoltage * tdsvoltage + 857.39 * tdsvoltage) * 0.5; 
      Blynk.virtualWrite(V0, tds);
      Serial.print("TDS Value: ");
      Serial.print(tds);
      Serial.println(" ppm");

      if (tds < thresholdTDS) {
        digitalWrite(npumpPin, HIGH); // Turn on nutrients dosing pump for low TDS
        delay(5000);
        digitalWrite(npumpPin, LOW);
      } else {
        digitalWrite(npumpPin, LOW); // Turn off nutrients dosing pump for high TDS
      }
    }
  }



  // Fan control based on temperature
  void checkTemperature() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    Blynk.virtualWrite(V2, temperature);
    Serial.print("%  Temp: ");
    Serial.print(temperature);
    Serial.println("°C");

    if (temperature > tempFanOn) {
      digitalWrite(fanPin, HIGH); // Turn on fan
    } 
    else if (temperature < tempFanOff) {
      digitalWrite(fanPin, LOW); // Turn off fan
    } 
    else {
      if (millis() - lastLoopTime >= 4 * 60 * 60 * 1000) { // Check every 4 hours
        digitalWrite(fanPin, !digitalRead(fanPin)); // Toggle fan
        lastLoopTime = millis();
      }
    }
  }


  // Notification if distance exceeds threshold
  void checkDistance() {
    long duration, distance;
    digitalWrite(ultraSonicTrig, LOW);
    delayMicroseconds(2);
    digitalWrite(ultraSonicTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultraSonicTrig, LOW);
    duration = pulseIn(ultraSonicEcho, HIGH);
    distance = duration * 0.034 / 2;

    Blynk.virtualWrite(V3, distance);
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

  if (distance > maxDistance) {
    Blynk.logEvent("distance","Water level low! Distance: " + String(distance) + "cm");
  } 
  delay(5000);
}


  //Turn on submersible pump with the interval of 4 hrs
  void wpump() {
    if (currentMillis - previousMillis >= inter) {
      previousMillis = currentMillis;
      Serial.println("Turning on pump");
      digitalWrite(wpumpPin, LOW); // Replace with LOW if using a relay connected to Active-Low output
      delay(20000); // Pump run time (20 seconds)
      Serial.println("Turning off pump");
      digitalWrite(wpumpPin, HIGH); // Replace with HIGH if using a relay connected to Active-Low output
    }
    delay(500);
  }



  //Turn on air pump with the interval of 30 min
  void airpump() {
    unsigned long curMillis = millis();
    if (curMillis - preMillis >= interval) {
      preMillis = curMillis;
      airpumpState = !airpumpState;
      digitalWrite(airpumpPin, airpumpState);
      if (airpumpState) {
        Serial.println("Air pump is ON");
      } else {
        Serial.println("Air pump is OFF");
      }
    }
  }


void setup() {
  Serial.begin(9600);
  dht.begin();
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);


  pinMode(fanPin, OUTPUT);
  pinMode(npumpPin, OUTPUT);
  pinMode(wpumpPin, OUTPUT);
  pinMode(airpumpPin, OUTPUT);
  pinMode(ultraSonicTrig, OUTPUT);
  pinMode(ultraSonicEcho, INPUT);
  pinMode(tdsPin, INPUT);

  digitalWrite(npumpPin, LOW);
  digitalWrite(wpumpPin, LOW);
  digitalWrite(airpumpPin, airpumpState);
}

void loop() {
  Blynk.run();
  timer.run(); 
  checkTDS();
  checkTemperature();
  checkDistance();
  wpump();
  airpump();
}


