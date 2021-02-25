/**
 * @author: Massimo Lombardi
 * @since: 05/01/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Implementazione della classe per la gestione del comando di apertura sul relè
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  21/02/2021 Creazione
 * 
 */

#include <Arduino.h>
#include "OpenAction.h"

OpenAction::OpenAction() {};

void OpenAction::open(int pin, unsigned int delay_ms) {
    digitalWrite(pin, HIGH);
    delay(delay_ms);
    digitalWrite(pin, LOW);
}