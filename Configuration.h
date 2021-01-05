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
 *   
 */

#include "configuration/BaseApiUrl.h"
#include "configuration/CheckOpenDelay.h"

#include <ESPAsync_WiFiManager.h>

class Configuration {

    private:
        BaseApiUrl apiUrl;
        CheckOpenDelay checkOpenDelay;

    public:
        Configuration();
        bool saveOnFile();
        bool readFromFile();
        void initForConfigAP(ESPAsync_WiFiManager& wifiManager);
};

#endif
