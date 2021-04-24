#ifndef OPENACTION_H
#define OPENACTION_H
/**
 * @author: Massimo Lombardi
 * @since: 05/01/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Classe per la gestione del comando di apertura sul relè
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  21/02/2021 Creazione
 * 
 */

class OpenAction {

    private:

    public:

        /**
         * Costruttore di default
         */ 
        OpenAction();

        /**
         * Metodo per comandare l'apertura
         * 
         * @param pin Pin da utilizzare per il comando
         * @param delay_ms Durata del comando
         */         
        void open(int pin, unsigned int delay_ms);

};

#endif