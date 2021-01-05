    /*
 * @Author: Massimo Lombardi
 * @Since: 05/01/2021
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Implementazione della classe per la gestione delle impostazioni configurabili sull'AP di configurazione
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   
 */

#include "Configuration.h"

Configuration::Configuration() {}

bool Configuration::saveOnFile() {}

bool Configuration::readFromFile() {}

void Configuration::initForConfigAP(ESPAsync_WiFiManager& wifiManager)  {
    wifiManager.addParameter(apiUrl.getParam());
    wifiManager.addParameter(checkOpenDelay.getParam());
}
