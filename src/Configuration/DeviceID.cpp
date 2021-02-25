/**
 * @author: Massimo Lombardi
 * @since: 07/11/2020
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Classe per la gestione del device ID per il login alle risorse REST
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  25/02/2021 Creazione
 *   
 */

#include "DeviceID.h"


const char DeviceID::DEVICEID_LABEL[16] = "DeviceID";
const char DeviceID::DEVICEID_PLACEHOLDER[33] = "ID Device registrato sul portale";


DeviceID::DeviceID() {
    strcpy(deviceID, "");
}


DeviceID::DeviceID(char* id) {
    strcpy(deviceID, id);
}


DeviceID::DeviceID(String id) {
    strcpy(deviceID, id.c_str());
}


DeviceID::DeviceID(DynamicJsonDocument& doc) {
    strcpy(deviceID, doc["deviceID"]);
}


bool DeviceID::isValid(String id) {

    if(id == NULL) {
        Serial.println("Impossibile salvare un ide device NULL");
        return false;
    }

    if(id == "") {
      Serial.println("Impossibile salvare un ID device vuote");
      return false;
    }
        
    if((strlen(id.c_str()) <= DEVICEID_MIN_LEN) || (strlen(id.c_str()) >= DEVICEID_MAX_LEN)) {
        Serial.println("Il device ID che si cerca di inserire non rispetta i requisiti");
        return false;
    }
        
    strcpy(deviceID, id.c_str());
    Serial.println("Salvato device ID - " + this->toString());
    return true;
}


bool DeviceID::isValid() {

    if(this->deviceID == NULL)
        return false;

    if(this->deviceID[0] == '\0')
        return false;

    if((strlen(this->deviceID) <= DEVICEID_MIN_LEN) || (strlen(this->deviceID) >= DEVICEID_MAX_LEN)) 
        return false;

    return true;
}


bool DeviceID::loadFromConfigField() {

    if(deviceIDConfigField->getValue() == NULL) {
        Serial.println("Device ID non valido: oggetto vuoto");
        return false;
    }
        
    if(deviceIDConfigField->getValue()[0] == '\0') {
        Serial.println("Device ID non valido: campo vuoto");
        return false;
    }

    if((strlen(deviceIDConfigField->getValue()) <= DEVICEID_MIN_LEN) || (strlen(deviceIDConfigField->getValue()) >= DEVICEID_MAX_LEN)) {
        Serial.println("Device ID non valido: deviceID troppo lungo o troppo corto");
        return false;
    }

    strcpy(deviceID, deviceIDConfigField->getValue());
    Serial.println("Salvato device ID - " + this->toString());
    return true;

}


DynamicJsonDocument DeviceID::toJSON() {
    DynamicJsonDocument doc(200);
    doc["deviceID"] = getStringDeviceID();
    return doc;
}


String DeviceID::toString() {
    return "DeviceID: " + String(deviceID);
}