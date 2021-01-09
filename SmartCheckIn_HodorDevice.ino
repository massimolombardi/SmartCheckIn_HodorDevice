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
 *   1.1	   M. Lombardi  09/01/2021 Introdotta gestione dei Tasks con Scheduler
 * 
 */
#include <EasyButton.h>
#include <TaskScheduler.h>
#include <ESP32httpUpdate.h>

#include "RestClient.h"
#include "ConnectionManager.h"
#include "src/Configuration.h"
#include "src/FileSystem/FileHandler.h"

#define FIRMWARE_VERSION "0.0.3"

#define ACTION_BUTTON_PIN 15
#define WIFI_RESET_PRESS_DURATION 5
#define FACTORY_RESET_PRESS_DURATION 15


/*********************************** Firme delle Funzioni ***********************************/

//Tasks
void restClient();
void printStatus();
void buttonReader();
void firmwareUpdater();
void checkConnectionStatus();

//Callback Pulsante
void factoryReset();
void resetWiFiConfiguration();



/*********************************** Variabili di Supporto ***********************************/

unsigned long startedAt_ms = 0;

//Bottone per azioni utente
EasyButton button(ACTION_BUTTON_PIN);

//Oggetto per la gestione dei parametri di configurazione
Configuration cfg;

//Oggetto per la gestione della connessione
ConnectionManager cm = ConnectionManager(&cfg);

//Scheduler e Tasks
Scheduler scheduler;
Task printStatusTask(5000, TASK_FOREVER, &printStatus);
Task buttonReaderTask(0, TASK_FOREVER, &buttonReader);
Task checkConnectionStatusTask(1000, TASK_FOREVER, &checkConnectionStatus);


void setup() {

	//Configurazione della comunicazione su porta seriale
	Serial.begin(115200);
	while(!Serial);

	startedAt_ms = millis();

	Serial.println("Avvio del device in corso su " + String(ARDUINO_BOARD));
	Serial.println("Firmware Version: " + String(FIRMWARE_VERSION));
	Serial.println("ESP_AsyncWiFiManager Version: " + String(ESP_WIFIMANAGER_VERSION));

	Serial.setDebugOutput(false);

	//Inizializzazione del bottone ed impostazione Callbacks
	button.begin();
	button.onPressedFor(WIFI_RESET_PRESS_DURATION, resetWiFiConfiguration);
	//button.onPressedFor(FACTORY_RESET_PRESS_DURATION, factoryReset);

	//Inizializzazione dei Tasks
	scheduler.addTask(printStatusTask);
	scheduler.addTask(buttonReaderTask);
	scheduler.addTask(checkConnectionStatusTask);

	//Avvio dei Tasks
	printStatusTask.enable();
	buttonReaderTask.enable();
	checkConnectionStatusTask.enable();
}


void loop() {
  scheduler.execute();
}


void resetWiFiConfiguration() {
    Serial.println("Ricevuto comando di reset parametri di rete");
    cm.disconnect();
    cfg.resetWiFiCredential();
    ESP.restart();
}


void factoryReset() {
  Serial.println("Ricevuto comando di factory reset");
  cm.disconnect();
  cfg.factoryReset();
}


void checkConnectionStatus() {
	if(!cm.isConnectionActive()) {
		Serial.println("Tentativo di ristabilire la connessione");
		//Tentativo di connessione
		if(cm.MakeConnection() == WIFI_DISCONNECTED) {
		//Se non riesco a connettermi apro l'AP config
		cm.startConfigAP();
		}
	}
}


void buttonReader() {
	button.read();
}


void printStatus() {
	cm.dumpConnectionStatus();
}


void restClient() {
	if(cm.isConnectionActive()) {
		RestClient rc;
		rc.testAPI();
	}
}


void firmwareUpdater() {

	if(cm.isConnectionActive()) {

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
