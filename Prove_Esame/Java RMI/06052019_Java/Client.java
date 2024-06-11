
/**
 * Client.java
 * */

import java.rmi.*;
import java.io.*;

class Client {

	public static void main(String[] args) {
		int registryPort = 1099;
		String registryHost = null;
		String serviceName = "ServerRMI";
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		// CONTROLLO I PARAMETRI
		if (args.length != 1 && args.length != 2) {
			System.out.println("[ERROR]: Client RegistryHost [registryPort]");
			System.exit(1);
		} else {
			registryHost = args[0];
			if (args.length == 2) {
				try {
					registryPort = Integer.parseInt(args[1]);
				} catch (Exception e) {
					System.out
							.println("[ERROR]: Client NomeHost [registryPort], registryPort intero");
					System.exit(1);
				}
			}
		}
		System.out.println("[DEBUG] check Parametri passato");

		// CONNESSIONE AL SERVIZIO RMI REMOTO
		try {
			String completeName = "//" + registryHost + ":" + registryPort + "/" + serviceName;
			RMI_interfaceFile serverRMI = (RMI_interfaceFile) Naming.lookup(completeName);
			System.out.println("Client RMI: Servizio \"" + serviceName + "\" connesso");

			/**
			 * Inizio corpo Client
			 */
			System.out.println("\nRichieste di servizio fino a fine file");
			String service;
			System.out.println("Servizio (1= lista_file , 2= numerazione?righe ), EOF per terminare: ");
			while ((service = stdIn.readLine()) != null) {
				// 1
				if (service.equals("1")) { // gestione servizio 1
					try {
						System.out.println("dirName? "); //richiesta parametro
						String dirName = stdIn.readLine(); //acquisizione parametro
						/*
						 * eventuali altre richieste di parametri da passare al metodo remoto
						 */
						try {
							int esito = serverRMI.lista_file(dirName);
							System.out.println("esito " + esito);
						} catch (RemoteException re) {
							System.out.println("Errore remoto: " + re.toString());
						}
					} catch (NumberFormatException e) {
						System.out.println("errore di input");
					}
				}
				// 2
				else if (service.equals("2")) { // gestione servizio 2

					System.out.println("/*param*/? "); //richiesta parametro
					String /*param*/ = stdIn.readLine(); //acquisizione parametro

					int res = 0;
					try {
						res = serverRMI./*metodo 2*/(id);
						System.out.println("eisto " + res + "!\n");
					} catch (RemoteException re) {
						System.out.println("Errore remoto: " + re.toString());
					}
				}
				else{ //servizio non disponibile
					System.out.println("Servizio non disponibile");
				}
					System.out.println("Servizio (1= lista_file , 2= numerazione?righe ), EOF per terminare: ");
				} // !EOF

		} catch (Exception e) {
			System.err.println("ClientRMI: " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}