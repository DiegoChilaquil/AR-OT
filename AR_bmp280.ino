#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <math.h>


Adafruit_BMP280 bmp; // I2C



void setup() {

  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;

  status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  //status = bmp.begin(BMP280_ADDRESS_ALT);
  delay(5000);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }

  void (*confg)();
  confg = bmpconfig;
  *confg;

  delay(100);
}

void loop() {

  float *ptr1;
  *ptr1 = values();

  for (int i = 0; i<=1; i++){

    if (i == 0){
      Serial.print("Temp = ");
      Serial.println(ptr1[i]);
    }
    else{

      Serial.print("Pres = ");
      Serial.println(ptr1[i]);
    }
    
  }

  
  delay(1000);

  free(ptr1);
}


void bmpconfig(){
  
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

float values(){
  //int a[2];
  float *value;
  value  = (float*)calloc(2, sizeof(float));

  value[0] = bmp.readTemperature();
  value[1] = bmp.readPressure();

  return *value;
}