/*
 * @Author: Massimo Lombardi
 * @Since: 05/01/2021
 * @Project: Smart Check-In: Hodor
 * 
 * @Brief: Implementazione della classe per la gestione delle chiamate REST remote
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   
 */

#include "RestClient.h" 


RestClient::RestClient() {
  
}

void RestClient::testAPI() {

  const char* server = "https://www.smartcheckin.cloud";
  
  httpsClient.setCACert(test_root_ca);
  //client.setCertificate(test_client_key); // for client verification
  //client.setPrivateKey(test_client_cert);  // for client verification

  Serial.println("\nStarting connection to server...");

  if(httpsClient.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    String content = "Hey, just testing a post request.";
    httpsClient.println("POST https://www.smartcheckin.cloud/api/v1/auth/login");
    httpsClient.println("Host: https://www.smartcheckin.cloud:80");
    httpsClient.println("Accept: */*");
    httpsClient.println("Content-Length: " + content.length());
    httpsClient.println("Content-Type: application/x-www-form-urlencoded");
    httpsClient.println();
    httpsClient.println(content);
  }

  while(httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while(httpsClient.available()) {
      char c = httpsClient.read();
      Serial.write(c);
    }

    httpsClient.stop();
  }
}
