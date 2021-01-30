/**
 * @author: Massimo Lombardi
 * @since: 07/11/2020
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Implementazione della classe per la gestione delle credenziali delle reti WiFi
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   
 */

#include "WiFiCredential.h"


WiFiCredential::WiFiCredential() {
    strcpy(ssid, "");
    strcpy(password, "");
}


WiFiCredential::WiFiCredential(String id, String psw) {
    strcpy(ssid, id.c_str());
    strcpy(password, psw.c_str());
}


WiFiCredential::WiFiCredential(DynamicJsonDocument& doc) {
    strcpy(ssid, doc["ssid"]);
    strcpy(password, doc["password"]);
}


bool WiFiCredential::isValid(String SSID, String psw) {

    if(SSID == NULL || psw == NULL) {
        Serial.println("Impossibile salvare credenziali NULL");
        return false;
    }

    if(SSID == "" || psw == "") {
      Serial.println("Impossibile salvare credenziali vuote");
      return false;
    }
        
    if((strlen(SSID.c_str()) <= SSID_MIN_LEN) || (strlen(SSID.c_str()) >= SSID_MAX_LEN)) {
        Serial.println("Le credenziali che si cercano di salvare non rispettano i requisiti");
        return false;
    }
    
    if((strlen(psw.c_str()) <= PASSWORD_MIX_LEN) || (strlen(psw.c_str()) >= PASSWORD_MAX_LEN)) {
      Serial.println("Le credenziali che si cercano di salvare non rispettano i requisiti");
      return false;
    }
        
    strcpy(ssid, SSID.c_str());
    strcpy(password, psw.c_str());
    Serial.println("Salvate credenziali - " + this->toString());
    return true;
}


bool WiFiCredential::isValid() {

    if(this->ssid == NULL || this->password == NULL)
        return false;

    if(this->ssid[0] == '\0' || this->password[0] == '\0')
        return false;

    if((strlen(this->ssid) <= SSID_MIN_LEN) || (strlen(this->ssid) >= SSID_MAX_LEN)) 
        return false;

    if((strlen(this->password) <= PASSWORD_MIX_LEN) || (strlen(this->password) >= PASSWORD_MAX_LEN)) 
        return false;

    return true;
}


DynamicJsonDocument WiFiCredential::toJSON() {
    DynamicJsonDocument doc(200);
    doc["ssid"] = getStringSSID();
    doc["password"] = getStringPassword();
    return doc;
}


String WiFiCredential::toString() {
    return "SSID: " + String(ssid) + " " + "Password: " + String(password);
}