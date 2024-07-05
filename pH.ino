#define BLYNK_TEMPLATE_ID "TMPL3AwTXlPyC"
#define BLYNK_TEMPLATE_NAME "Hydrponics System"
#define BLYNK_AUTH_TOKEN "AJXwBqUNwdA9J7rUFt4dUWW1VVmDgQGf"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <time.h>
//#include <ThingSpeak.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
BlynkTimer timer;
WiFiClient espClient;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800); // Indian timezone, adjust if necessary

ESP8266WebServer server(80);

// WiFi credentials
char ssid[] = "MAKAUT_WB";
char pass[] = "Cisco@123";

const int phPin = A0;   // Analog pin connected to the pH sensor
const int apumpPin = D4;   // Digital pin connected to the first pump
const int bpumpPin = D3;    // Digital pin connected to the second pump
const int lightPin = D0;
const int phLowThreshold = 6.5;
const int phHighThreshold = 7.5;
const unsigned long pumpInterval = 300000;
unsigned long previousMillis = 0;


/*unsigned long myChannelNumber = 2474433;
const char *myWriteAPIKey = "078XXR2AB8NEAVUD";
const char *serve = "api.thingspeak.com";*/


void automaticlight() {
  server.handleClient();

  timeClient.update(); // Update time

  // Get current time
  String formattedTime = timeClient.getFormattedTime();

  // Display current time on serial monitor
  Serial.print("Current time: ");
  Serial.println(formattedTime);

  // Automatic light control
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();


  if (currentHour == 18 && currentMinute == 0) {
    digitalWrite(lightPin, HIGH);
    Serial.println("Light turned ON");
  }

  if (currentHour == 7 && currentMinute == 0) {
    digitalWrite(lightPin, LOW);
    Serial.println("Light turned OFF");
  }

  delay(1000);
}

// Handle root URL
void handleRoot() {
  server.send(200, "text/plain", "Hello from ESP8266!");
}

// Handle not found URL
void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}


// Function to turn on the light
void turnOnLight() {
  digitalWrite(lightPin, HIGH);
  Serial.println("Light turned ON");
}

// Function to turn off the light
void turnOffLight() {
  digitalWrite(lightPin, LOW);
  Serial.println("Light turned OFF");
}


// Blynk virtual pin handler to control the light manually
BLYNK_WRITE(V4) {
  int value = param.asInt();
  if (value == 1) {
    turnOnLight();
  } else {
    turnOffLight();
  }
}


void checkpH() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= pumpInterval) {
    previousMillis = currentMillis;
    int sensorValue = analogRead(phPin);
    float voltage = sensorValue * (3.3 / 1023.0);  // Convert the analog reading to voltage

    // Convert voltage to pH value based on the calibration
    float slope = (9-4) / (2.5-1.5);
    float intercept = 4 - slope * 1.5;
    float ph = slope * voltage + intercept;
    Blynk.virtualWrite(V1, ph);
    //ThingSpeak.writeField(myChannelNumber, 2, ph, myWriteAPIKey);
    Serial.print("pH: ");
    Serial.println(ph);
    delay(2000);

    if (ph < phLowThreshold) {
      digitalWrite(bpumpPin, HIGH);   // Turn on the basic pump for low pH
      delay(5000);
      digitalWrite(bpumpPin, LOW);
      digitalWrite(apumpPin, LOW);   // Turn off acidic pump for high pH
    } 
    else if (ph > phHighThreshold) {
      digitalWrite(apumpPin, HIGH);   // Turn on the acidic pump for low pH
      delay(5000);
      digitalWrite(apumpPin, LOW);
      digitalWrite(bpumpPin, LOW);   // Turn off basic pump for high pH
    } 
    else {
      digitalWrite(apumpPin, LOW);
      digitalWrite(bpumpPin, LOW);
    }
  }
}


void setup() {
  Serial.begin(9600);
    // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timeClient.begin();
  //timeClient.updateInterval(60000); // Update time every minute
  timer.setInterval(60000, automaticlight);
  // Setup HTTP server routes
  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();

  pinMode(phPin, INPUT);
  pinMode(apumpPin, OUTPUT);
  pinMode(bpumpPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  digitalWrite(apumpPin, LOW);
  digitalWrite(bpumpPin, LOW);
}


void loop() {
  Blynk.run();
  timer.run();
  checkpH();
  automaticlight();
}