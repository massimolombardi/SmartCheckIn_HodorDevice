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

            /**
         * Costruttore di default
         * 
         * @param swVersion Numero di versione del firmware installata
         */ 
        DeviceStatus(String swVersion);

        /**
         * Getter per recuperare lo stato device
         * 
         * @return lo stato del device
         */ 
        bool getOperative();

        /**
         * Getter per recuperare la versione firmware
         * 
         * @return la versione firmware
         */ 
        String getSoftwareVersion();

        /**
         * Getter per recuperare lo stato di errore
         * 
         * @return lo stato di errore
         */ 
        String getErrorInfo();

        /**
         * Getter per recuperare l'identificativo device
         * 
         * @return l'identificativo device
         */ 
        String getDeviceID();

};

#endif