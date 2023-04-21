
#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Totalplay-BAAA"
#define WIFI_PASSWORD "BAAA02A9uCnEZeZ9"

/* 2. Define the API Key */
#define API_KEY "AIzaSyB992N3FJa-ThwXgpF8LN0-TJVQ0bv0yR0"

/* 3. Define the RTDB URL */
#define DATABASE_URL "ar-for-iot-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "za230120005@zapopan.tecmm.edu.mx"
#define USER_PASSWORD "diegodlrv123"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

String uid;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

String databasePath;

String tempPath = "temperatura";

String parentPath;


void setup() 
{

    Serial.begin(115200);
    //timeClient.begin();

    initWifi();
    Firebase.begin(&config, &auth);
    
    Firebase.reconnectWiFi(true);

    Firebase.setDoubleDigits(5);

    config.timeout.serverResponse = 10 * 1000;

    Serial.println("Getting User UID");
    while ((auth.token.uid) == "") {
      Serial.print('.');
      delay(1000);
    }

      // Print user UID
    uid = auth.token.uid.c_str();
    Serial.print("User UID: ");
    Serial.println(uid);

  // Update database path
    databasePath = "/UsersData/" + uid + "/lecturas";
      pinMode(GPIO_ID_PIN(12), INPUT_PULLUP);
      pinMode(GPIO_ID_PIN(2), OUTPUT);
      digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {

  int swval = digitalRead(12);
  Serial.println(temperatura());
  digitalWrite(BUILTIN_LED,HIGH);
  if (swval == LOW) 
  { 
      digitalWrite(BUILTIN_LED, LOW);
      FirebaseJson json;
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0))
        {
        parentPath= databasePath + "/";

        sendDataPrevMillis = millis();
        json.set(tempPath.c_str(), String(temperatura()));
        Serial.printf("Set json... %s\n", Firebase.RTDB.set(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());

    }
  }
  else
  {
    Serial.println(swval);
    digitalWrite(BUILTIN_LED, HIGH);
    Serial.println("datos no enviados a firebase");
  }
  
}

void initWifi()
{

 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    
  #if defined(ESP8266)
    // In ESP8266 required for BearSSL rx/tx buffer for large data handle, increase Rx size as needed.
    fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
  #endif

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);
  



}


float temperatura()
{
  int average = 0;
  for (int i=0; i < 10; i++) {
  average = average + analogRead(A0);
  delay(5);
  }
  average = average/10;   //se toman 10 muestras cada 5ms y se hace el promedio
  float milivolts = (average / 1024.0) * 3300; //3.3V = 3300 here
  float celsius = milivolts / 10;
  
  return celsius;

}