#ifndef APIHANDLER_H
#define APIHANDLER_H
/**
 * @author: Massimo Lombardi
 * @since: 05/01/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Classe per la gestione delle API di gestione device esposte dal server
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   1.1	   M. Lombardi  16/01/2021 Introdotta gestione chiamata rest di login
 * 
 */

#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "src/Configuration.h"
#include "src/Http/RestClient.h"

#define MAX_JSON_BUFFER 2048


class APIHandler {

	private:

		const String ENDLINE = "\r\n";

		String API_HOST = "";
		String API_PORT = "";

		String LOGIN_RESOUCE = "/auth/login";
		String STATUS_RESOUCE = "/devices/status";
		String CHECK_OPEN_RESOUCE = "/actions/open";
		String CHECK_OPEN_CONFIRMATION_RESOUCE = "/actions/open/confirmed";

		RestClient* restClient;
		bool activeSession;
		String authToken = "";

		//Putantore all'Oggetto Configurazione
		Configuration* cfg;


	public:

		APIHandler(Configuration* configuration);
		void login();
		void status(bool operative, String errorInfo);
		bool checkOpen();
		bool openConfirmation();

};

 #endif
