package RMI;

/**
 * ServerImple.java
 * 	Implementa i servizi dichiarati in RemOp.
 * */

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class ServerImpl extends UnicastRemoteObject implements RMI_interfaceFile { // estende UnicastRemoteObject per
																					// rendere l'oggetto remoto,
																					// implementa l'interfaccia
																					// RMI_interfaceFile
	private static Dato dati[];
	private static final int MAX_DATI = 10;

	// Costruttore
	public ServerImpl() throws RemoteException {
		super();
		System.out.println("[DEBUG] inizializzato");
	}

	public static void main(String[] args) {
		// INIZIALIZZAZIONE STRUTTURA DATI
		dati = new Dato[MAX_DATI];
		for (int i = 0; i < MAX_DATI; i++) {
			dati[i] = new Dato();
		}
		dati[1] = new Dato();

		int registryPort = 1099; // Porta di default del registry
		String registryHost = "localhost"; // Host del registry
		String serviceName = "ServerImpl"; // Nome del servizio

		// Controllo parametri
		if (args.length != 0 && args.length != 1) {
			System.out.println("Sintassi: ServerImpl [registryPort]");
			System.exit(1);
		}
		if (args.length == 1) {
			try {
				registryPort = Integer.parseInt(args[0]);
			} catch (Exception e) {
				System.out.println("Sintassi: ServerImpl [registryPort], registryPort intero");
				System.exit(2);
			}
		}

		// Registrazione del servizio RMI
		String completeName = "//" + registryHost + ":" + registryPort + "/" + serviceName;
		try {
			ServerImpl serverRMI = new ServerImpl();
			Naming.rebind(completeName, serverRMI);
			System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato");
		} catch (Exception e) {
			System.err.println("Server RMI \"" + serviceName + "\": " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}

	@Override
	public int elimina_sci(String id) throws RemoteException { // Implementazione del metodo dichiarato in
																// RMI_interfaceFile
		int esito = -1;
		System.out.println("[DEBUG] elimina ");

		if (id == null)
			return esito;

		return esito;
	}

	@Override
	public int noleggia_sci(String id, int giorno, int mese, int anno, int giorniNol) throws RemoteException { // implementazione
																												// del
																												// metodo
																												// dichiarato
																												// in
																												// RMI_interfaceFile
		int esito = -1;
		System.out.println("[DEBUG] noleggia scii");

		return esito;
	}

}