#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();

int samplerate = 100;

void getx()
{
  mma.read();

  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);
  Serial.print("\t X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  char accelx_char[20];
  itoa((event.acceleration.x * 100), accelx_char, 10);
  appendFile(SD, "/acceldata.txt", accelx_char);
  Serial.print(accelx_char);
}

void gety()
{
  mma.read();

  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);
  Serial.print("\t Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  char accely_char[20];
  itoa((event.acceleration.y * 100), accely_char, 10);
  appendFile(SD, "/acceldata.txt", accely_char);
  Serial.print(accely_char);
}

void getz()
{
  mma.read();

  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);
  Serial.print("\t Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  char accelz_char[20];
  itoa((event.acceleration.z * 100), accelz_char, 10);
  appendFile(SD, "/acceldata.txt", accelz_char);
  Serial.print(accelz_char);
}

void getbrake()
{
  if (analogRead(A1) < 10)
{
  appendFile(SD, "/acceldata.txt","Off");
  Serial.print("\t Brake Off");
}
else
{
  appendFile(SD, "/acceldata.txt","On");
  Serial.print("\t Brake On"); 
}
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
//    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
//        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void setup(){
    Serial.begin(115200);
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

//    Serial.print("SD Card Type: ");
//    if(cardType == CARD_MMC){
//        Serial.println("MMC");
//    } else if(cardType == CARD_SD){
//        Serial.println("SDSC");
//    } else if(cardType == CARD_SDHC){
//        Serial.println("SDHC");
//    } else {
//        Serial.println("UNKNOWN");
//    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
//    Serial.printf("SD Card Size: %lluMB\n", cardSize);
      
    writeFile(SD, "/acceldata.txt", "(X,Y,Z (m/s^2)); Brake");
    appendFile(SD, "/acceldata.txt", "\r\n");

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_4_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");

}

//int num = 0;
//char numChar[20];
void loop(){
  delay(samplerate);
  
  getx();   //x acceleration in mm/s^2
  appendFile(SD, "/acceldata.txt","\t");
  
  gety();   //y accleration in mm/s^2
  appendFile(SD, "/acceldata.txt","\t");
  
  getz();   //z accelration in mm/s^2
  appendFile(SD, "/acceldata.txt","\t");
  
  getbrake();   //brake status
  appendFile(SD, "/acceldata.txt","\t");

  appendFile(SD, "/acceldata.txt","\r\n");  //ends line
  Serial.println("\t");
} 
