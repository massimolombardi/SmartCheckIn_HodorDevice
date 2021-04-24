#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
/**
 * @author: Massimo Lombardi
 * @since: 07/11/2020
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Classe per la gestione della connessione WiFi utilizzando ESPAsync_WiFiManager
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   1.1       M. Lombardi  09/01/2021 Migliorata gestione dell'AP di configurazione
 *                                     Migliorata gestione della memorizzazione credenziali
 *   1.2	   M. Lombardi  16/01/2021 Introdotta gestione parametri configurazione aggiuntivi
 *   1.3       M. Lombardi  20/04/2021 Configurato GPIO led nominale
 */

#include <WiFiMulti.h>
#include <ESPAsync_WiFiManager.h>              
#include "src/Configuration.h"

#define ESP_WIFIMANAGER_VERSION "1.3.0"

//Parametri di configurazione connessione
#define WIFI_MAX_CONNECTION_RETRY 5
#define WIFI_WAIT_BETWEEN_CONNECT_MS 1000
#define WIFI_WAIT_BEFORE_1ST_CONNECT_MS 0

//Parametri di configurazione segnalazione errori
#define BLINKS_NUM_ON_INVALID_CREDENTIAL 5
#define WAIT_BETWEEN_BLINKS 500

//Parametri portale di configurazione
#define DEVICE_NAME "SmartCheckIn_Hodor"
#define CONFIGURATION_LED 16
#define HTTP_CONFIGURATION_PORT 80
#define CONFIGURATION_AP_NAME "SmartCheckIn - Config"
#define CONFIGURATION_AP_PASSWORD "smartcheckin"


enum WIFI_CONNECTION_STATUS {
  WIFI_ERROR = -1,
  WIFI_DISCONNECTED = 0,
  WIFI_CONNECTED = 1 
};


/**
 * Classe per la gestione della connessione
 */ 
class ConnectionManager {

  private:
    //Oggetto per la gestione delle connessioni
    WiFiMulti wifiHandler;
  
    //Server DNS
    DNSServer dnsServer;
  
    //Web server per l'AP di configurazione
    AsyncWebServer* webServer;

    //Putantore all'Oggetto Configurazione
    Configuration* cfg;

    void notifyErrorWithLed(int numOfBlink);
    bool loadConnectionParams();


  public:
    /**
    * Costruttore di default
    *
    * @param configuration Puntatore all'oggetto configuration
    */ 
    ConnectionManager(Configuration* configuration);

    /**
    * Metodo per l'avvio dell'AP di configurazione
    */     
    void startConfigAP(); 

    /**
    * Metodo per stabilire la connessione
    * 
    * @return WIFI_CONNECTION_STATUS Stato della connessione
    */ 
    WIFI_CONNECTION_STATUS MakeConnection();
    
    /**
    * Metodo per la disconnessione
    */  
    void disconnect();   

    /**
    * Metodo per la verifica dello stato connessione
    */  
    bool isConnectionActive(); 

    /**
    * Metodo per la stampa dello stato connessione
    */  
    void dumpConnectionStatus();
  
};

#endif
