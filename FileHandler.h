#ifndef FILEHANDLER_H
#define FILEHANDLER_H
/*
 * @Author: Massimo Lombardi
 * @Since: 07/11/2020
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Classe Singleton per la gestione del File System. Questa classe va inclusa per prima altrimenti l'applicativo crasha per colpa di un problema
 *          sulla libreria FS.h
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   1.1       M. Lombardi  21/12/2020 INtrodotta lettura/scrittura di JSON da file
 *   
 */

#include <FS.h>
#include <LITTLEFS.h> 
#include <ArduinoJson.h>


#define FileFS LITTLEFS
#define FS_Name "LittleFS"


class FileHandler {

    public:

        static FS* getInstance();
        static bool writeJson(String fileName, DynamicJsonDocument* jsonObject);
        static bool loadJson(String fileName, DynamicJsonDocument* jsonObject);
        static bool removeFile(String fileName);
        static void formatFilesystem();

    private:

        static FS* filesystem;

        FileHandler() {}
        FileHandler(FileHandler const&);
        void operator=(FileHandler const&);

};

#endif
