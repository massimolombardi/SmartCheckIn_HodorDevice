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
#include "RestClient.h"
#include <ESP32httpUpdate.h>

#define FIRMWARE_VERSION "0.0.2"

unsigned long startedAt_ms = 0;
ConnectionManager cm = ConnectionManager();

// Duration.
int duration = 2000;

// Button.
EasyButton button(12);

// Callback.
void onPressedForDuration() {
    Serial.println("Button has been pressed for the given duration!");
    FileHandler::deleteFile("/wifi_cred.dat");
    ESP.restart();
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

  Serial.println(FIRMWARE_VERSION);
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

    if((int)(millis() - startedAt_ms)/1000 % 4 == 0)
      cm.dumpConnectionStatus();

    if((int)(millis() - startedAt_ms)/1000 % 2 == 0) {
      RestClient rc;
      rc.testAPI();
    }

    if((int)(millis() - startedAt_ms)/1000 % 10 == 0) {

        t_httpUpdate_return ret = ESPhttpUpdate.update("http://192.168.1.18/firmware.bin");

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                break;

            case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                break;

            case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                break;

            default:
                Serial.println("Test");
                break;
        }
    }    
  }
}
