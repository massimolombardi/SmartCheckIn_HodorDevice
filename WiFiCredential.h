#ifndef WIFICREDENTIAL_H
#define WIFICREDENTIAL_H
/*
 * @Author: Massimo Lombardi
 * @Since: 07/11/2020
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Classe per la gestione delle credenziali delle reti WiFi
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   
 */

#include <WString.h>

#define SSID_MAX_LEN 32
#define PASSWORD_MIX_LEN 8
#define PASSWORD_MAX_LEN 64
#define BUFFER_MAX_LEN (int)(SSID_MAX_LEN + PASSWORD_MAX_LEN)

#define CREDENTIAL_FILENAME F("/wifi_cred.dat")


class WiFiCredential {

    private:    
        char ssid[SSID_MAX_LEN];
        char password[PASSWORD_MAX_LEN];


    public:        
        String getStringSSID() { return String(ssid); }    
        String getStringPassword() { return String(password); }
        
        char* getSSID() { return ssid; }        
        char* getPassword() { return password; }

        bool isValidCredential();
        bool storeCredential(String SSID, String psw);

        void loadFromFile();
        void saveToFile();
        void removeCredential();
};

#endif
