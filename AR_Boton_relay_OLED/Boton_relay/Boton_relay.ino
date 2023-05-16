#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <boton_test.h>
#include <string.h>
//libraries for the FireBase conection and wifi
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

/* 1. Define the wifi password and name */
#define WIFI_SSID "TecJalisco Docentes"
#define WIFI_PASSWORD "RUAP0379"

/* 2. Define the API Key */
#define API_KEY "AIzaSyB992N3FJa-ThwXgpF8LN0-TJVQ0bv0yR0"

/* 3. Define the RTDB URL */
#define DATABASE_URL "ar-for-iot-default-rtdb.firebaseio.com"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "za230120005@zapopan.tecmm.edu.mx"
#define USER_PASSWORD "diegodlrv123"
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid;
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;
String databasePath;
String relayPath = "relay_1";  //define the relays path on the database
String parentPath;


void setup() {

  Serial.begin(9600);
  // Here, button is pin12, relay1 is pin2 and also internal led.
  pinMode(GPIO_ID_PIN(12), INPUT_PULLUP);
  pinMode(GPIO_ID_PIN(2), OUTPUT);
  digitalWrite(GPIO_ID_PIN(2), HIGH);  //led apagado
  pinMode(GPIO_ID_PIN(0), OUTPUT);
  digitalWrite(0, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;  // Don't proceed, loop forever
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Iniciando");
  display.display();
  delay(1000);

  // defines all about the database and the wifi//
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
  databasePath = "/UsersData/" + uid + "/relay";
}


void loop() {
  
  display.clearDisplay();
  //SE AGREGA LA CONEXION DEL WIFI Y FBDTB
  FirebaseJson json;
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    parentPath = databasePath + "/";

    sendDataPrevMillis = millis();
    json.set(relayPath.c_str(), String(relay1()));
    Serial.printf("Set json... %s\n", Firebase.RTDB.set(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
  }


  Serial.println(String(relay1()));
}


void initWifi() {

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
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
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h


#if defined(ESP8266)
    // In ESP8266 required for BearSSL rx/tx buffer for large data handle, increase Rx size as needed.
  fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
#endif

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);
}

int relay1() {

  union boton boton_selec;
  union boton *val = &boton_selec;
  static bool laststate_button;
  int output_relay;
  volatile int *output = &output_relay;
  boton_selec.boton1 = digitalRead(12);

  if (val->boton1 == HIGH && laststate_button == LOW) {
    //digitalWrite(2, HIGH);
    //APAGA RELAY
    //digitalWrite(0, LOW);
    //Serial.println(val->boton1);
    output_relay = 0;
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);  //no hay necesidad que posicionar el cursor
    display.print("OFF");
    display.display();
    delay(300);
    //return *output;
  }

  if (val->boton1 == HIGH && laststate_button == HIGH) {
    //digitalWrite(2, HIGH);
    //APAGA RELAY
    //digitalWrite(0, LOW);
    //Serial.println(val->boton1);
    display.setTextSize(1);
    output_relay = 1;
    display.setTextColor(WHITE);
    display.setCursor(0, 10);  //no hay necesidad que posicionar el cursor
    display.print("ON");
    display.display();
    delay(300);
    //return *output;
  }


  if (val->boton1 == LOW && laststate_button == LOW) {

    digitalWrite(2, LOW);
    //ENCIENDE RELAY
    digitalWrite(0, HIGH);
    output_relay = 1;
    //Serial.println(val->boton1);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);  // no hay necesidad que posicionar el cursor
    display.print("ON");
    display.display();
    laststate_button = HIGH;
    delay(300);
    // return *output;
  }

  else if (val->boton1 == LOW && laststate_button == HIGH) {

    digitalWrite(2, HIGH);
    //ENCIENDE RELAY
    digitalWrite(0, LOW);
    //Serial.println(val->boton1);
    output_relay = 0;
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);  // no hay necesidad que posicionar el cursor
    display.print("OFF");
    display.display();
    laststate_button = LOW;
    delay(300);
    //return *output;
  }

  return output_relay;
}