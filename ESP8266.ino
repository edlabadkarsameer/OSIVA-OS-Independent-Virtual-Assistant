#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// Replace these with your network credentials
#define WIFI_SSID "your-SSID"
#define WIFI_PASSWORD "your-PASSWORD"

// Firebase project API key and database URL
#define FIREBASE_HOST "smart-home-automation-14fd3-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "EVLx1Y7mV28z0plGUZadP1Hdk7Lcmfo5DB1AJcit"

// Define the GPIO pins for the devices
#define FAN_PIN D1
#define AC_PIN D2


FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi. IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  // Set GPIO pins as outputs
  pinMode(FAN_PIN, OUTPUT);
  pinMode(AC_PIN, OUTPUT);
 

  // Attach the Firebase stream callback
  if (Firebase.beginStream(firebaseData, "/buttons")) {
    Firebase.setStreamCallback(firebaseData, streamCallback, streamTimeoutCallback);
  } else {
    Serial.println("Could not begin stream");
    Serial.println("REASON: " + firebaseData.errorReason());
  }
}

void loop() {
  // Your main code loop (not needed in this example since we use Firebase stream)
}

void streamCallback(StreamData data) {
  if (data.dataType() == "json") {
    String path = data.dataPath();
    bool state = data.boolData();

    if (path == "/button1") {
      digitalWrite(FAN_PIN, state);
    } else if (path == "/button2") {
      digitalWrite(AC_PIN, state);
    } 

    Serial.print("Path: ");
    Serial.print(path);
    Serial.print(" - State: ");
    Serial.println(state);
  }
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("Stream timeout, resuming...");
  }
}
