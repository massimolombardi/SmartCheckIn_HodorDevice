/**
 * @author: Massimo Lombardi
 * @since: 05/01/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Implementazione della classe per la gestione delle chiamate REST remote
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   1.1	   M. Lombardi  16/01/2021 Introdotta gestione chiamata rest di login
 *   1.2	   M. Lombardi  21/02/2021 Separate logiche invocazione API da client HTTP
 * 
 */

#include "RestClient.h" 


RestClient::RestClient(String hostUrl, String port) {

	httpsClient = WiFiClientSecure();
	httpsClient.setCACert(LETS_ENCRIPT_CA_CERTIFICATE);
	
	API_PORT = atoi(port.c_str());
	API_HOST = hostUrl;
	BASE_API_URL = String("https://") + API_HOST + String(":") + port;

	BASE_REQUEST_HEADER = String("Host: ") + hostUrl + ENDLINE +
							String("authority: ") + hostUrl + ENDLINE +
							String("scheme: https") + ENDLINE +
							String("accept: */*") + ENDLINE +
							String("accept-encoding: gzip, deflate, br") + ENDLINE +
							String("accept-language: it-IT,it;q=0.9,en-US;q=0.8,en;q=0.7,es;q=0.6") + ENDLINE +
							String("content-type: application/json") + ENDLINE +
							String("sec-fetch-dest: empty") + ENDLINE +
							String("sec-fetch-mode: cors") + ENDLINE +
							String("sec-fetch-site: none") + ENDLINE +
							String("user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36") + ENDLINE;
}


int RestClient::POST(String resource, String payload, String authToken, DynamicJsonDocument* response) {

	if(!connect())
		return 503;

	String request = String("POST ") + BASE_API_URL + resource +  " HTTP/1.1" + ENDLINE + 
						BASE_REQUEST_HEADER +
						String("path: ") + String(resource) + ENDLINE +
						String("method: POST") + ENDLINE +
						String("content-length: ") + payload.length() + ENDLINE;

	if(authToken != NULL)
		request += String("Authorization: Bearer ") + authToken + ENDLINE;
	
	request += ENDLINE + payload + ENDLINE + ENDLINE;

	httpsClient.print(request);
	
	int httpStatusCode = getStatusCode();
	Serial.println("Ricevuto status code: " + String(httpStatusCode));

	if(httpStatusCode == 200) {
		if(!parseJsonPayload(response)) {				
			Serial.println("Errore durante la deserializzazione del payload");
		}
	}

	httpsClient.stop();
	return httpStatusCode;
}


int RestClient::GET(String resource, String authToken, DynamicJsonDocument* response) {

	if(!connect())
		return 503;

	String request = String("GET ") + BASE_API_URL + resource +  " HTTP/1.1" + ENDLINE + 
						BASE_REQUEST_HEADER +
						String("path: ") + String(resource) + ENDLINE +
						String("method: GET") + ENDLINE;

	if(authToken != NULL)
		request += String("Authorization: Bearer ") + authToken + ENDLINE + ENDLINE;

	httpsClient.print(request);

	int httpStatusCode = getStatusCode();
	Serial.println("Ricevuto status code: " + String(httpStatusCode));

	if(httpStatusCode == 200) {
		if(!parseJsonPayload(response)) {				
			Serial.println("Errore durante la deserializzazione del payload");
		}
	}

	httpsClient.stop();
	return httpStatusCode;
}


bool RestClient::connect() {

	bool result = httpsClient.connect(API_HOST.c_str(), API_PORT);

	if(!result)
		Serial.println("Impossibile connettersi al server " + API_HOST + ":" + String(API_PORT));
	else	
		Serial.println("Connessione SSL stabilita con successo");

	return result;
}


int RestClient::getStatusCode() {

	bool firstLine = true;
	String statusCodeLine;

	while(httpsClient.connected()) {
		String line = httpsClient.readStringUntil('\n');

		if(firstLine) {
			firstLine = false;
			statusCodeLine = line;
		}
		
		if (line == "\r") {
			break;
		}
	}

	//Status code reg exp
	//(HTTP|http)\/(1|2)\.\d \d{3}(.|\\s)+$
	int statusCode = 0;
	char* tmpString = NULL;

	char buffer[32];
	char* p = buffer;
	statusCodeLine.toCharArray(buffer, sizeof(buffer));

	while((tmpString = strtok_r(p, " ", &p)) != NULL) {
		statusCode = atoi(tmpString);
		if(statusCode > 0)
			return statusCode;
	}
  
	return 0;
}


bool RestClient::parseJsonPayload(DynamicJsonDocument* doc) {

		char buffer[MAX_JSON_BUFFER];
		int i = 0;
		while(httpsClient.available()) {

			while(i == 0 && httpsClient.read() != '{')
				buffer[i] = '{';

			i++;

			if(i >= MAX_JSON_BUFFER) {
				Serial.println("Impossibile gestire payloads piu lunghi di " + String(MAX_JSON_BUFFER) + " caratteri");
				break;
			}

			buffer[i] = httpsClient.read();
			//Serial.write(buffer[i]);
		}

		for(int j=i; j>=0; j--)	{
			if(buffer[j] == '}') {
				buffer[j+1] = '\0';
				break;
			}
		}

		DeserializationError error = deserializeJson(*doc, buffer);
		if(error) {
			Serial.print(F("Errore durante la deserializzazione del JSON ricevuto: "));
			Serial.println(error.c_str());
			return false;
		}

		return true;		
}