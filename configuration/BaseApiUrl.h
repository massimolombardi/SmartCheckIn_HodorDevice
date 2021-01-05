#ifndef BASEURLAPI_H
#define BASEURLAPI_H
/*
 * @Author: Massimo Lombardi
 * @Since: 05/01/2021
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Classe per la gestione della chiave di configurazione API Base URL
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   
 */

#include <ESPAsync_WiFiManager.h>

#define API_BASE_URL_LEN 64

class BaseApiUrl {

    private:

        ESPAsync_WMParameter* parameter;

        char LABEL[16] = "BaseApiUrl";
        char PLACEHOLDER[32] = "URL Base per l'invocazione API";

        char value[API_BASE_URL_LEN] = "https://smartcheckin.cloud/api/v1";
        char defaultValue[API_BASE_URL_LEN] = "https://smartcheckin.cloud/api/v1";

    public:

        BaseApiUrl() {
            parameter = new ESPAsync_WMParameter(LABEL, PLACEHOLDER, value, API_BASE_URL_LEN);
        }

        String getValue() {
            if(isValid())
                return String(value);
            return String(defaultValue);
        }

        void setValue(char* newValue) {
            strcpy(value, newValue);
        }

        ESPAsync_WMParameter* getParam() {
            return parameter;
        }

        bool isValid() {
            if(value == NULL || value == "") {
                Serial.println("Valore non ammesso per BaseApiUrl, impostato al valore di default");
                return false;
            }           
            //else if(match("https:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&=]*)", String(value))) {
            //    Serial.println("Valore non ammesso per BaseApiUrl, impostato al valore di default");
                return false;  
            //}
            return true;
        }

};

#endif