
/**
 * RMI_Client.java
 *
 */

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;

class RMI_Client {

	public static void main(String[] args) {
		final int REGISTRYPORT = 1099;
		String registryHost = null; // host remoto con registry
		String serviceName = "";
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		// Controllo dei parametri della riga di comando
		if (args.length != 2) {
			System.out.println("Sintassi: RMI_Registry_IP ServiceName");
			System.exit(1);
		}
		registryHost = args[0];
		serviceName = args[1];

		System.out.println("Invio richieste a " + registryHost + " per il servizio di nome " + serviceName);

		// Connessione al servizio RMI remoto
		try {
			String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
			RMI_interfaceFile serverRMI = (RMI_interfaceFile) Naming.lookup(completeName);
			System.out.println("ClientRMI: Servizio \"" + serviceName + "\" connesso");

			System.out.println("\nRichieste di servizio fino a fine file");

			String service;
			System.out.print("Servizio (E=Elimina, V=Visualizza): ");

			/* ciclo accettazione richieste utente */
			while ((service = stdIn.readLine()) != null) {

				if (service.equals("E")) {
					System.out.print("Numero di targa: ");
                    String targa = null;
                    targa = stdIn.readLine();

                    if(serverRMI.elimina_prenotazione(targa)==0){
                        System.out.println(
                            "Eliminazione di " + targa + " avvenuta con successo");
                    }else{
                        System.out.println(
                            "Impossibile eliminare " + targa);
                    }
				} // E=Elimina

				else if (service.equals("V")) {
					System.out.print("Tipo: ");
                    String tipo = null;
                    tipo = stdIn.readLine();
                    try{
                        Prenotazione[] prenotazioni = serverRMI.visualizza_prenotazioni(tipo);
                        if(prenotazioni == null){
                            System.out.println("Non esistono veicoli del tipo specificato");
                        }else{
                            System.out.println("TARGA  |PATENTE |TIPO  | IMMAGINE");
                            for(Prenotazione p:prenotazioni){
                                if(p != null){
                                    System.out.println(p.getTarga() + "|" + p.getPatente() + "   |" + p.getTipo() + "|" + p.getImmagine());
                                }
                            }
                        }
                    }catch(RemoteException r){
                        r.printStackTrace();
                    }
                }// V=Visualizza

				else
					System.out.println("Servizio non disponibile");

				System.out.print("Servizio (E=Elimina, V=Visualizza): ");
			} // while (!EOF), fine richieste utente

		} catch (NotBoundException nbe) {
			System.err.println("ClientRMI: il nome fornito non risulta registrato; " + nbe.getMessage());
			nbe.printStackTrace();
			System.exit(1);
		} catch (Exception e) {
			System.err.println("ClientRMI: " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}