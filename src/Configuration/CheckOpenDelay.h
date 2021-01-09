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

#include <ESPAsync_WiFiManager.h>

class CheckOpenDelay {

    private:

        ESPAsync_WMParameter* parameter;

        char LABEL[16] = "CheckOpenDelay";
        char PLACEHOLDER[34] = "Controllo Apertura Porta Ogni (s)";

        int value = 30;
        int defaultValue = 30;
        char convertedValue[3] = "30";

    public:

        CheckOpenDelay() {
            parameter = new ESPAsync_WMParameter(LABEL, PLACEHOLDER, convertedValue, 2);
        }

        int getValue() {
            if(isValid())
                return value;
            return defaultValue;
        }

        ESPAsync_WMParameter* getParam() {
            return parameter;
        }

        void setValue(int newValue) {
            value = newValue;
        }

        bool isValid() {
            if(value <= 0 || value >= 99) {
                Serial.println("Valore non ammesso per CheckOpenDelay, impostato al valore di default");
                return false;
            }
            return true;
        }

};

#endif