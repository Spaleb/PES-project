#include "MoistureSensor.h"

/**
 * @brief 
 * Deze functie leest de analoge waarde van de vochtigheidssensor, zet deze om naar een percentage en retourneert het. De functie maakt gebruik van de map-functie om de sensorwaarde te schalen tussen 0% (droog) en 100% (nat), op basis van vooraf gedefinieerde droge en natte waarden. 
 * De resulterende waarde wordt vervolgens beperkt tot het bereik van 0% tot 100% met behulp van de constrain-functie.
 * 
 * 
 * @return int 
 */
int MoistureSensor::readMoisturePercent() {
  int sensorValue = analogRead(sensorPin);
  int moisturePercent = map(sensorValue, dryValue, wetValue, 0, 100);
  return constrain(moisturePercent, 0, 100);
}

/**
 * @brief 
 * Deze functie leest de vochtigheidssensor en stuurt een bericht naar de server als er een significante verandering in vochtigheid is gedetecteerd. De functie vergelijkt de huidige vochtigheidswaarde met de laatste gemeten waarde en stuurt alleen een bericht als het verschil groter is dan 5%. Dit helpt om onnodige berichten te voorkomen bij kleine fluctuaties in vochtigheid.
 * 
 */
void MoistureSensor::loop() {
  int moisture = readMoisturePercent();

  if ((moisture - lastMoisture) > 5 || (lastMoisture - moisture) > 5) {
    client.println("MOI:" + String(moisture));

    lastMoisture = moisture;
  }
}
