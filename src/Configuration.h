#ifndef CONFIGURATION_H
#define CONFIGURATION_H
/**
 * @author: Massimo Lombardi
 * @since: 05/01/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Classe per la gestione delle impostazioni configurabili sull'AP di configurazione
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   1.1       M. Lombardi  09/01/2021 Aggiunta gestione della memorizzazione e cancellazione credenziali
 *   1.2       M. Lombardi  16/01/2021 Migliorata gestione caricamento, salvataggio e cancellazione configurazioni
 *                                     Introdotta gestione parametri di login a risorse Smart Check-In
 *   1.3       M. Lombardi  17/01/2021 Aggiunta documentazione delle chiamate di classe
 *   
 */

#include <ESPAsync_WiFiManager.h>
//#include "Configuration/BaseApiUrl.h"
//#include "Configuration/CheckOpenDelay.h"
#include "Configuration/WiFiCredential.h"
#include "Configuration/LoginCredential.h"

#define CREDENTIAL_FILENAME F("/wifi.json")
#define CONFIGURATION_FILENAME F("/config.json")
#define SMART_CHECKIN_FILENAME F("/smartcheckin.json")


class Configuration {

    private:
        //BaseApiUrl apiUrl;
        //CheckOpenDelay checkOpenDelay;
        WiFiCredential wifiCredential;
        LoginCredential loginCredential;


        /**
         * Metodo per il caricamento della configurazione WiFi da file che si occupa
         * anche del controllo di validità dei parametri caricati.
         * La funzione è privata in quanto utilizzata solo'allinterno della funziona pubblica initialize()
         * 
         * @return true se i parametri sono validi, false altrimenti
         */ 
        bool loadWiFiCredential();


        /**
         * Metodo per il salvataggio della configurazione di WiFi da file che si occupa
         * anche del controllo di validità dei parametri caricati. Per consentire reset di configurazioni separati
         * questa funzione verifica che il file su cui si scrive la configurazione esista, in questo caso infatti 
         * non è stato richiesto un reset della medesima configurazione e si tralascia il salvataggio. Infatti sull'AP
         * di configurazione (ad oggi) lasciare i parametri bianchi esprime la volontà di lasciarli invariati.
         * In questa release del firmware infatti non vengono ripristinati i valori salvati in memoria nell'AP di configurazione.
         * La funzione è privata in quanto utilizzata solo'allinterno della funziona pubblica save()
         * 
         * @return true se i parametri sono validi, false altrimenti
         */ 
        bool saveWiFiCredential(String SSID, String psw);


        /**
         * Metodo per il caricamento della configurazione di Login da file che si occupa
         * anche del controllo di validità dei parametri caricati.
         * La funzione è privata in quanto utilizzata solo'allinterno della funziona pubblica initialize()
         * 
         * @return true se i parametri sono validi, false altrimenti
         */ 
        bool loadLoginCredential();


         /**
         * Metodo per il salvataggio della configurazione di Login da file che si occupa
         * anche del controllo di validità dei parametri caricati. Per consentire reset di configurazioni separati
         * questa funzione verifica che il file su cui si scrive la configurazione esista, in questo caso infatti 
         * non è stato richiesto un reset della medesima configurazione e si tralascia il salvataggio. Infatti sull'AP
         * di configurazione (ad oggi) lasciare i parametri bianchi esprime la volontà di lasciarli invariati.
         * In questa release del firmware infatti non vengono ripristinati i valori salvati in memoria nell'AP di configurazione.
         * La funzione è privata in quanto utilizzata solo'allinterno della funziona pubblica save()
         * 
         * @return true se i parametri sono validi, false altrimenti
         */ 
        bool saveLoginCredential();


    public:

        /**
         * Costruttore di default dell'oggetto configurazione
         */ 
        Configuration();
        

        /**
         * Metodo per la creazione dei campi editabili sull'AP di configurazione per i parametri 
         * aggiuntivi. I parametri aggiuntivi sono tutti quelli utilizzati, eccetto dconfigurazione IP
         * e credenziali WiFi
         * anche del controllo di validità dei parametri caricati.
         * La funzione è privata in quanto utilizzata solo'allinterno della funziona pubblica initialize()
         * 
         * @param wifiManager oggetto ESPAsync_WiFiManager per poter aggiungere i parametri all'AP. 
         */ 
        void initForConfigAP(ESPAsync_WiFiManager& wifiManager);


        /**
         * Metodo per reset della configurazione di WiFi. Questo metodo elimina il file di configurazione 
         * relativo al WiFi.
         */ 
        void resetWiFiCredential();


        /**
         * Metodo per reset della configurazione di Login. Questo metodo elimina il file di configurazione 
         * relativo al Login.
         */ 
        void resetLoginCredential();


        /**
         * Metodo per reset della configurazione globale. Questo metodo elimina tutti i files di configurazione.
         */ 
        void factoryReset();


        /**
         * Metodo per l'inizializzazione della configurazione che si occupa di leggere i singoli files sui quali
         * sono salvati i vari parametri. Il metodo gestisce le validità verificate durante i vari caricamenti.
         * 
         * @return true se i parametri sono validi, false altrimenti
         */ 
        bool initialize();


         /**
         * Metodo per il salvataggio della configurazione che si occupa di scrivere sui singoli files sui quali
         * sono salvati i vari parametri. Il metodo gestisce le validità verificate durante i vari caricamenti.
         * 
         * @param wifiManager oggetto ESPAsync_WiFiManager per poter aggiungere i parametri all'AP. 
         * 
         * @return true se i parametri sono validi, false altrimenti
         */ 
        bool save(ESPAsync_WiFiManager& wifiManager);


        /**
         * Metodi di get per il recupero dei valori di configurazione salvati in memoria.
         */ 
        String getAPIBaseUrl();
        int getCheckOpenDelay();
        char* getWiFiSSID();
        char* getWiFiPassword();
        String getLoginUsername();
        String getLoginPassword();

};

#endif
