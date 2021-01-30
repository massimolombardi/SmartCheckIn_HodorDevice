#ifndef WIFICREDENTIAL_H
#define WIFICREDENTIAL_H
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

#include <WString.h>
#include <ArduinoJson.h>

#define SSID_MIN_LEN 1
#define SSID_MAX_LEN 32
#define PASSWORD_MIX_LEN 8
#define PASSWORD_MAX_LEN 64
#define BUFFER_MAX_LEN (int)(SSID_MAX_LEN + PASSWORD_MAX_LEN)


class WiFiCredential {

    private:        
        char ssid[SSID_MAX_LEN];
        char password[PASSWORD_MAX_LEN];


    public:  
        WiFiCredential();
        WiFiCredential(String id, String psw);
        WiFiCredential(DynamicJsonDocument& doc);

        char* getSSID() { return ssid; }        
        char* getPassword() { return password; }
        String getStringSSID() { return String(ssid); }    
        String getStringPassword() { return String(password); }

        bool isValid();        
        bool isValid(String SSID, String psw);

        String toString();
        DynamicJsonDocument toJSON();

};

#endif
