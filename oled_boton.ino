#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(9600);
  pinMode(GPIO_ID_PIN(12), INPUT_PULLUP);
  pinMode(GPIO_ID_PIN(2), OUTPUT);
  digitalWrite(GPIO_ID_PIN(2), HIGH);//led apagado

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println("SSD1306 allocation failed");
  for(;;); // Don't proceed, loop forever
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
  
  
}
void loop() 
{
  
  int sensorval = digitalRead(12);
  volatile int* val = &sensorval;

      if (*val == HIGH)
      {
        display.clearDisplay();
        display.display();
        digitalWrite(2, LOW);
        Serial.println(*val);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        //display.setCursor(75, 10); no hay necesidad que posicionar el cursor
        display.print("Boton no presionado");
        display.display();
        delay(10);
      }
      else{
        display.clearDisplay();
        display.display();
        digitalWrite(2, HIGH);
        Serial.println(*val);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        //display.setCursor(75, 10); no hay necesidad que posicionar el cursor
        display.print("Boton presionado");
        display.display();
        delay(10);
        
      }

}


void texto()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("hola mundo");

}
