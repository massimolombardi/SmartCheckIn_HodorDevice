#ifndef TESTSSUITE_H
#define TESTSSUITE_H
/**
 * @author: Massimo Lombardi
 * @since: 18/04/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Suite di test per la scheda CPU sviluppata
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  24/04/2021 Prima versione
 * 
 */

#define TEST_SUITE_VERSION "0.0.1"

//Costanti per la definizione dei tempi di pressione pulsante
#define SINGLE_CICLE_PRESS_DURATION_MS 50
#define CONTINUOUS_CICLE_PRESS_DURATION_MS 5000
#define EXIT_TESTS_SUITE_PRESS_DURATION_MS 10000

//Constanti temporali per l'esecuzione dei tests
#define TEST_CICLE_PERIODICITY_MS 2000
#define DELAY_BEFORE_START_MS 2000

class TestsSuite {
    
    private:

        int LED_PIN;
        int OPEN_ACTION_PIN;
        int ACTION_BUTTON_PIN;
        
        bool running = true;
        bool continuousTesting = false;


        /**
        * Metodo privato per la gestione dei comandi sui GPIO
        */
        void togglePinForDelay(int pin, int action, int delay_ms);
        
        
        /**
        * Metodo per la gestione dei comandi tramite user button
        */
        void buttonHandler();


        /**
        * Test della sequenza nominale di apertura e utilizzo led
        */        
        void testInstallation();

    public:

        /**
        * Costruttore di default
        *
        * @param ledPin GPIO per il comando del LED di notifica
        * @param actionPin GPIO per il comando del relay
        * @param btnPin GPIO del pulsante utente
        */
        TestsSuite(int ledPin, int actionPin, int btnPin);

        /**
        * Metodo per l'esecuzione della modalità di test
        */        
        void startTesting();

}; 

#endif