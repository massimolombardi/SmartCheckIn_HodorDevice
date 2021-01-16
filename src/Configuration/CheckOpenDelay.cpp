/*
 * @Author: Massimo Lombardi
 * @Since: 05/01/2021
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Implementazione della classe per la gestione della chiave di configurazione Check Open Delay
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   
 */
/*
#include "CheckOpenDelay.h"

CheckOpenDelay::CheckOpenDelay() {
    parameter = new ESPAsync_WMParameter(LABEL, PLACEHOLDER, convertedValue, 2);
}


CheckOpenDelay::CheckOpenDelay(DynamicJsonDocument* jdoc) {
    if(jdoc["checkOpenDelay"] != NULL && jdoc["checkOpenDelay"][0] != "\0")
       strcpy(convertedValue, jdoc["checkOpenDelay"]);
    parameter = new ESPAsync_WMParameter(LABEL, PLACEHOLDER, convertedValue, 2);

}


ESPAsync_WMParameter* CheckOpenDelay::getParam() {
    return parameter;
}


int CheckOpenDelay::getValue() {
    if(isValid())
        return value;
    return defaultValue;
}


void CheckOpenDelay::setValue(int newValue) {
    value = newValue;
}


bool CheckOpenDelay::isValid() {
    if(value <= 0 || value >= 99) {
        Serial.println("Valore non ammesso per CheckOpenDelay, impostato al valore di default");
        return false;
    }
    return true;
}


DynamicJsonDocument CheckOpenDelay::toJSON() {
    DynamicJsonDocument doc(32);
    doc["checkOpenDelay"] = value;
    return doc;
}*/