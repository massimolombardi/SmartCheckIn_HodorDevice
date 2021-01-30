/**
 * @author: Massimo Lombardi
 * @since: 05/01/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Classe per la gestione delle impostazioni configurabili sull'AP di configurazione
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   1.1       M. Lombardi  09/01/2021 Aggiunta gestione della memorizzazione e cancellazione credenziali
 *   1.2       M. Lombardi  16/01/2021 Migliorata gestione caricamento, salvataggio e cancellazione configurazioni
 *                                     Introdotta gestione parametri di login a risorse Smart Check-In
 *   1.3       M. Lombardi  17/01/2021 Aggiunta documentazione delle chiamate di classe
 *   
 */

#include <ArduinoJson.h>
#include "Configuration.h"
#include "FileSystem/FileHandler.h"


Configuration::Configuration() {
    wifiCredential = WiFiCredential();
    loginCredential = LoginCredential();
}


void Configuration::initForConfigAP(ESPAsync_WiFiManager& wifiManager)  {
    wifiManager.addParameter(loginCredential.getUsernameConfigField());
    wifiManager.addParameter(loginCredential.getPasswordConfigField());
}


bool Configuration::loadWiFiCredential() {
    
    Serial.println("Lettura delle credenziali da file");

    DynamicJsonDocument doc(200);
    if(!FileHandler::loadJson(CREDENTIAL_FILENAME, &doc))
        return false;

    wifiCredential = WiFiCredential(doc);
    return wifiCredential.isValid();
}


bool Configuration::saveWiFiCredential(String SSID, String psw) {
    
    //Se quando chiedo di salvare il file esiste significa che non è stato richiesto il reset di quell'impostazione
    //quindi ignoro il salvataggio di questa parte e considero sia già stata fatta, in modo da evitare la segnalazione non voluta di errori
    if(FileHandler::fileExists(CREDENTIAL_FILENAME))
        return true;
    
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


bool Configuration::loadLoginCredential() {
    
    Serial.println("Lettura delle credenziali da file");

    DynamicJsonDocument doc(200);
    if(!FileHandler::loadJson(SMART_CHECKIN_FILENAME, &doc))
        return false;

    loginCredential = LoginCredential(doc);
    return loginCredential.isValid();
}


bool Configuration::saveLoginCredential() {

    //Se quando chiedo di salvare il file esiste significa che non è stato richiesto il reset di quell'impostazione
    //quindi ignoro il salvataggio di questa parte e considero sia già stata fatta, in modo da evitare la segnalazione non voluta di errori
    if(FileHandler::fileExists(SMART_CHECKIN_FILENAME))
        return true;

    if(loginCredential.loadFromConfigField()) {
        DynamicJsonDocument jdoc = loginCredential.toJSON();
        return FileHandler::writeJson(SMART_CHECKIN_FILENAME, &jdoc);
    }
    else {
        Serial.println("Lettura dei parametri di Login dal portale di configurazione fallita");
        return false;
    }
    return true;
}


void Configuration::resetLoginCredential() {
    FileHandler::deleteFile(SMART_CHECKIN_FILENAME);
}


bool Configuration::initialize() {

    bool loadingOk = true; 

    Serial.println("Lettura della configurazione da file");

    DynamicJsonDocument doc1(200);
    if(!FileHandler::loadJson(CREDENTIAL_FILENAME, &doc1)) {
        Serial.println("Impossibile caricare il file " + String(CREDENTIAL_FILENAME));
        loadingOk = false;
    }
    else {
        wifiCredential = WiFiCredential(doc1);
        if(wifiCredential.isValid())
            Serial.println("Configurazione WiFi caricata con successo");
        else
            loadingOk = false;
    }
        
    DynamicJsonDocument doc2(200);
    if(!FileHandler::loadJson(SMART_CHECKIN_FILENAME, &doc2)) {
        Serial.println("Impossibile caricare il file " + String(SMART_CHECKIN_FILENAME));
        loadingOk = false;
    }
    else {
        loginCredential = LoginCredential(doc2);
        if(loginCredential.isValid())
            Serial.println("Configurazione Login caricata con successo");
        else
            loadingOk = false;
    }  
    
    return loadingOk;
}


bool Configuration::save(ESPAsync_WiFiManager& wifiManager) {
    
    bool saveOk = true;

    saveOk &= saveWiFiCredential(wifiManager.getSSID(), wifiManager.getPW());
    saveOk &= saveLoginCredential();

    return saveOk;
}


void Configuration::factoryReset() {
    FileHandler::formatFilesystem();
}



String Configuration::getAPIBaseUrl() {
    return "https://smartcheckin.cloud";
}


int Configuration::getCheckOpenDelay() {
    return 30;
};


String Configuration::getLoginUsername() {
    return loginCredential.getStringUsername();
}


String Configuration::getLoginPassword() {
    return loginCredential.getStringPassword();
}


char* Configuration::getWiFiSSID() {
    return wifiCredential.getSSID();
}


char* Configuration::getWiFiPassword() {
    return wifiCredential.getPassword();
}


/* Funzione per il merge di piu file json
void mergeJSON(JsonObject dest, JsonObjectConst src) {
   for (auto kvp : src)
     dest[kvp.key()] = kvp.value();
}
*/