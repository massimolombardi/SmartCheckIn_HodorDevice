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
 *   1.1	   M. Lombardi  16/01/2021 Itrodotta gestione chiamata rest di login
 * 
 */

#include "RestClient.h" 


RestClient::RestClient(Configuration* configuration) {
	cfg = configuration;
	httpsClient = WiFiClientSecure();
	httpsClient.setCACert(LETS_ENCRIPT_CA_CERTIFICATE);

	activeSession = false;
	
	BASE_REQUEST_HEADER = String("Host: www.smartcheckin.cloud:443") + ENDLINE +
							String("authority: smartcheckin.cloud") + ENDLINE +
							String("path: /api/v1/auth/login") + ENDLINE +
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


void RestClient::login() {

	if(connect() && !activeSession) {
		
		//String content = String("{ \"email\": \"") + cfg->getLoginUsername() + String("\", \"password\": \"") +  cfg->getLoginPassword() + "\" }";
		String content = "{ \"email\": \"alborgo@smartcheckin.cloud\", \"password\": \"4e1w8dip\" }";
		
		POST(LOGIN_RESOUCE, content, false);

		Serial.println("Richiesta inviata: " + content);

		int httpStatusCode = getStatusCode();	

		if(httpStatusCode == 200) {
			
			DynamicJsonDocument doc(MAX_JSON_BUFFER);
			if(parseJsonPayload(&doc)) {

				if(strcmp(doc["status"], "success") == 0) {					
					const char* tmpToken = doc["token"];
					authToken = String(tmpToken);
					activeSession = true;
					Serial.println("Ricevuto Token di Autenticazione");
				}
				else {
					Serial.println("Ricevuto status error dalla chiamata di login");
				}
			}
			else {
				Serial.println("Errore durante la deserializzazione del payload");
			}
		}
		else {
			Serial.println("Ricevuto HTTP Status Code dalla risorsa STATUS: " + String(httpStatusCode));
		}

		httpsClient.stop();
		Serial.println("Connessione con il server di login terminata");
	}
}


void RestClient::status() {

	if(!activeSession && connect()) {
		
		String content = "{ \"idDevice\": \"1\", \"operative\": \"ok\", \"softwareVersionNumber\": \"0.4\", \"errorInfo\": \"running\" }";

		POST(STATUS_RESOUCE, content, true);

		Serial.println("Richiesta inviata: " + content);

		int httpStatusCode = getStatusCode();	

		if(httpStatusCode == 401) {
			activeSession = false;
		}
			
		Serial.println("Ricevuto HTTP Status Code dalla risorsa STATUS: " + String(httpStatusCode));

		httpsClient.stop();
		Serial.println("Connessione con il server di login terminata");
	}
}


void RestClient::POST(String resource, String payload, bool auth) {

	String request = String("POST ") + BASE_API_URL + API_VERSION + resource +  " HTTP/1.1" + ENDLINE + 
						BASE_REQUEST_HEADER +
						String("method: POST") + ENDLINE +
						String("content-length: ") + payload.length() + ENDLINE;

	if(auth)
		request += String("Authorization: Bearer ") + authToken + ENDLINE;
	
	request = ENDLINE + payload + ENDLINE + ENDLINE;

	httpsClient.print(request);
}


bool RestClient::connect() {

	bool result = httpsClient.connect(API_HOST, 443);

	if(!result)
		Serial.println("Impossibile connettersi al server specificato");
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