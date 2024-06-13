
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

		dati[0] = new Dato("Stanza0", "SM", "Qui", "Pluto", "Paperino", "L", "L");
		dati[1] = new Dato("Stanza1", "P", "Pippo", "L", "Paperino", "Topolino", "L");
		dati[2] = new Dato("Stanza2", "SP", "Qui", "Quo", "Qua", "L", "Paperino");
		dati[3] = new Dato("Stanza3", "M", "Quo", "Pluto", "L", "Topolino", "L");

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
	public boolean aggiungi_stanza(String nomeStanza, char tipoComunicazione) throws RemoteException { // Implementazione
																										// del metodo 1
																										// dichiarato in
																										// RMI_interfaceFile

		boolean esito = false;
		boolean piena = false;
		System.out.println("[DEBUG] aggiungi_stanza ");

		for (Dato d : dati) {
			if (d.getNome().equals("L")) {
				piena = false;
				break;
			} else {
				piena = true;
			}
		}

		for (Dato d : dati) {
			if (!d.getNome().equals(nomeStanza)) {
				if (d.getNome().equals("L")) {
					d.setNome(nomeStanza);
					d.setStato(tipoComunicazione + "");
					d.setUtente1("L");
					d.setUtente2("L");
					d.setUtente3("L");
					d.setUtente4("L");
					d.setUtente5("L");
					esito = true;
					break;
				}
			} else {
				System.out.println("Stanza già presente");
				return esito;
			}
		}

		for (Dato d : dati) {
			System.out.println(d.toString());
		}

		return esito;
	}

	@Override
	public Dato[] elimina_utente(String nomeUtente) throws RemoteException { // Implementazione del metodo 2 dichiarato
																				// in RMI_interfaceFile

		Dato[] temp = new Dato[dati.length];
		int i = 0;
		System.out.println("[DEBUG] elimina_utente");

		for (Dato d : dati) {
			boolean modified = false;
			if (d.getUtente1().equals(nomeUtente)) {
				d.setUtente1("L");
				modified = true;
			}
			if (d.getUtente2().equals(nomeUtente)) {
				d.setUtente2("L");
				modified = true;
			}
			if (d.getUtente3().equals(nomeUtente)) {
				d.setUtente3("L");
				modified = true;
			}
			if (d.getUtente4().equals(nomeUtente)) {
				d.setUtente4("L");
				modified = true;
			}
			if (d.getUtente5().equals(nomeUtente)) {
				d.setUtente5("L");
				modified = true;
			}
			if (modified) {
				temp[i++] = d;
			}
		}

		Dato[] esito = new Dato[i];
		System.arraycopy(temp, 0, esito, 0, i);

		for (Dato d : dati) {
			System.out.println(d.toString());
		}

		return i == 0 ? null : esito;
	}

}