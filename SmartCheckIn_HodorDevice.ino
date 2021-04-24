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
 *   1.3	   M. Lombardi  16/01/2021 Introdotta gestione parametri configurazione aggiuntivi
 *   1.4 	   M. Lombardi  20/04/2021 Introdotti alcuni miglioramenti alla gestione chiamate API
 * 									   Configurati GPIO previsti da progetto
 */

#include <TaskScheduler.h>
#include <ESP32httpUpdate.h>

#include "APIHandler.h"
#include "ConnectionManager.h"
#include "src/Configuration.h"
#include "src/Tests/TestsSuite.h"
#include "src/Device/OpenAction.h"
#include "src/Device/DeviceStatus.h"
#include "src/FileSystem/FileHandler.h"

#define FIRMWARE_VERSION "1.0.8"

//Pin GPIO per il comando al relay
#define OPEN_ACTION_PIN 2

//Pin GPIO per il collegamento dello user button
#define ACTION_BUTTON_PIN 4

//Pin GPIO per il collegamento del LED di notifica
#define CONFIGURATION_LED 16

//Tempistiche relative alle azioni utente sul pulsante
#define REBOOT_PRESS_DURATION_MS 50
#define LOGIN_RESET_PRESS_DURATION_MS 5000
#define WIFI_RESET_PRESS_DURATION_MS 10000
#define FACTORY_RESET_PRESS_DURATION_MS 20000

//Delay di mantenimento relay chiuso per impartire il comando
#define TOGGLE_RELAY_FOR_MS 500

//Numero di retry prima di decretare un problema sulla connessione WiFi
#define CONNECTION_RETRY_BEFORE_CFG_AP 5

//Delay controllo stato connessione: 1 secondo
#define CHECK_CONNECTION_STATUS_DELAY_MS 1000

//Delay stampa su seriale stato connessione: 5 secondi
#define PRINT_STATUS_DELAY_MS 5000

//Delay controllo apertura necessaria: 30 secondi
#define CHECK_FOR_OPENING_DELAY_MS 30000

//Delay invio aggiornamento status al server: 2 minuti
#define SEND_STATUS_DELAY_MS 120000

//Delay controllo aggiornamento firmware: 1 giorno
#define CHECK_UPDATE_DELAY_MS 86400000


/*********************************** Firme delle Funzioni ***********************************/

//Tasks
void checkOpenHandler();
void sendStatusHandler();
void printStatus();
void buttonHandler();
void firmwareUpdateHandler();
void checkConnectionStatus();

//Callbacks Pulsante
void factoryReset();
void resetWiFiConfiguration();
void resetLoginConfiguration();


/*********************************** Variabili di Supporto ***********************************/

//Oggetto per la gestione dei parametri di configurazione
Configuration cfg;

//Oggetto per la gestione della connessione
ConnectionManager cm = ConnectionManager(&cfg);

//Oggetto per la gestione della comunicazione REST
APIHandler apiHandler = APIHandler(&cfg);

//Oggetto per la gestione dello stato device
DeviceStatus deviceStatus = DeviceStatus(FIRMWARE_VERSION);

//Oggetto per la gestione del comando di apertura
OpenAction openAction = OpenAction();

//Scheduler e Tasks
Scheduler scheduler;
Task printStatusTask(PRINT_STATUS_DELAY_MS, TASK_FOREVER, &printStatus);
Task buttonReaderTask(0, TASK_FOREVER, &buttonHandler);
Task checkConnectionStatusTask(CHECK_CONNECTION_STATUS_DELAY_MS, TASK_FOREVER, &checkConnectionStatus);
Task checkOpenTask(CHECK_FOR_OPENING_DELAY_MS, TASK_FOREVER, &checkOpenHandler);
Task sendStatusTask(SEND_STATUS_DELAY_MS, TASK_FOREVER, &sendStatusHandler);
Task checkUpdateTask(CHECK_UPDATE_DELAY_MS, TASK_FOREVER, &firmwareUpdateHandler);


/**
* Metodo principale di setup del device
*/
void setup() {

	//Configurazione della comunicazione su porta seriale
	Serial.begin(115200);
	while(!Serial);

	Serial.println("Avvio del device in corso su " + String(ARDUINO_BOARD));
	Serial.println("Firmware Version: " + String(FIRMWARE_VERSION));
	Serial.println("ESP_AsyncWiFiManager Version: " + String(ESP_WIFIMANAGER_VERSION));

	Serial.setDebugOutput(false);

	//Inizializzazione del bottone di azioni utente
	pinMode(ACTION_BUTTON_PIN, INPUT_PULLUP); 

	//Inizializzazione del pin di apertura
	pinMode(OPEN_ACTION_PIN, OUTPUT); 

	//Caricamento della configurazione. In caso di errore faccio partire l'AP ricorsivamente
	while(!cfg.initialize()) 
		cm.startConfigAP();

	//Inizializzazione dei Tasks
	scheduler.addTask(printStatusTask);
	scheduler.addTask(buttonReaderTask);
	scheduler.addTask(checkConnectionStatusTask);
	scheduler.addTask(checkOpenTask);
	scheduler.addTask(sendStatusTask);

	//Avvio dei Tasks
	printStatusTask.enable();
	buttonReaderTask.enable();
	checkConnectionStatusTask.enable();
	checkOpenTask.enable();
	sendStatusTask.enable();

	//Controllo se è richiesto di entrare nella modalità di test
	if(digitalRead(ACTION_BUTTON_PIN) == LOW) {
		TestsSuite ts = TestsSuite(CONFIGURATION_LED, OPEN_ACTION_PIN, ACTION_BUTTON_PIN);
		ts.startTesting();
	}

	//Controllo se è necessario effettuare un aggiornamento del firmware dal server locale
	firmwareUpdateHandler();
}


/**
* Loop principale
*/
void loop() {
  scheduler.execute();
}


/**
* Funzione per la gestione del reset configurazione WiFi su comando
*/
void resetWiFiConfiguration() {
    Serial.println("Ricevuto comando di reset parametri di rete");
    cm.disconnect();
    cfg.resetWiFiCredential();
    ESP.restart();
}


/**
* Funzione per la gestione del reset configurazione credenziali API su comando
*/
void resetLoginConfiguration() {
    Serial.println("Ricevuto comando di reset parametri di Login");
    cfg.resetLoginCredential();
    ESP.restart();
}


/**
* Funzione per la gestione del factory reset su comando
*/
void factoryReset() {
  Serial.println("Ricevuto comando di factory reset");
  cm.disconnect();
  cfg.factoryReset();
}


/**
* Task per il controllo dello stato connessione
*/
void checkConnectionStatus() {

	static int connectionRetryCounter = 0;

	if(!cm.isConnectionActive()) {

		Serial.println("Tentativo di ristabilire la connessione");
		
		//Tentativo di connessione
		int status = cm.MakeConnection();

		if(status == WIFI_DISCONNECTED) {
			connectionRetryCounter++;
			if(connectionRetryCounter < CONNECTION_RETRY_BEFORE_CFG_AP) {
				//Se non riesco a connettermi per CONNECTION_RETRY_BEFORE_CFG_AP volte apro l'AP config
				cm.startConfigAP();
			}
		}
		else {
			connectionRetryCounter = 0;
		}
	}
}


/**
* Task per il controllo delle azioni eseguite sul bottone utente
*/
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


/**
* Task per la stampa dello stato connessione
*/
void printStatus() {
	cm.dumpConnectionStatus();
}


/**
* Task per l'invio dello stato al server
*/
void sendStatusHandler() {
	if(cm.isConnectionActive()) {
		apiHandler.status(deviceStatus.getOperative(), deviceStatus.getErrorInfo());
	}
}


/**
* Task per il controllo apertura necessaria
*/
void checkOpenHandler() {
	if(cm.isConnectionActive()) {
		if(apiHandler.checkOpen()) {
			openAction.open(OPEN_ACTION_PIN, TOGGLE_RELAY_FOR_MS);
			apiHandler.openConfirmation();
		}	
	}
}


/**
* Task per la gestione degli aggiornamenti firmware
*/
void firmwareUpdateHandler() {

	//Controllo la connessione ed eventualmente la stabilisco
	checkConnectionStatus();

	Serial.println("Controllo se sono disponibili aggiornamenti software sulla rete locale");

	if(cm.isConnectionActive()) {

		//TODO: Controllare se necessario aggiornare prima di effettuare l'aggiornamento

		t_httpUpdate_return ret = ESPhttpUpdate.update("http://192.168.1.16:8000/firmware.bin");

		switch(ret) {
			case HTTP_UPDATE_FAILED:
				Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
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
