#ifndef FILEHANDLER_H
#define FILEHANDLER_H
/**
 * @author: Massimo Lombardi
 * @since: 07/11/2020
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Classe Singleton per la gestione del File System attraverso la libreria LittleFS per ESP32. 
 *         Questa classe va inclusa per prima altrimenti l'applicativo crasha per colpa di un problema sulla libreria FS.h
 *         La classe si occupa di astrarre ulteriormente l'utilizzo del File System, rendendo più sicuro l'accesso all'oggetto
 *         File System esposto da LittleFS. La libreria NON include alcun meccanismo di Thread-Safety.
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  13/12/2020 Creazione
 *   1.1       M. Lombardi  21/12/2020 Introdotta lettura/scrittura di JSON da file
 *   1.2       M. Lombardi  23/12/2020 Introdotta documentazione di utilizzo
 *   
 */

#include <FS.h>
#include <LITTLEFS.h> 
#include <ArduinoJson.h>


#define FileFS LITTLEFS
#define FS_Name "LittleFS"

/**
 * Impostando a true questa macro si consente la formattazione automatica del File System in caso
 * di errori durante il mount della risorsa. Questi errori sono spesso dovuti ad una mancata formattazione
 * del File System al primo avvio del device. Non è tuttavia possibile rilevare eventuali errori di mount
 * causati da un avvio sporco della scheda prima di effettuare la formattazione.
 */
#define AUTO_FORMAT_ON_MOUNT_FAILED true

/**
 * Dimensione massima dei file che è possibile aprire.
 */ 
#define MAX_FILE_SIZE 2048


class FileHandler {

    public:

        /**
         * Metodo statico per la scrittura di un file JSON su File System.
         * 
         * @param fileName contiene il nome del file da scrivere
         * @param jsonObject è un puntatore all'oggetto DynamicJsonDocument da scrivere sul file
         * 
         * @return true se la scrittura va a buon fine, false altrimenti
         */ 
        static bool writeJson(String fileName, DynamicJsonDocument* jsonObject);

        /**
         * Metodo statico per la lettura di un file JSON da File System.
         * 
         * @param fileName contiene il nome del file da leggere
         * @param jsonObject è un puntatore all'oggetto DynamicJsonDocument su cui scrivere le informazioni lette
         * 
         * @return true se la lettura va a buon fine, false altrimenti
         */ 
        static bool loadJson(String fileName, DynamicJsonDocument* jsonObject);

        /**
         * Metodo statico per la cancellazione di un file da File System.
         * 
         * @param fileName contiene il nome del file da eliminare
         * 
         * @return true se l'eliminazione va a buon fine, false altrimenti
         */ 
        static bool deleteFile(String fileName);


        /**
         * Metodo statico per il controllo di esistenza di un file da File System.
         * 
         * @param fileName contiene il nome del file cercato
         * 
         * @return true se il file esiste, false altrimenti
         */ 
        static bool fileExists(String fileName);


        /**
         * Metodo statico per la formattazione del File System.
         * Questo metodo deve essere invocato in caso si voglia effettuare una formattazione completa del File System
         * eliminando completamente tutti i files salvati. Il metodo non richiede conferma alcuna prima di effettuare l'operazione.
         */ 
        static void formatFilesystem();


    private:

        /**
         * Puntatore all'oggetto file System gestito da LittleFS.
         */ 
        static FS* filesystem;

        /**
         * Costruttore privato in modo da evitare istanze multiple dell'oggetto File System.
         */ 
        FileHandler() {}

        /**
         * Costruttore di copia privato in modo da evitare duplicazioni dell'oggetto file System.
         */ 
        FileHandler(FileHandler const&);

        /**
         * Metodo privato per l'assegnamento di un oggetto di tipo classe in modo da evitare duplicazioni inutili 
         * dell'oggetto file System.
         */ 
        void operator=(FileHandler const&);

        /**
         * Metodo privato per l'accesso all'oggetto File System.
         * Il metodo di accesso all'oggetto file System è utilizzato esclusivamente all'interno della classe,
         * in questo modo non è possibile effettuare operazioni sul file System non previste dalla classe di gestione.
         * 
         * @return il puntatore all'oggetto File System
         */ 
        static FS* getInstance();

};

#endif
