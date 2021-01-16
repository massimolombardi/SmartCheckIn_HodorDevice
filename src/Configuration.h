#ifndef CONFIGURATION_H
#define CONFIGURATION_H
/*
 * @Author: Massimo Lombardi
 * @Since: 05/01/2021
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Classe per la gestione delle impostazioni configurabili sull'AP di configurazione
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   1.1       M. Lombardi  09/01/2021 Aggiunta gestione della memorizzazione e cancellazione credenziali
 *   
 */

#include <ESPAsync_WiFiManager.h>
//#include "Configuration/BaseApiUrl.h"
//#include "Configuration/CheckOpenDelay.h"
#include "Configuration/WiFiCredential.h"
#include "Configuration/LoginCredential.h"

#define CREDENTIAL_FILENAME F("/wifi.json")
#define CONFIGURATION_FILENAME F("/config.json")
#define SMART_CHECKIN_FILENAME F("/smartcheckin.json")


class Configuration {

    private:
        //BaseApiUrl apiUrl;
        //CheckOpenDelay checkOpenDelay;
        WiFiCredential wifiCredential;
        LoginCredential loginCredential;


    public:
        Configuration();
        
        void initForConfigAP(ESPAsync_WiFiManager& wifiManager);

        /**
         * Funzioni per la gestione delle impostazioni WiFi
         */ 
        bool loadWiFiCredential();
        void resetWiFiCredential();
        bool saveWiFiCredential(String SSID, String psw);
        char* getWiFiSSID();
        char* getWiFiPassword();


        /**
         * Funzioni per la gestione delle impostazioni di Login
         */ 
        bool loadLoginCredential();
        void resetLoginCredential();
        bool saveLoginCredential();
        String getLoginUsername();
        String getLoginPassword();

        void factoryReset();

        String getAPIBaseUrl();
        String getUsername();
        String getPassword();
        int getCheckOpenDelay();

        bool initialize();
        bool save(ESPAsync_WiFiManager& wifiManager);

};

#endif
