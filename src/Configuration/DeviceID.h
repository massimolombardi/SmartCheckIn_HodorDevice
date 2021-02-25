#ifndef DEVICEID_H
#define DEVICEID_H
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

#include <WString.h>
#include <ArduinoJson.h>
#include <ESPAsync_WiFiManager.h>

#define DEVICEID_MIN_LEN 1
#define DEVICEID_MAX_LEN 40
#define BUFFER_MAX_LEN (int)(DEVICEID_MIN_LEN + DEVICEID_MAX_LEN)


class DeviceID {

    private:   

        static const char DEVICEID_LABEL[16];
        static const char DEVICEID_PLACEHOLDER[33];

        char deviceID[DEVICEID_MAX_LEN];

        ESPAsync_WMParameter* deviceIDConfigField = new ESPAsync_WMParameter(DEVICEID_LABEL, DEVICEID_PLACEHOLDER, "", DEVICEID_MAX_LEN);;


    public:  
        DeviceID();
        DeviceID(char* id);
        DeviceID(String id);
        DeviceID(DynamicJsonDocument& doc);

        bool loadFromConfigField();
        ESPAsync_WMParameter getAPConfigParam();

        char* getDeviceID() { return deviceID; }        
        String getStringDeviceID() { return String(deviceID); }    
        ESPAsync_WMParameter* getDeviceIDConfigField() { return deviceIDConfigField; }

        bool isValid();        
        bool isValid(String id);

        String toString();
        DynamicJsonDocument toJSON();

};

#endif