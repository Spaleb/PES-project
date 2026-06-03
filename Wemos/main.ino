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

char DEVICE_ID = 'B';

WiFiClient client;
BedSensor bed(A0);
LedCheck ledCheck;
RFID rfid;
LedStrip ledstrip;
MoistureSensor msensor;
SHT3xSensor sht3x;



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

  ledCheck.begin();
  displayInit();
  rfid.begin();
  ledstrip.begin();
    sht3x.begin();

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
        if (msg == "1" || msg == "2") {
          ledCheck.handleCommand(msg[0]);
        } 
        else if (msg == "on" || msg == "off" || msg == "red" || msg == "green" || msg == "blue") {
          ledstrip.handleCommand(msg, client);
        }  else {
          displayShow(msg.c_str());
        }
      }

      msg = "";
    } else {
      msg += c;
    }
  }


  ledCheck.update();
  displayUpdate();
  bed.update();
  rfid.update();
  msensor.loop();
  sht3x.loop();


  delay(50);
}
