int sensorPin = A0;
int sensorValue;
bool bedPressure = false;

/**
 * @brief Sets the baudrate for the Serial output
 * 
 */
void setup() {
  Serial.begin(9600);
}

/**
 * @brief keeps reading the BedPressure
 * 
 */
void loop() {
  //Print sensorValue for troubleshooting
  //Serial.println(sensorValue);

  bool newBedPressure = readBedPressure();
  handleStatusChange(newBedPressure);
  delay(1000);
}

/**
 * @brief Function for reading  the value of the pressure sensor.
 * Return if some pressure is detected on the sensor.
 * 
 * @return true 
 * @return false 
 */
bool readBedPressure(){
  sensorValue = analogRead(sensorPin);
  return (sensorValue > 500);
}

/**
 * @brief Function prints to the serial that there is a pressure difference detected.
 * This will in the future be extended to send a message to the PI allowing it to take measurements.
 * 
 * @param newBedPressure 
 */
void handleStatusChange(bool newBedPressure){
  if (newBedPressure != bedPressure){
    bedPressure = newBedPressure;

    // Printing the change of status
    if (bedPressure){
      Serial.println("Er is druk op het bed gedetecteerd");
    } else {
      Serial.println("Er is geen druk meer op het bed");
    }
  }
}
