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

#include <Arduino.h>

#include "TestsSuite.h"


TestsSuite::TestsSuite(int ledPin, int actionPin, int btnPin) {
    LED_PIN = ledPin;
    OPEN_ACTION_PIN = actionPin;
    ACTION_BUTTON_PIN = btnPin;
}


void TestsSuite::startTesting() {

    Serial.println("Avvio del device in modalità test in corso su " + String(ARDUINO_BOARD));
    Serial.println("Test Suite Version: " + String(TEST_SUITE_VERSION)); 

    togglePinForDelay(LED_PIN, HIGH, 500);
    delay(500);
    togglePinForDelay(LED_PIN, HIGH, 500);
    delay(500);
    togglePinForDelay(LED_PIN, HIGH, 500);
    delay(500);

    while(running) {

        static unsigned long previousMillis = 0;
        static unsigned long globalCounter = 0;

        if(millis() - previousMillis >= 10000) {
            previousMillis = millis();
            globalCounter++;
            Serial.println("I'm alive at iteration " + String(globalCounter)); 
        }

        buttonHandler();

        if(continuousTesting) {

            unsigned long testCounter = 0;

            while(true) {
                Serial.println("Ciclo di test numero " + String(testCounter) + ", trascorsi " + String(testCounter * TEST_CICLE_PERIODICITY_MS) + " ms");
                testCounter++;
                testInstallation();
                buttonHandler();
                delay(TEST_CICLE_PERIODICITY_MS);
            }  
        }
    }

}


void TestsSuite::testInstallation() {
  //Blink del LED
  togglePinForDelay(LED_PIN, HIGH, 500);
  delay(500);
  togglePinForDelay(LED_PIN, HIGH, 500);

  //Toggle del rele
  delay(500);
  togglePinForDelay(OPEN_ACTION_PIN, HIGH, 500);
}


void TestsSuite::buttonHandler() {

    static int duration = 0;

    while(digitalRead(ACTION_BUTTON_PIN) == LOW) {
        delay(50);
        duration += 50;
    }

    if(duration >= EXIT_TESTS_SUITE_PRESS_DURATION_MS) {
        Serial.println("Uscita dalla suite di test ed avvio nominale del dispositivo");  
        running = false;
        continuousTesting = false;
        duration = 0;
    }
    if(duration >= CONTINUOUS_CICLE_PRESS_DURATION_MS && !continuousTesting) {
        Serial.println("Avvio del ciclo continuo di test. Verrà eseguito un ciclo ogni " + String(TEST_CICLE_PERIODICITY_MS) + "ms");  
        continuousTesting = !continuousTesting;
        duration = 0;
    }
    else if(duration >= SINGLE_CICLE_PRESS_DURATION_MS && !continuousTesting) {
        Serial.println("Avvio una sequenza di test, controllare la board");  
        testInstallation();
        duration = 0;
    }
    else {
        duration = 0;
    }
}


void TestsSuite::togglePinForDelay(int pin, int action, int delay_ms) {
    digitalWrite(pin, action);
    delay(delay_ms);
    digitalWrite(pin, action == HIGH ? LOW : HIGH);
}
