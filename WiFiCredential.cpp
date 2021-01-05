/*
 * @Author: Massimo Lombardi
 * @Since: 07/11/2020
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Implementazione della classe per la gestione delle credenziali delle reti WiFi
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   
 */

#include "WiFiCredential.h"
#include "FileHandler.h"


bool WiFiCredential::storeCredential(String SSID, String psw) {

    if(SSID == "" || psw == "") {
      Serial.println("Impossibile salvare credenziali vuote");
      return false;
    }
        
    if((strlen(SSID.c_str()) >= SSID_MAX_LEN) || (strlen(psw.c_str()) <= PASSWORD_MIX_LEN) || (strlen(psw.c_str()) >= PASSWORD_MAX_LEN)) {
      Serial.println("Le credenziali che si cercano di salvare non rispettano i requisiti");
      return false;
    }
        

    strcpy(ssid, SSID.c_str());
    strcpy(password, psw.c_str());

    Serial.print("Salvate credenziali: ");
    Serial.print(SSID);
    Serial.print(" ");
    Serial.print(psw);
    Serial.print("\n");
    return true;
}


bool WiFiCredential::isValidCredential() {

    if(this->ssid == "" || this->password == "")
        return false;

    if((strlen(this->ssid) >= SSID_MAX_LEN) || (strlen(this->password) <= PASSWORD_MIX_LEN) || (strlen(this->password) >= PASSWORD_MAX_LEN)) 
        return false;

    return true;
}


void WiFiCredential::loadFromFile() {

  Serial.println("Lettura delle credenziali da file");

  DynamicJsonDocument doc(200);
  bool result = FileHandler::loadJson(CREDENTIAL_FILENAME, &doc);

  if(!result)
    return;

  const char* id = doc["ssid"];
  const char* psw = doc["password"];
  strcpy(ssid, id);
  strcpy(password, psw);

  Serial.print("Letto ssid: ");
  Serial.println(ssid);
  Serial.print("Letta Password: ");
  Serial.println(password);
}


void WiFiCredential::saveToFile() {

  DynamicJsonDocument doc(200);
  doc["ssid"] = getStringSSID();
  doc["password"] = getStringPassword();

  bool result = FileHandler::writeJson(CREDENTIAL_FILENAME, &doc);
  if (!result) {
    return;
  }
}


void WiFiCredential::removeCredential() {
  FileHandler::deleteFile(CREDENTIAL_FILENAME);
}
