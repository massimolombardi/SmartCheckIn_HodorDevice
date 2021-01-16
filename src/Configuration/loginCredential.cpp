/*
 * @Author: Massimo Lombardi
 * @Since: 07/11/2020
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Implementazione della classe per il login alle risorse REST
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   
 */

#include "LoginCredential.h"


const char LoginCredential::USERNAME_LABEL[16] = "loginUsername";
const char LoginCredential::USERNAME_PLACEHOLDER[32] = "Username Account Smart Check-In";
const char LoginCredential::PASSWORD_LABEL[16] = "loginPassword";
const char LoginCredential::PASSWORD_PLACEHOLDER[32] = "Password Account Smart Check-In";


LoginCredential::LoginCredential() {
    strcpy(username, "");
    strcpy(password, "");
}


LoginCredential::LoginCredential(char* user, char* psw) {
    strcpy(username, user);
    strcpy(password, psw);
}


LoginCredential::LoginCredential(String user, String psw) {
    strcpy(username, user.c_str());
    strcpy(password, psw.c_str());
}


LoginCredential::LoginCredential(DynamicJsonDocument& doc) {
    strcpy(username, doc["username"]);
    strcpy(password, doc["password"]);
}


bool LoginCredential::isValid(String user, String psw) {

    if(user == NULL || psw == NULL) {
        Serial.println("Impossibile salvare credenziali di login NULL");
        return false;
    }

    if(user == "" || psw == "") {
      Serial.println("Impossibile salvare credenziali di login vuote");
      return false;
    }
        
    if((strlen(user.c_str()) <= USERNAME_MIN_LEN) || (strlen(user.c_str()) >= USERNAME_MAX_LEN)) {
        Serial.println("Le credenziali di login che si cercano di salvare non rispettano i requisiti");
        return false;
    }
    
    if((strlen(psw.c_str()) < PASSWORD_MIX_LEN) || (strlen(psw.c_str()) >= PASSWORD_MAX_LEN)) {
      Serial.println("Le credenziali di login che si cercano di salvare non rispettano i requisiti");
      return false;
    }
        
    strcpy(username, user.c_str());
    strcpy(password, psw.c_str());
    Serial.println("Salvate credenziali di login - " + this->toString());
    return true;
}


bool LoginCredential::isValid() {

    if(this->username == NULL || this->password == NULL)
        return false;

    if(this->username[0] == '\0' || this->password[0] == '\0')
        return false;

    if((strlen(this->username) <= USERNAME_MIN_LEN) || (strlen(this->username) >= USERNAME_MAX_LEN)) 
        return false;

    if((strlen(this->password) < PASSWORD_MIX_LEN) || (strlen(this->password) >= PASSWORD_MAX_LEN)) 
        return false;

    return true;
}


bool LoginCredential::loadFromConfigField() {

    if(usernameConfigField->getValue() == NULL || passwordConfigField->getValue() == NULL) {
        Serial.println("Parametri di Login non validi: oggetto vuoto");
        return false;
    }
        
    if(usernameConfigField->getValue()[0] == '\0' || passwordConfigField->getValue()[0] == '\0') {
        Serial.println("Parametri di Login non validi: campo vuoto");
        return false;
    }

    if((strlen(usernameConfigField->getValue()) <= USERNAME_MIN_LEN) || (strlen(usernameConfigField->getValue()) >= USERNAME_MAX_LEN)) {
        Serial.println("Parametri di Login non validi: username troppo lungo o troppo corto");
        return false;
    }

    if((strlen(passwordConfigField->getValue()) < PASSWORD_MIX_LEN) || (strlen(passwordConfigField->getValue()) >= PASSWORD_MAX_LEN)) {
        Serial.println("Parametri di Login non validi: password troppo lunga o troppo corta");
        return false;
    }

    strcpy(username, usernameConfigField->getValue());
    strcpy(password, passwordConfigField->getValue());
    Serial.println("Salvate credenziali di login - " + this->toString());
    return true;

}


DynamicJsonDocument LoginCredential::toJSON() {
    DynamicJsonDocument doc(200);
    doc["username"] = getStringUsername();
    doc["password"] = getStringPassword();
    return doc;
}


String LoginCredential::toString() {
    return "Username: " + String(username) + " " + "Password: " + String(password);
}