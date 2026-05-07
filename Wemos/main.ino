#include <ESP8266WiFi.h>
#include "BedSensor.h"
#include "LedCheck.h"
#include "DisplayMatrix.h"

/**
 * @brief Sets values for Wemos to connect to the network, server and its own ID.
 * 
 */
const char* ssid = "NSELab";
const char* password = "NSELabWiFi";

const char* serverIP = "145.52.127.166";
const int serverPort = 5000;

const char DEVICE_ID = 'A';

WiFiClient client;
BedSensor bed(A0);
LedCheck led;

/**
 * @brief Connects to the network and informes the PI that an connection has been made
 * Starts the led and display function
 * 
 */
void setup() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  client.connect(serverIP, serverPort);
  client.println(DEVICE_ID);

  led.begin();
  displayInit();
}

/**
 * @brief Keeps a connection to the PI
 * 
 * 
 */
void loop() {

  if (!client.connected()) {
    client.connect(serverIP, serverPort);
    client.println(DEVICE_ID);
    delay(2000);
    return;
  }

/**
 * @brief Wemos receives messages from the PI and turns them to cmd text that other functions trigger
 * 
 */
  while (client.available()) {
    char cmd = client.read();

    led.handleCommand(cmd);
    displayHandleCommand(cmd);
  }


  bool newValue = bed.read();
  static bool lastValue = false;

  if (newValue != lastValue) {
    lastValue = newValue;

    client.print("Wemos ");
    client.print(DEVICE_ID);
    client.println(" is verbonden.");
    client.print(";");

    if (newValue) {
      client.println("BED ON");
    } else {
      client.println("BED OFF");
    }
  }


  led.update();
  displayUpdate(); 
  delay(200);
}
