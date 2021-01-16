#ifndef CHECKOPENDELAY_H
#define CHECKOPENDELAY_H
/*
 * @Author: Massimo Lombardi
 * @Since: 05/01/2021
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Classe per la gestione della chiave di configurazione Check Open Delay
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   
 */

#include <ArduinoJson.h>
#include <ESPAsync_WiFiManager.h>

/*
class CheckOpenDelay {

    private:

        ESPAsync_WMParameter* parameter;

        static const int DEFAULT_VALUE = 30;
        static const char LABEL[16] = "CheckOpenDelay";
        static const char PLACEHOLDER[34] = "Controllo Apertura Porta Ogni (s)";

        int value = 30;
        char convertedValue[3] = "30";


    public:
        CheckOpenDelay();
        CheckOpenDelay(DynamicJsonDocument* jdoc);

        ESPAsync_WMParameter* getParam();
        
        bool isValid();
        int getValue();
        void loadFefault();
        void setValue(int newValue);

        DynamicJsonDocument toJSON();

};

#endif*/