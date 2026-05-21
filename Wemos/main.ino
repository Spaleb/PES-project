#include <ESP8266WiFi.h>
#include "BedSensor.h"
#include "LedCheck.h"
#include "DisplayMatrix.h"
#include "RFID.h"

const char* ssid = "happyvibeswifi";
const char* password = "zoetoefeestschuur";

const char* serverIP = "192.168.0.217";
const int serverPort = 5000;

char DEVICE_ID = 'A';

WiFiClient client;
BedSensor bed(A0);
LedCheck led;
RFID rfid;

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
  led.begin();
  displayInit();
  rfid.begin();

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
          led.handleCommand(msg[0]);
        } else {
          displayShow(msg.c_str());
        }
      }

      msg = "";
    } else {
      msg += c;
    }
  }


  led.update();
  displayUpdate();
  bed.update();
  rfid.update();

  delay(50);
}
