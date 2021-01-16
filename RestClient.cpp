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
#include <ArduinoJson.h>

RestClient::RestClient() {
	httpsClient = WiFiClientSecure();
}

void RestClient::getToken() {

	const char*  server = "www.smartcheckin.cloud";

	httpsClient.setCACert(test_root_ca);
	Serial.println("Starting connection to server...");
	
	if(!httpsClient.connect(server, 443))
		Serial.println("Connection failed!");
	else {
		
		Serial.println("Connected to server!");

		String content = "{ \"email\": \"alborgo@smartcheckin.cloud\", \"password\": \"4e1w8dip\" }\r\n";
		String requestStr =  String("POST https://smartcheckin.cloud/api/v1/auth/login HTTP/1.1\r\n") + 
								String("Host: www.smartcheckin.cloud:443\r\n") +
								String("authority: smartcheckin.cloud\r\n") +
								String("method: POST\r\n") +
								String("path: /api/v1/auth/login\r\n") +
								String("scheme: https\r\n") +
								String("accept: */*\r\n") +
								String("accept-encoding: gzip, deflate, br\r\n") +
								String("accept-language: it-IT,it;q=0.9,en-US;q=0.8,en;q=0.7,es;q=0.6\r\n") +
								String("content-length: ") + content.length() + String("\r\n") +
								String("content-type: application/json\r\n") +
								String("sec-fetch-dest: empty\r\n") +
								String("sec-fetch-mode: cors\r\n") +
								String("sec-fetch-site: none\r\n") +
								String("user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36\r\n") +
								String("\r\n") + 
								content +
								String("\r\n");
		
		httpsClient.print(requestStr);

		Serial.println("Richiesta inviata: " + content);

		bool firstLine = true;
		while(httpsClient.connected()) {
			String line = httpsClient.readStringUntil('\n');

			if(firstLine) {
				firstLine = false;
				Serial.println(line);
			}
			
			if (line == "\r") {
				break;
			}
		}
		
		//Status code reg exp
		//(HTTP|http)\/(1|2)\.\d \d{3}(.|\\s)+$

		char buffer[2048];
		int i = 0;
		while(httpsClient.available()) {

			while(i == 0 && httpsClient.read() != '{')
				buffer[i] = '{';

			i++;

			if(i >= 2048) {
				Serial.println("Impossibile gestire payloads piu lunghi di 2048 caratteri");
				break;
			}

			buffer[i] = httpsClient.read();
			Serial.write(buffer[i]);
		}

		for(int j=i; j>=0; j--)	
			if(buffer[j] == '}') {
				buffer[j+1] = '\0';
				break;
			}

		DynamicJsonDocument doc(2048);
		deserializeJson(doc, buffer);

		const char* a = doc["token"];

		Serial.println(String(a));

		Serial.println("Fine");
		httpsClient.stop();
	}
}