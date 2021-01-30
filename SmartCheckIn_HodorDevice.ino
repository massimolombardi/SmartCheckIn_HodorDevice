/**
 * @author: Massimo Lombardi
 * @since: 07/11/2020
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Software per la gestione della scheda di apertura porta su comando presente su risorsa REST.
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Introdotta gestione connessioni con ESPAsync_WiFiManager
 *   1.1	   M. Lombardi  09/01/2021 Introdotta gestione dei Tasks con Scheduler
 *   1.2	   M. Lombardi  10/01/2021 Introdotta gestione azioni multiple pulsante
 *   1.3	   M. Lombardi  16/01/2021 Itrodotta gestione parametri configurazione aggiuntivi
 * 
 */

#include <TaskScheduler.h>
#include <ESP32httpUpdate.h>

#include "RestClient.h"
#include "ConnectionManager.h"
#include "src/Configuration.h"
#include "src/FileSystem/FileHandler.h"

#define FIRMWARE_VERSION "0.0.4"

#define ACTION_BUTTON_PIN 15
#define REBOOT_PRESS_DURATION_MS 50
#define LOGIN_RESET_PRESS_DURATION_MS 5000
#define WIFI_RESET_PRESS_DURATION_MS 10000
#define FACTORY_RESET_PRESS_DURATION_MS 20000


/*********************************** Firme delle Funzioni ***********************************/

//Tasks
void restClientHandler();
void printStatus();
void buttonHandler();
void firmwareUpdateHandler();
void checkConnectionStatus();

//Callbacks Pulsante
void factoryReset();
void resetWiFiConfiguration();
void resetLoginConfiguration();


/*********************************** Variabili di Supporto ***********************************/

unsigned long startedAt_ms = 0;

//Oggetto per la gestione dei parametri di configurazione
Configuration cfg;

//Oggetto per la gestione della connessione
ConnectionManager cm = ConnectionManager(&cfg);

//Oggetto per la gestione della comunicazione REST
RestClient restClient = RestClient(&cfg);

//Scheduler e Tasks
Scheduler scheduler;
Task printStatusTask(5000, TASK_FOREVER, &printStatus);
Task buttonReaderTask(0, TASK_FOREVER, &buttonHandler);
Task checkConnectionStatusTask(1000, TASK_FOREVER, &checkConnectionStatus);
Task restClientTask(5000, TASK_FOREVER, &restClientHandler);

void setup() {

	//Configurazione della comunicazione su porta seriale
	Serial.begin(115200);
	while(!Serial);

	startedAt_ms = millis();

	Serial.println("Avvio del device in corso su " + String(ARDUINO_BOARD));
	Serial.println("Firmware Version: " + String(FIRMWARE_VERSION));
	Serial.println("ESP_AsyncWiFiManager Version: " + String(ESP_WIFIMANAGER_VERSION));

	Serial.setDebugOutput(false);

	//Inizializzazione del bottone di azioni utente
	pinMode(ACTION_BUTTON_PIN, INPUT_PULLUP); 

	//Caricamento della configurazione. In caso di errore faccio partire l'AP ricorsivamente
	while(!cfg.initialize()) 
		cm.startConfigAP();

	//Inizializzazione dei Tasks
	scheduler.addTask(printStatusTask);
	scheduler.addTask(buttonReaderTask);
	scheduler.addTask(checkConnectionStatusTask);
	scheduler.addTask(restClientTask);

	//Avvio dei Tasks
	printStatusTask.enable();
	buttonReaderTask.enable();
	checkConnectionStatusTask.enable();
	restClientTask.enable();
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


void resetLoginConfiguration() {
    Serial.println("Ricevuto comando di reset parametri di Login");
    cfg.resetLoginCredential();
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
		//#TODO - forse inserire una tolleranza, altrimenti un drop di connessione temporaneo incastra il device
		cm.startConfigAP();
		}
	}
}


void buttonHandler() {

	static int duration = 0;

	while(digitalRead(ACTION_BUTTON_PIN) == LOW) {
		delay(50);
		duration += 50;
	}
	
	if(duration >= FACTORY_RESET_PRESS_DURATION_MS) {  
		factoryReset();
		duration = 0;
  	}
	else if(duration >= WIFI_RESET_PRESS_DURATION_MS) {
		resetWiFiConfiguration();
		duration = 0;
	}
	else if(duration >= LOGIN_RESET_PRESS_DURATION_MS) {
		resetLoginConfiguration();
		duration = 0;
	}
	else if(duration >= REBOOT_PRESS_DURATION_MS) {
		ESP.restart();
		duration = 0;
	}
	else {
		duration = 0;
	}
}


void printStatus() {
	cm.dumpConnectionStatus();
}


void restClientHandler() {
	if(cm.isConnectionActive()) {
		restClient.login();
		restClient.status();
	}
}


void firmwareUpdateHandler() {

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
