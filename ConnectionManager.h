#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
/*
 * @Author: Massimo Lombardi
 * @Since: 07/11/2020
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Classe per la gestione della connessione WiFi utilizzando ESPAsync_WiFiManager
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 * 
 */

#include <WiFiMulti.h>
#include <ESPAsync_WiFiManager.h>              

#define ESP_WIFIMANAGER_VERSION "1.3.0"

#define WIFI_MAX_CONNECTION_RETRY 10
#define WIFI_MULTI_CONNECT_WAITING_MS 100L
#define WIFI_MULTI_1ST_CONNECT_WAITING_MS 0

#define CONFIGURATION_AP_NAME "SmartCheckIn - Config"
#define CONFIGURATION_AP_PASSWORD "smartcheckin"

#define HTTP_CONFIGURATION_PORT 80
#define CONFIGURATION_LED 4


enum WIFI_CONNECTION_STATUS {
  WIFI_ERROR = -1,
  WIFI_DISCONNECTED = 0,
  WIFI_CONNECTED = 1 
};


class ConnectionManager {

  private:
    //Oggetto per la gestione delle connessioni
    WiFiMulti wifiHandler;
  
    //Server DNS
    DNSServer dnsServer;
  
    //Web server per l'AP di configurazione
    AsyncWebServer* webServer;

    //Oggetto contenente le Credenziali
    WiFiCredential credenziali;

    void loadConnectionParams();

  public:
    ConnectionManager();
    
    WIFI_CONNECTION_STATUS MakeConnection();
    
    void startConfigAP();
    
    bool isConnectionActive();
    
    void dumpConnectionStatus();
  
};

#endif
