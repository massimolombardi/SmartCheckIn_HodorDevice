/**
 * @author: Massimo Lombardi
 * @since: 05/01/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Implementazione della classe per la gestione delle API di gestione device esposte dal server
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   1.1	   M. Lombardi  16/01/2021 Introdotta gestione chiamata rest di login
 * 
 */

#include "APIHandler.h" 


APIHandler::APIHandler(Configuration* configuration) {
	activeSession = false;
	cfg = configuration;
	restClient = new RestClient(cfg->getAPIHost(), cfg->getAPIPort());
}


void APIHandler::login() {

	if(!activeSession) {
		
		//String content = String("{ \"email\": \"") + cfg->getLoginUsername() + String("\", \"password\": \"") +  cfg->getLoginPassword() + "\" }";
		String content = "{ \"username\": \"Test\", \"password\": \"test\", \"deviceId\": \"test\" }";
		
		DynamicJsonDocument doc(MAX_JSON_BUFFER);
		int httpStatus = restClient->POST(LOGIN_RESOUCE, content, "", &doc);

		if(httpStatus == 200) {	

			const char* tmpToken = doc["token"];
			authToken = String(tmpToken);

			if(authToken != NULL && authToken != "") {
				activeSession = true;
				Serial.println("Ricevuto Token di Autenticazione");
			}
			else {
				activeSession = false;
				Serial.println("Ricevuto Token di Autenticazione non valido");
			}

		}
		else {
			Serial.println("Ricevuto HTTP Status Code dalla risorsa STATUS: " + String(httpStatus));
		}
	}
}


void APIHandler::status(bool operative, String errorInfo) {

	if(activeSession) {
		
		String content = String("{ \"operative\": ") + operative ? String("true") : String("false") + String(",\"softwareVersionNumber\": \"0.4\", \"errorInfo\": \"") + errorInfo + String("\" }");

		DynamicJsonDocument doc(MAX_JSON_BUFFER);
		int httpStatus = restClient->POST(STATUS_RESOUCE, content, authToken, &doc);

		if(httpStatus == 200) {	
			Serial.println("Stato del device inviato");
		}
		else if(httpStatus == 401) {
			activeSession = false;
		}
		else {
			Serial.println("Ricevuto HTTP Status Code dalla risorsa STATUS: " + String(httpStatus));
		}	
	}
	else {
		Serial.println("Tentativo di autenticazione in corso...");
		login();
	}
}


bool APIHandler::checkOpen() {

	if(activeSession) {

		DynamicJsonDocument doc(MAX_JSON_BUFFER);
		int httpStatus = restClient->GET(CHECK_OPEN_RESOUCE, authToken, &doc);

		if(httpStatus == 200) {	
			Serial.println("Richiesta di controllo apertura inviata");
			bool openRequired = doc["openRequired"];
			Serial.println("Ricevuto comando di apertura");
			return true;
		}
		else if(httpStatus == 401) {
			activeSession = false;
		}
		else {
			Serial.println("Ricevuto HTTP Status Code dalla risorsa STATUS: " + String(httpStatus));
		}
	}
	else {
		Serial.println("Tentativo di autenticazione in corso...");
		login();
	}

	return false;
}