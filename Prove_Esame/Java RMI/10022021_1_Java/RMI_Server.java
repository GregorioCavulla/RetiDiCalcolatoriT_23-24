
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

		dati[0] = new Dato("A", new int[] { 1, 1, 2021 }, 1, "modello1", 10.0, "foto1.jpg");
		dati[1] = new Dato("B", new int[] { 2, 2, 2021 }, 2, "modello2", 20.0, "foto2.jpg");
		dati[2] = new Dato("C", new int[] { 3, 3, 2021 }, 3, "modello3", 30.0, "foto3.jpg");
		dati[3] = new Dato("D", new int[] { 4, 4, 2021 }, 4, "modello4", 40.0, "foto4.jpg");

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
	public int elimina_sci(String id) throws RemoteException { // Implementazione del metodo 1 dichiarato in
																// RMI_interfaceFile

		int esito = -1;
		System.out.println("[DEBUG] elimina_sci ");
		String fileName;
		File file;

		for (Dato d : dati) {
			if (d.getId().equals(id)) {
				fileName = d.getNomeFileFoto();
				file = new File(fileName);
				if (file.exists()) {
					file.delete();
					esito = 0;
				} else {
					esito = -1;
				}
				d.setId("L");
				d.setData(new int[] { -1, -1, -1 });
				d.setDataString(d.getData()[0] + "/" + d.getData()[1] + "/" + d.getData()[2]);
				d.setGiorni(-1);
				d.setModello("-1");
				d.setCostoGiornaliero(-1);
				d.setNomeFileFoto("L");

				esito = 1;
				break;
			}
		}

		for (Dato d : dati) {
			System.out.println(d.toString());
		}

		return esito;
	}

	@Override
	public int noleggia_sci(String id, int giorno, int mese, int anno, int durata) throws RemoteException { // Implementazione
																											// del
																											// metodo 2
																											// dichiarato
																											// in
																											// RMI_interfaceFile

		System.out.println("[DEBUG] noleggia sci ");
		int esito = -1;
		int[] data = { giorno, mese, anno };

		for (Dato d : dati) {
			if (d.getId().equals("L")) {
				d.setId(id);
				d.setData(data);
				d.setGiorni(durata);
				esito = 1;
				break;
			}
		}

		for (Dato d : dati) {
			System.out.println(d.toString());
		}

		return esito;
	}

}