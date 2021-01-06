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
 *
 */

#include <WiFi.h>
#include "Configuration.h"
#include "WiFiCredential.h"
#include "ConnectionManager.h"

/*
 * Costruttore
 */
ConnectionManager::ConnectionManager() {
  pinMode(CONFIGURATION_LED, OUTPUT);  
  webServer = new AsyncWebServer(HTTP_CONFIGURATION_PORT);
}


/*
 * Funzione per la gestione delle connessioni
 */
WIFI_CONNECTION_STATUS ConnectionManager::MakeConnection() {

  loadConnectionParams();

  WiFi.mode(WIFI_STA);
  uint8_t connectionStatus = WL_DISCONNECTED;
  
  delay(WIFI_MULTI_1ST_CONNECT_WAITING_MS);

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
      delay(WIFI_MULTI_CONNECT_WAITING_MS);
    }
  }
  Serial.println("Impossibile connettersi");  
  return WIFI_DISCONNECTED;
}


/*
 * Funzione per l'avvio dell'AP di configurazione
 */
void ConnectionManager::startConfigAP() {
  
  Serial.println("Avvio del portale di configurazione");

  ESPAsync_WiFiManager wifiManager(webServer, &dnsServer, "ConfigOnSwitch");

  //Soglia di qualità delle reti e selezione automatica del canale dell'AP
  wifiManager.setMinimumSignalQuality(-1);
  wifiManager.setConfigPortalChannel(0);

  //Lettura delle precedenti configurazioni
  String storedSSID = wifiManager.WiFi_SSID();
  String storedPsw = wifiManager.WiFi_Pass();
  
  if((storedSSID != "") && (storedPsw != "")) {
    wifiManager.setConfigPortalTimeout(120);
    Serial.println("Got stored Credentials. Timeout 120s");
  }
  else
    Serial.println("No stored Credentials. No timeout");

  //Inizializzazione dei parametri aggiuntivi di configurazione
  Configuration cfg;
  cfg.initForConfigAP(wifiManager);

  //Notifica apertura portale con il led accesso
  digitalWrite(CONFIGURATION_LED, HIGH);
  
  //Avvio dell'AP di configurazione
  if(!wifiManager.startConfigPortal(CONFIGURATION_AP_NAME, CONFIGURATION_AP_PASSWORD))
    Serial.println("Connessione fallita");
  else {
    Serial.println("Configurazione Completata");
  }

  if(String(wifiManager.getSSID(0)) != "") {

    //Reset delle precedenti impostazioni
    memset(&credenziali, 0, sizeof(credenziali));
    
    if(credenziali.storeCredential(wifiManager.getSSID(), wifiManager.getPW())) {
      Serial.println("Credenziali salvate per " + wifiManager.getSSID());
      wifiHandler.addAP(credenziali.getSSID(), credenziali.getPassword());
      credenziali.saveToFile();
    }
    else {
      Serial.println("SSID o Password non validi.");
    }
  }
  else {
    Serial.println("Credenziali ricevute vuote");
  }

  //Terminata la fase di configurazione il led si spegne
  digitalWrite(CONFIGURATION_LED, LOW);
}


/*
 * Funzione per il controllo dello stato di connessione
 * Ritorna TRUE se la connessione è attiva, FALSE altrimenti
 */
bool ConnectionManager::isConnectionActive() {
  if(WiFi.status() == WL_CONNECTED)
    return true;
  else
    return false;
}


/*
 * Funzione di riepilogo stato della connessione
 */
void ConnectionManager::dumpConnectionStatus() {
  
  if(this->isConnectionActive()) {
    Serial.println("Connessione Stabilita su rete: " + WiFi.SSID());  
  }
  else {
    Serial.println("Connessione non attiva");
  }  
}


void ConnectionManager::loadConnectionParams() {

  credenziali.loadFromFile();
  
  if(credenziali.isValidCredential()) {
    Serial.println("Credenziali caricate per " + credenziali.getStringSSID());
    wifiHandler.addAP(credenziali.getSSID(), credenziali.getPassword());
  }
  else {
    Serial.println("SSID o Password non validi");
  }
}
