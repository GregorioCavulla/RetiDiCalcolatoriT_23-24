
/**
 * RMI_Server.java
 * 	Implementa i servizi dichiarati in RemOp.
 * */

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.io.*;
import java.util.*;

public class RMI_Server extends UnicastRemoteObject implements RMI_InterfaceFile { // estende UnicastRemoteObject per
																					// rendere l'oggetto remoto,
																					// implementa l'interfaccia
																					// RMI_interfaceFile
	private static Dato dati[];
	private static final int MAX_DATI = 10;

	// Costruttore
	public RMI_Server() throws RemoteException {
		super();
		System.out.println("[DEBUG] inizializzato");
		for (Dato d : dati) {
			System.out.println(d.toString());
		}
	}

	public static void main(String[] args) {
		// INIZIALIZZAZIONE STRUTTURA DATI
		dati = new Dato[MAX_DATI];

		dati[0] = new Dato("AA000AA", "00001", "auto", "AA000AA_img");
		dati[1] = new Dato("BB000BB", "00002", "auto", "BB000BB_img");
		dati[2] = new Dato("EN000NE", "00003", "camper", "EN000NE_img");
		dati[3] = new Dato("FF111FF", "00004", "camper", "FF111FF_img");

		for (int i = 4; i < MAX_DATI; i++) {
			dati[i] = new Dato();
		}

		int registryPort = 1099; // Porta di default del registry
		String registryHost = "localhost"; // Host del registry
		String serviceName = "ServerRMI"; // Nome del servizio

		// Controllo parametri
		if (args.length != 0 && args.length != 1) {
			System.out.println("Sintassi: RMI_Server [registryPort]");
			System.exit(1);
		} else if (args.length == 0) {
			System.out.println("[DEBUG]: Parametri vuoti, utilizzo valori di default");
		}
		if (args.length == 1) {
			System.out.println("[DEBUG]: Parametri passati 1: " + args[0] + "registryPort");
			try {
				registryPort = Integer.parseInt(args[0]);
			} catch (Exception e) {
				System.out.println("Sintassi: RMI_Server [registryPort], registryPort intero");
				System.exit(2);
			}
		}

		// Registrazione del servizio RMI
		String completeName = "//" + registryHost + ":" + registryPort + "/" + serviceName;
		try {
			RMI_Server serverRMI = new RMI_Server();
			Naming.rebind(completeName, serverRMI);
			System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato, porta: " + registryPort + ".");
		} catch (Exception e) {
			System.err.println("Server RMI \"" + serviceName + "\": " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}

	/**
	 * Implementazione dei metodi dichiarati in RMI_interfaceFile
	 */

	@Override
	public int elimina_prenotazione(String targa) throws RemoteException { // Implementazione del metodo 1 dichiarato in
																			// RMI_interfaceFile

		int esito = -1;
		System.out.println("[DEBUG] elimina_prenotazione ");

		File imgFolder;

		for (Dato d : dati) {
			if (d.getTarga().equals(targa)) {
				imgFolder = new File(d.getImgFolder());

				if (!imgFolder.isDirectory() || !imgFolder.canRead() || !imgFolder.exists()) {
					System.out.println("mi sono rotto qui");
					esito = -1;
					break;
				}

				for (File f : imgFolder.listFiles()) {
					f.delete();
				}

				imgFolder.delete();
				d.setTarga("L");
				d.setPatente("0");
				d.setTipo("L");
				d.setImgFolder("L");

				esito = 1;
			}
		}
		for (Dato d : dati) {
			System.out.println(d.toString());
		}

		System.out.println("[DEBUG] elimina_prenotazione: " + esito);
		return esito;
	}

	@Override
	public Dato[] visualizza_prenotazioni(String tipo) throws RemoteException { // Implementazione del metodo dichiarato
																				// in RMI_interfaceFile

		Dato[] temp = new Dato[dati.length]; // Inizializzare l'array con la dimensione adeguata
		int i = 0;

		for (Dato d : dati) {
			if (d.getTipo().equals(tipo)) {
				char[] targa = d.getTarga().toCharArray(); // Correzione del metodo toCharArray

				if ((targa[0] == 'E' && targa[1] >= 'D') || targa[0] > 'E') { // Correzione dell'operatore di confronto
					temp[i] = d;
					i++;
				}
			}
		}

		// Ridimensionare l'array per contenere solo gli elementi effettivamente
		// aggiunti
		Dato[] esito = new Dato[i];
		System.arraycopy(temp, 0, esito, 0, i);

		System.out.println("[DEBUG] visualizza_prenotazioni ");

		for (Dato d : dati) {
			System.out.println(d.toString());
		}

		System.out.println("[DEBUG] visualizza_prenotazioni: " + esito.length);
		return esito; // Restituire l'array ridimensionato
	}

}