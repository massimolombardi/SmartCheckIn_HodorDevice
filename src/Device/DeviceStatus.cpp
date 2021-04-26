/**
 * @author: Massimo Lombardi
 * @since: 05/01/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Implementazione della classe per la gestione dello stato device
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  21/02/2021 Creazione
 *   1.1	   M. Lombardi  21/02/2021 Introdotta struttura di gestione stato
 * 
 */

#include <ESP.h>
#include "DeviceStatus.h"


DeviceStatus::DeviceStatus(String swVersion) {
    deviceID = String(ESP_getChipId(), HEX);
    softwareVersionNumber = swVersion;
    operative = true;
    errorInfo = "Running";
}


bool DeviceStatus::getOperative() {
    return operative;
}

String DeviceStatus::getSoftwareVersion() {
    return softwareVersionNumber;
}


String DeviceStatus::getErrorInfo() {
    return errorInfo;
}


String DeviceStatus::getDeviceID() {
    return deviceID;
}