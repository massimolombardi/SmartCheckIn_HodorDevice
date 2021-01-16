/*
 * @Author: Massimo Lombardi
 * @Since: 07/11/2020
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Implementazione della classe per la gestione della connessione WiFi utilizzando ESPAsync_WiFiManager
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   1.1       M. Lombardi  09/01/2021 Migliorata gestione dell'AP di configurazione
 *                                     Migliorata gestione della memorizzazione credenziali
 */

#include <WiFi.h>
#include "ConnectionManager.h"


ConnectionManager::ConnectionManager(Configuration* configuration) {
	cfg = configuration;
	pinMode(CONFIGURATION_LED, OUTPUT);  
	webServer = new AsyncWebServer(HTTP_CONFIGURATION_PORT);
}


WIFI_CONNECTION_STATUS ConnectionManager::MakeConnection() {

	//if(!loadConnectionParams()) {
	//	Serial.println("Impossibile connettersi con credenziali non valide");  
	//	return WIFI_DISCONNECTED;
	//}
	//Caricamento della configurazione all'interno del WiFi Handler
	wifiHandler.addAP(cfg->getWiFiSSID(), cfg->getWiFiPassword());

	WiFi.mode(WIFI_STA);
	uint8_t connectionStatus = WL_DISCONNECTED;

	delay(WIFI_WAIT_BEFORE_1ST_CONNECT_MS);

	//Effettua fino a WIFI_MAX_CONNECTION_RETRY tentativi di connessione prima di fermarsi
	for(int i=0; (i<WIFI_MAX_CONNECTION_RETRY) && (connectionStatus != WL_CONNECTED); i++) {

		Serial.println("Tentativo di connessione " + String(i+1) + " con " + WiFi.SSID() + " in corso...");

		connectionStatus = wifiHandler.run();

		if(connectionStatus == WL_CONNECTED) {      
			Serial.println("Connessione Stabilita con " + WiFi.SSID());    
			return WIFI_CONNECTED;
		}      
		else {
			Serial.println("Tentativo di connessione " + String(i+1) + " fallito");
			delay(WIFI_WAIT_BETWEEN_CONNECT_MS);
		}
	}

	Serial.println("Impossibile connettersi");  
	return WIFI_DISCONNECTED;
}


void ConnectionManager::startConfigAP() {
  
	Serial.println("Avvio del portale di configurazione");

	ESPAsync_WiFiManager wifiManager(webServer, &dnsServer, DEVICE_NAME);

	//Soglia di qualità delle reti e selezione automatica del canale dell'AP
	wifiManager.setMinimumSignalQuality(-1);
	wifiManager.setConfigPortalChannel(0);

	//Inizializzazione dei parametri aggiuntivi di configurazione
	//cfg->initForConfigAP(wifiManager);

	//Notifica apertura portale con il led accesso
	digitalWrite(CONFIGURATION_LED, HIGH);

	//Avvio dell'AP di configurazione
	if(!wifiManager.startConfigPortal(CONFIGURATION_AP_NAME, CONFIGURATION_AP_PASSWORD))
		Serial.println("Connessione all'AP di configurazione fallita");
	else {
		Serial.println("Configurazione Completata. Avvio del device con i nuovi parametri");
	}

/*
	if(cfg->saveWiFiCredential(wifiManager.getSSID(), wifiManager.getPW())) {
		Serial.println("Credenziali salvate per " + wifiManager.getSSID());
		wifiHandler.addAP(cfg->getWiFiSSID(), cfg->getWiFiPassword());
	}
	else {
		//In caso di configurazione non valida segnalo con un blink della luce
		notifyErrorWithLed(BLINKS_NUM_ON_INVALID_CREDENTIAL);
		Serial.println("SSID o Password non validi");
	}
*/

	if(cfg->save(wifiManager)) {
		Serial.println("Configurazione salvata con successo");
		wifiHandler.addAP(cfg->getWiFiSSID(), cfg->getWiFiPassword());
		Serial.println("Credenziali WiFi aggiunte per " + wifiManager.getSSID());
	}
	else {
		//In caso di configurazione non valida segnalo con un blink della luce
		notifyErrorWithLed(BLINKS_NUM_ON_INVALID_CREDENTIAL);
		Serial.println("La configurazione caricata non è valida");
	}

	//Terminata la fase di configurazione il led si spegne
	digitalWrite(CONFIGURATION_LED, LOW);
}


bool ConnectionManager::loadConnectionParams() {
  if(cfg->loadWiFiCredential()) {
    Serial.println("Credenziali caricate per " + String(cfg->getWiFiSSID()));
    wifiHandler.addAP(cfg->getWiFiSSID(), cfg->getWiFiPassword());
    return true;
  }
  else {
    Serial.println("Nessuna configurazione WiFi valida trovata");
    return false;
  }
}


void ConnectionManager::disconnect() {
    WiFi.disconnect(false, true);
    delay(3000);
}


bool ConnectionManager::isConnectionActive() {
  if(WiFi.status() == WL_CONNECTED)
    return true;
  else
    return false;
}


void ConnectionManager::dumpConnectionStatus() {
	if(this->isConnectionActive()) {
		Serial.println("Connessione Stabilita su rete: " + WiFi.SSID());  
	}
	else {
		Serial.println("Connessione non attiva");
	}  
}


void ConnectionManager::notifyErrorWithLed(int numOfBlink) {
	for(int i=0; i<numOfBlink; i++) {
		digitalWrite(CONFIGURATION_LED, LOW);
		delay(WAIT_BETWEEN_BLINKS);
		digitalWrite(CONFIGURATION_LED, HIGH);
		delay(WAIT_BETWEEN_BLINKS);
	}
}
