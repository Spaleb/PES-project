#include <ESP8266WiFi.h>
#include "BedSensor.h"
#include "LedCheck.h"
#include "DisplayMatrix.h"
#include "RFID.h"
#include "LedStrip.h"
#include "MoistureSensor.h"
#include "SHT3x.h"


const char* ssid = "NSELab";
const char* password = "NSELabWiFi";

const char* serverIP = "145.52.127.166";
const int serverPort = 5000;

char DEVICE_ID = 'B'; //A; BED, RFID | B; SHT3X, MATRIX | C; LEDStrip, MOIS

WiFiClient client;
BedSensor bed(A0);
LedCheck ledCheck;
RFID rfid;
LedStrip ledstrip;
MoistureSensor msensor;
SHT3xSensor sht3x;
DisplayMatrix display;



void setup() {

  Serial.begin(115200);
  delay(500);
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  client.connect(serverIP, serverPort);
  client.setTimeout(10);
  client.println(DEVICE_ID);
  client.print('\n');

 //ID A;
   //rfid.begin();

  //ID B;
    sht3x.begin();
    display.init();

  //ID C;
  //ledstrip.begin();
  

}

void loop() {

  
  if (!client.connected()) {
    client.stop();

    client.connect(serverIP, serverPort);
    client.println(DEVICE_ID);
    client.print('\n');
  }


  static String msg = "";

  while (client.available()) {
    char c = client.read();

    if (c == '\n') {
      msg.trim();

      if (msg.length() > 0) {

        if (msg == "LEDon" || msg == "LEDoff" || msg == "red" || msg == "green" || msg == "blue" || msg == "LEDBRANDON" || msg == "LEDBRANDOFF") {
          ledstrip.handleCommand(msg, client);
        }
        else if (msg == "VENTon" || msg == "VENToff") {
          sht3x.handleCommand(msg, client);
        }
        else {
          display.handleCommand(msg, client);
        }
      }

      msg = "";
    } else {
      msg += c;
    }
  }

  //A;
  //bed.update();
  //rfid.update();
 
  //B;
  display.update();
  sht3x.loop();

  //C;
  //msensor.loop();

  delay(50);
}
