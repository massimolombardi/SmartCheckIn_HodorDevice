#ifndef RESTCLIENT_H
#define RESTCLIENT_H
/**
 * @author: Massimo Lombardi
 * @since: 05/01/2021
 * @project: Smart Check-In: Hodor
 * 
 * @brief: Classe per la gestione delle chiamate REST remote
 * 
 *   Versione   Autore      Data       Commenti
 *   --------- -----------  ---------- -----------
 *   1.0       M. Lombardi  05/01/2021 Creazione
 *   1.1	   M. Lombardi  16/01/2021 Introdotta gestione chiamata rest di login
 *   1.2	   M. Lombardi  21/02/2021 Separate logiche invocazione API da client HTTP
 * 
 */

#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

#define MAX_JSON_BUFFER 2048


/**
* Classe per la gestione delle chiamate HTTP di tipo REST
*/
class RestClient {

	private:

		//Gestione configurazione x.509 per il futuro
		//const char* test_client_key = "";   //to verify the client
		//const char* test_client_cert = "";  //to verify the client

		const char* LETS_ENCRIPT_CA_CERTIFICATE= \
												"-----BEGIN CERTIFICATE-----\n"
												"MIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw\n" \
												"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
												"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw\n" \ 
												"WhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\n" \
												"RW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \ 
												"AoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP\n" \
												"R5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx\n" \
												"sxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm\n" \
												"NHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg\n" \
												"Z3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG\n" \
												"/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC\n" \
												"AYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB\n" \
												"Af8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA\n" \
												"FHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw\n" \
												"AoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw\n" \
												"Oi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB\n" \
												"gt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W\n" \
												"PTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl\n" \ 
												"ikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz\n" \
												"CkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm\n" \
												"lJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4\n" \
												"avAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2\n" \
												"yJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O\n" \
												"yK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids\n" \
												"hCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+\n" \
												"HlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv\n" \
												"MldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX\n" \
												"nLRbwHOoq7hHwg==\n" \
												"-----END CERTIFICATE-----\n";

		String API_HOST = "";
		int API_PORT = 0;
		String BASE_API_URL = "";
		String BASE_REQUEST_HEADER = "";

		const String ENDLINE = "\r\n";

		WiFiClientSecure httpsClient;

		/**
		* Gestione della connessione SSL al server
		*/
		bool connect();
		
		/**
		* Parsing dello status code di risposta
		*/		
		int getStatusCode();
		
		/**
		* Funzioen per il parsing del body della response
		*
		* @param doc Documento JSON per il salvataggio del messaggio
		*/		
		bool parseJsonPayload(DynamicJsonDocument* doc);


	public:

		/**
		* Costruttore di default
		*
		* @param hostUrl Nome host del server
		* @param port Porta su cui sono esposti i servizi
		*/
		RestClient(String hostUrl, String port);

		/**
		* Chiamata di tipo GET
		*
		* @param resource Risorsa da invocare
		* @param authToken Token di autenticazione
		* @param response Documento JSON da utilizzare per gestire la risposta
		*/
		int GET(String resource, String authToken, DynamicJsonDocument* response);

		/**
		* Chiamata di tipo POST
		*
		* @param resource Risorsa da invocare
		* @param authToken Token di autenticazione
		* @param payload Payload del messaggio da inviare
		* @param response Documento JSON da utilizzare per gestire la risposta
		*/
		int POST(String resource, String payload, String authToken, DynamicJsonDocument* response);

};

 #endif
