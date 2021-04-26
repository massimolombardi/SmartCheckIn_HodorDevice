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
 *   1.2 	   M. Lombardi  26/04/2021 Gestione API status corretta
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

		/**
		* Costruttore di default
		*
		* @param configuration Puntatore all'oggetto configurazione
		*/  
		APIHandler(Configuration* configuration);

		/**
		* Metodo per la procedura di login sul server delle API per ottenere un JWT
		*/  
		void login();

		/**
		* Metodo per la pubblicazione dello stato
		*
		* @param operative Stato del device
		* @param errorInfo Messaggio di errore
		*/  
		void status(bool operative, String errorCode, String firmwareVersion);

		/**
		* CMetodo per il controllo dell'apertura necessaria
		*
		* @return bool apertura necessaria
		*/  
		bool checkOpen();

		/**
		* Metodo per l'invio conferma apertura
		*
		* @return bool apertura confermata
		*/  
		bool openConfirmation();

};

 #endif
