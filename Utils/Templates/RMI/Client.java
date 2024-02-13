package RMI;

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
        

		//CONTROLLO I PARAMETRI
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
			String completeName = "//" + registryHost + ":" + registryPort + "/"	+ serviceName;
			RMI_interfaceFile serverRMI = (RMI_interfaceFile) Naming.lookup(completeName);
			System.out.println("Client RMI: Servizio \"" + serviceName + "\" connesso");

			System.out.println("\nRichieste di servizio fino a fine file");
			String service;
			System.out.println("Servizio (N=, E=): ");
			while ((service = stdIn.readLine()) != null) {
				//N
				if (service.equals("N")) {
                    try{
                        System.out.println("id? ");
                        String id = stdIn.readLine();
                        System.out.println("giorno? ");
                        int giorno= Integer.parseInt(stdIn.readLine());
                        System.out.println("mese? ");
                        int mese= Integer.parseInt(stdIn.readLine());
                        System.out.println("anno? ");
                        int anno= Integer.parseInt(stdIn.readLine());
                        System.out.println("quanti giorni? ");
                        int durata= Integer.parseInt(stdIn.readLine());
                        try {
                            int esito = serverRMI.noleggia_sci(id,giorno,mese,anno,durata);
                            System.out.println("esito "+esito);
                        } catch (RemoteException re) {
                            System.out.println("Errore remoto: " + re.toString());
                        }
                    }catch(NumberFormatException e){
                        System.out.println("errore di input");
                    }
				} 
				//E
				else
					if (service.equals("E")) {

						String id;
						System.out.println("id? ");
						id = stdIn.readLine();


						int res = 0;
						try {
							res = serverRMI.elimina_sci(id);
							System.out.println("eisto "+res+"!\n");
						} catch (RemoteException re) {
							System.out.println("Errore remoto: " + re.toString());
						}
					} 

					//NIENTE
					else System.out.println("Servizio non disponibile");

				System.out.println("Servizio (N=   , E=   ): ");
			} // !EOF

		} catch (Exception e) {
			System.err.println("ClientRMI: " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}