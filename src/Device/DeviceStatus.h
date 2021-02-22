#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H
/**
 * @author: Massimo Lombardi
 * @since: 05/01/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Classe per la gestione dello stato device
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  21/02/2021 Creazione
 *   1.1	   M. Lombardi  21/02/2021 Introdotta struttura di gestione stato
 * 
 */

#include <WString.h>

#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())


class DeviceStatus {


    private:
        bool operative = true;
        String softwareVersionNumber;
        String errorInfo = "ok"; 
        String deviceID = "";
    
    
    public:
        DeviceStatus(String swVersion);
        bool getOperative();
        String getSoftwareVersion();
        String getErrorInfo();
        String getDeviceID();

};

#endif