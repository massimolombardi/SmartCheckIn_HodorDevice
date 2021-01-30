#ifndef LOGINCREDENTIAL_H
#define LOGINCREDENTIAL_H
/**
 * @author: Massimo Lombardi
 * @since: 07/11/2020
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Classe per la gestione delle credenziali per il login alle risorse REST
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   
 */

#include <WString.h>
#include <ArduinoJson.h>
#include <ESPAsync_WiFiManager.h>

#define USERNAME_MIN_LEN 1
#define USERNAME_MAX_LEN 32
#define PASSWORD_MIX_LEN 8
#define PASSWORD_MAX_LEN 64
#define BUFFER_MAX_LEN (int)(USERNAME_MIN_LEN + USERNAME_MAX_LEN)


class LoginCredential {

    private:   

        static const char USERNAME_LABEL[16];
        static const char USERNAME_PLACEHOLDER[32];
        static const char PASSWORD_LABEL[16];
        static const char PASSWORD_PLACEHOLDER[32];

        char username[USERNAME_MAX_LEN];
        char password[PASSWORD_MAX_LEN];

        ESPAsync_WMParameter* usernameConfigField = new ESPAsync_WMParameter(USERNAME_LABEL, USERNAME_PLACEHOLDER, "", USERNAME_MAX_LEN);;
        ESPAsync_WMParameter* passwordConfigField = new ESPAsync_WMParameter(PASSWORD_LABEL, PASSWORD_PLACEHOLDER, "", PASSWORD_MAX_LEN);


    public:  
        LoginCredential();
        LoginCredential(char* user, char* psw);
        LoginCredential(String user, String psw);
        LoginCredential(DynamicJsonDocument& doc);

        bool loadFromConfigField();
        ESPAsync_WMParameter getAPConfigParam();

        char* getUsername() { return username; }        
        char* getPassword() { return password; }
        String getStringUsername() { return String(username); }    
        String getStringPassword() { return String(password); }
        ESPAsync_WMParameter* getUsernameConfigField() { return usernameConfigField; }
        ESPAsync_WMParameter* getPasswordConfigField() { return passwordConfigField; }

        bool isValid();        
        bool isValid(String user, String psw);

        String toString();
        DynamicJsonDocument toJSON();

};

#endif