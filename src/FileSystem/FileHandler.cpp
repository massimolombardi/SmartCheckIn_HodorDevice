/*
 * @Author: Massimo Lombardi
 * @Since: 07/11/2020
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Classe Singleton per la gestione del File System attraverso la libreria LittleFS per ESP32. 
 *         Questa classe va inclusa per prima altrimenti l'applicativo crasha per colpa di un problema sulla libreria FS.h
 *         La classe si occupa di astrarre ulteriormente l'utilizzo del File System, rendendo più sicuro l'accesso all'oggetto
 *         File System esposto da LittleFS. La libreria NON include alcun meccanismo di Thread-Safety.
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   1.1       M. Lombardi  21/12/2020 Introdotta lettura/scrittura di JSON da file
 *   
 */

#include "FileHandler.h"

FS* FileHandler::filesystem = NULL;

FS* FileHandler::getInstance() {          
  
  if(filesystem == NULL) {              
    
    filesystem = &LITTLEFS;                 
    
    if(!FileFS.begin(true)) {

      Serial.println(F("Mount del File System fallito. Formattazione necessaria"));

      #if AUTO_FORMAT_ON_MOUNT_FAILED
      formatFilesystem();
      #endif
    }

  }                            
  return filesystem;
}


bool FileHandler::writeJson(String fileName, DynamicJsonDocument* jsonObject) {

  if(fileName == NULL || fileName == "") {
    Serial.println("Il file richiesto non è valido");
    return false;
  }
  
  File file = getInstance()->open(fileName, "w");  
  if(!file) {
    Serial.println("Errore nell'apertura in scrittura del file: " + fileName);
    return false;
  }

  serializeJson(*jsonObject, file);
  return true;
}


bool FileHandler::loadJson(String fileName, DynamicJsonDocument* jsonObject) {

  if(fileName == NULL || fileName == "") {
    Serial.println("Il file richiesto non è valido");
    return false;
  }

  File file = getInstance()->open(fileName, "r");
  if(!file) {
    Serial.println("Errore nell'apertura in lettura del file: " + fileName);
    return false;
  }

  size_t size = file.size();
  if(size > MAX_FILE_SIZE) {
    Serial.println("Il file che si richiede di aprire è troppo grande");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);

  auto error = deserializeJson(*jsonObject, buf.get());
  if(error) {
    Serial.println("Errore durante il parsing del JSON letto dal file: " + fileName);
    return false;
  }

  return true;
}


bool FileHandler::deleteFile(String fileName) {

  if(fileName == NULL || fileName == "") {
    Serial.println("Il file richiesto non è valido");
    return false;
  }

  getInstance()->remove(fileName);
  return true;
}

bool FileHandler::fileExists(String fileName) {

  if(fileName == NULL || fileName == "") {
    Serial.println("Il file richiesto non è valido");
    return false;
  }

  return getInstance()->exists(fileName);
}


void FileHandler::formatFilesystem() {
    Serial.println(F("Formattazione File System in corso..."));
    FileFS.format();
    Serial.println(F("Formattazione completata, riavvio il dispositivo"));
    ESP.restart();
}
