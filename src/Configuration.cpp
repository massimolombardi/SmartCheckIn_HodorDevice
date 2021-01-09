    /*
 * @Author: Massimo Lombardi
 * @Since: 05/01/2021
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Implementazione della classe per la gestione delle impostazioni configurabili sull'AP di configurazione
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   1.1       M. Lombardi  09/01/2021 Aggiunta gestione della memorizzazione e cancellazione credenziali
 * 
 */

#include <ArduinoJson.h>
#include "Configuration.h"
#include "FileSystem/FileHandler.h"


Configuration::Configuration() {
    wifiCredential = WiFiCredential();
}


void Configuration::initForConfigAP(ESPAsync_WiFiManager& wifiManager)  {
    wifiManager.addParameter(apiUrl.getParam());
    wifiManager.addParameter(checkOpenDelay.getParam());
}


/***************************** GESTIONE Credenziali WiFi *****************************/

bool Configuration::loadWiFiCredential() {
    
    Serial.println("Lettura delle credenziali da file");

    DynamicJsonDocument doc(200);
    if(!FileHandler::loadJson(CREDENTIAL_FILENAME, &doc))
        return false;

    wifiCredential = WiFiCredential(doc);
    return wifiCredential.isValid();
}


bool Configuration::saveWiFiCredential(String SSID, String psw) {
    if(wifiCredential.isValid(SSID, psw)) {
        DynamicJsonDocument jdoc = wifiCredential.toJSON();
        return FileHandler::writeJson(CREDENTIAL_FILENAME, &jdoc);
    }
    else {
       return false;
    }
}


void Configuration::resetWiFiCredential() {
    FileHandler::deleteFile(CREDENTIAL_FILENAME);
}


char* Configuration::getWiFiSSID() {
    return wifiCredential.getSSID();
}


char* Configuration::getWiFiPassword() {
    return wifiCredential.getPassword();
}



/***************************** GESTIONE Altri Parametri *****************************/

void Configuration::factoryReset() {
    FileHandler::formatFilesystem();
}

String Configuration::getUsername() {}
String Configuration::getPassword() {}

String Configuration::getAPIBaseUrl() {
    return apiUrl.getValue();
}

int Configuration::getCheckOpenDelay() {
    return checkOpenDelay.getValue();
};
