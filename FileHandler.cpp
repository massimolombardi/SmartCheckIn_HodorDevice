/*
 * @Author: Massimo Lombardi
 * @Since: 07/11/2020
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Implementazione della classe Singleton per la gestione del File System.
 *          sulla libreria FS.h
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   1.1       M. Lombardi  21/12/2020 INtrodotta lettura/scrittura di JSON da file
 *   
 */

#include "FileHandler.h"

FS* FileHandler::filesystem = NULL;

//Ritorna il puntatore all'oggetto 
FS* FileHandler::getInstance() {          
  if(filesystem == NULL) {              
    filesystem = &LITTLEFS;                 
    if(!FileFS.begin(true)) {
      Serial.println(F("Mount del FyleSystem fallito. Formattazione necessaria"));
    }
  }                            
  return filesystem;
}


bool FileHandler::writeJson(String fileName, DynamicJsonDocument* jsonObject) {

  if(fileName == "") {
    Serial.println("Il file richiesto non è valido");
    return false;
  }
  
  File file = getInstance()->open(fileName, "w");
  
  if(!file) {
    Serial.println("Errore nell'apertura del file in scrittura: " + fileName);
    return false;
  }

  serializeJson(*jsonObject, file);
  return true;
}


bool FileHandler::loadJson(String fileName, DynamicJsonDocument* jsonObject) {

  if(fileName == "") {
    Serial.println("Il file richiesto non è valido");
    return false;
  }

  File file = getInstance()->open(fileName, "r");
  if(!file) {
    Serial.println("Errore nell'apertura del file in lettura: " + fileName);
    return false;
  }

  size_t size = file.size();
  if(size > 1024) {
    Serial.println("Errore in file specificato è troppo grande");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);

  auto error = deserializeJson(*jsonObject, buf.get());
  if(error) {
    Serial.println("Errore durante il parsing del file letto: " + fileName);
    return false;
  }

  return true;
}


bool FileHandler::removeFile(String fileName) {

  if(fileName == "") {
    Serial.println("Il file richiesto non è valido");
    return false;
  }

  getInstance()->remove(fileName);
  return true;
  
}


void FileHandler::formatFilesystem() {
    Serial.println(F("Reset della configurazione in corso..."));
    FileFS.format();
    ESP.restart();
}
