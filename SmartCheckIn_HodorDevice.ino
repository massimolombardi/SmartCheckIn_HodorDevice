/*
 * @Author: Massimo Lombardi
 * @Since: 07/11/2020
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Software per la gestione della scheda di apertura porta su comando presente su risorsa REST.
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Introdotta gestione connessioni con ESPAsync_WiFiManager
 * 
 */
#include "WiFiCredential.h"
#include "ConnectionManager.h"
#include <EasyButton.h>
#include "FileHandler.h"

unsigned long startedAt_ms = 0;
ConnectionManager cm = ConnectionManager();

// Duration.
int duration = 2000;

// Button.
EasyButton button(12);

// Callback.
void onPressedForDuration() {
    Serial.println("Button has been pressed for the given duration!");
    FileHandler::formatFilesystem();
}

void setup() {

  //Configurazione della comunicazione su porta seriale
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Avvio del device in corso su " + String(ARDUINO_BOARD));
  //Serial.println("ESP_WiFiManager Version " + String(ESP_WIFIMANAGER_VERSION));
  //Serial.println("ESP_DoubleResetDetector Version " + String(ESP_DOUBLE_RESET_DETECTOR_VERSION));

  Serial.setDebugOutput(false);

  // Initialize the button.
  button.begin();
  
  // Attach callback.
  button.onPressedFor(duration, onPressedForDuration);
  
  startedAt_ms = millis();
}


void loop() {

  
  if(!cm.isConnectionActive()) {

    //Tentativo di connessione
    if(cm.MakeConnection() == WIFI_DISCONNECTED) {    
      
      //Se non riesco a connettermi apro l'AP config
      cm.startConfigAP();
    }
  
  }
  else {
    
    button.read();

    if((millis() - startedAt_ms) % 2000 == 0)
      cm.dumpConnectionStatus();
  
  }
}
