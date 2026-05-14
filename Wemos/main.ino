#include <ESP8266WiFi.h>
#include "BedSensor.h"
#include "LedCheck.h"
#include "DisplayMatrix.h"

const char* ssid = "NSELab";
const char* password = "NSELabWiFi";

const char* serverIP = "145.52.127.166";
const int serverPort = 5000;

const char DEVICE_ID = 'A';

WiFiClient client;

BedSensor bed(A0);
LedCheck led;

void setup() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  client.connect(serverIP, serverPort);
  client.println(DEVICE_ID);
  led.begin();
  displayInit();
}

void loop() {

  // reconnect indien nodig
  if (!client.connected()) {
    client.stop();

    if (client.connect(serverIP, serverPort)) {
      client.println(DEVICE_ID);
    }

    delay(1000);

    return;
  }

  // DATA VAN SERVER
  if (client.available()) {

    String msg = client.readStringUntil('\n');
    msg.trim();
    if (msg.length() > 0) {
      if (msg == "1" || msg == "2") {
        led.handleCommand(msg[0]);
      }
      else {
        displayShow(msg.c_str());
      }
    }
  }

  led.update();
  displayUpdate();
  bed.update(client);
  

  delay(50);
}
