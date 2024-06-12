
/**
 * Client.java
 * */

import java.rmi.*;
import java.io.*;
import java.util.Arrays;

class RMI_Client {

	public static void main(String[] args) {
		int registryPort = 1099; // Porta di default del registry RMI
		String registryHost = null; // Host del registry RMI, di default localhost
		String serviceName = "ServerRMI"; // Nome del servizio RMI, di default ServerRMI

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
		System.out.println("Invio richieste a " + registryHost + " per il servizio di nome " + serviceName);

		// CONNESSIONE AL SERVIZIO RMI REMOTO
		try {
			String completeName = "//" + registryHost + ":" + registryPort + "/" + serviceName;
			RMI_InterfaceFile serverRMI = (RMI_InterfaceFile) Naming.lookup(completeName);
			System.out.println("Client RMI: Servizio \"" + serviceName + "\" connesso");

			/**
			 * Inizio corpo Client
			 */
			System.out.println("\nRichieste di servizio fino a fine file\n");

			String service;
			System.out.println("Servizio (1= elimina_sci, 2=noleggia_sci), EOF per terminare: ");

			while ((service = stdIn.readLine()) != null) {
				// 1
				if (service.equals("1")) { // gestione servizio 1
					/**
					 * INIZIALIZZO I PARAMETRI DA PASSARE E RICEVERE DAL METODO REMOTO
					 */

					String identificatore;

					int risposta;

					try {
						System.out.println("identificatore? "); // richiesta parametro
						identificatore = stdIn.readLine(); // acquisizione parametro
						/*
						 * eventuali altre richieste di parametri da passare al metodo remoto
						 */
						try {
							risposta = serverRMI.elimina_sci(identificatore);
							System.out.println("esito " + risposta + "!\n");
						} catch (RemoteException re) {
							System.out.println("Errore remoto: " + re.toString());
						}
					} catch (NumberFormatException e) {
						System.out.println("errore di input");
					}
				} // fine servizio 1

				else if (service.equals("2")) { // gestione servizio 2
					/**
					 * INIZIALIZZO I PARAMETRI DA PASSARE E RICEVERE DAL METODO REMOTO
					 */

					int giorno, mese, anno, durata;
					String identificatore;

					int risposta;

					try {
						// richiesta parametro
						System.out.println("identificatore? "); // richiesta parametro
						identificatore = stdIn.readLine(); // acquisizione parametro

						try {
							System.out.println("giorno di inizio noleggio? "); // richiesta parametro
							giorno = Integer.parseInt(stdIn.readLine()); // acquisizione parametro
							if (giorno < 1 || giorno > 30) {
								System.out.println("[ERRORE]: il giorno deve essere compreso tra 1 e 30");
								continue;
							}

							System.out.println("mese di inizio noleggio? "); // richiesta parametro
							mese = Integer.parseInt(stdIn.readLine()); // acquisizione parametro
							if (mese < 1 || mese > 12) {
								System.out.println("[ERRORE]: il mese deve essere compreso tra 1 e 12");
								continue;
							}

							System.out.println("anno di inizio noleggio? "); // richiesta parametro
							anno = Integer.parseInt(stdIn.readLine()); // acquisizione parametro

							System.out.println("durata del noleggio? "); // richiesta parametro
							durata = Integer.parseInt(stdIn.readLine()); // acquisizione parametro

						} catch (NumberFormatException e) {
							System.out.println("[ERRORE]: " + e);
							continue;
						}

						try {
							// chiamata al metodo remoto
							risposta = serverRMI.noleggia_sci(identificatore, giorno, mese, anno, durata);
							System.out.println("eisto " + risposta + "!\n");
						} catch (RemoteException re) {
							System.out.println("Errore remoto: " + re.toString());
						}
					} catch (NumberFormatException e) { // per eventuali parsing dell'input
						System.out.println("errore di input");
					}
				} // fine servizio 2

				else { // servizio non disponibile
					System.out.println("Servizio non disponibile");
				}
				System.out.println("Servizio (1= elimina_sci, 2= noleggia_sci), EOF per terminare: ");
			} // while !EOF

		} catch (

		Exception e) {
			System.err.println("ClientRMI: " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}