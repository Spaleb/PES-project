#include <ESP8266WiFi.h>
#include "BedSensor.h"
#include "LedCheck.h"
#include "DisplayMatrix.h"
#include "RFID.h"
#include "LedStrip.h"
#include "MoistureSensor.h"
#include "SHT3x.h"


const char* ssid = "PESC";
const char* password = "useruser";

const char* serverIP = "10.42.0.92";
const int serverPort = 5000;
char DEVICE_ID = 'A'; //A; BED, RFID | B; SHT3X, MATRIX | C; LEDStrip, MOIS

WiFiClient client;
BedSensor bed(A0);
LedCheck ledCheck;
RFID rfid;
LedStrip ledstrip;
MoistureSensor msensor;
SHT3xSensor sht3x;
DisplayMatrix display;


/**
 * @brief
 * Deze functie is de setup-functie;
 * Het initialiseert de WiFi-verbinding, maakt verbinding met de server en initialiseert de sensoren en apparaten op basis van het DEVICE_ID.
 */
void setup() {
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  client.connect(serverIP, serverPort);
  client.setTimeout(10);
  client.println(DEVICE_ID);
  client.print('\n');


 //ID A;
  rfid.begin();

  //ID B;
  //sht3x.begin();
  //display.init();

  //ID C;
  //ledstrip.begin();
  

}
/**
 * @brief 
 * Deze functie is de loop-functie;
 * Het controleert of de client nog steeds verbonden is met de server en probeert opnieuw verbinding te maken als dat niet het geval is. 
 * Vervolgens leest het inkomende berichten van de server, verwerkt deze en stuurt de juiste commando's naar de sensoren en apparaten op basis van het bericht.
 * 
 */
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
  bed.update();
  rfid.update();
 
  //B;
  //display.update();
  //sht3x.loop();

  //C;
  //msensor.loop();

  delay(50);
}
