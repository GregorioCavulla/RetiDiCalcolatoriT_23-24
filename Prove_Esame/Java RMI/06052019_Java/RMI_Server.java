
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
	// private static Dato dati[];
	// private static final int MAX_DATI = 10;

	// Costruttore
	public RMI_Server() throws RemoteException {
		super();
		// System.out.println("[DEBUG] inizializzato");
	}

	public static void main(String[] args) {
		// INIZIALIZZAZIONE STRUTTURA DATI
		// dati = new Dato[MAX_DATI];
		// for (int i = 0; i < MAX_DATI; i++) {
		// dati[i] = new Dato();
		// }
		// dati[1] = new Dato();

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
	public String[] lista_file(String dirName) throws RemoteException { // Implementazione del metodo 1 dichiarato in
																		// RMI_interfaceFile

		// @Override
		// public String[] lista_file(String dirName) throws RemoteException {
		// Debug message
		System.out.println("[DEBUG] lista_file ");

		// Verify directory exists and is readable
		File dir = new File(dirName);
		if (!dir.exists()) {
			System.out.println("[DEBUG] Directory non esistente");
			throw new RemoteException("Directory non esistente");
		}

		if (!dir.isDirectory()) {
			System.out.println("[DEBUG] Non è una directory");
			throw new RemoteException("Non è una directory");
		}

		if (!dir.canRead()) {
			System.out.println("[DEBUG] Directory non leggibile");
			throw new RemoteException("Directory non leggibile");
		}

		// List files and filter out those containing fewer than 3 consonants
		String[] allFiles = dir.list();
		if (allFiles == null) {
			return new String[0];
		}

		// First pass: Count files that meet the criteria
		int count = 0;
		for (String fileName : allFiles) {
			int consonantCount = 0;
			for (char c : fileName.toCharArray()) {
				if (Character.isLetter(c) && "aeiouAEIOU".indexOf(c) == -1) {
					consonantCount++;
				}
				if (consonantCount >= 3) {
					count++;
					break;
				}
			}
		}

		// Create array with appropriate size
		String[] filteredFiles = new String[count];

		// Second pass: Add files that meet the criteria to the array
		int index = 0;
		for (String fileName : allFiles) {
			int consonantCount = 0;
			for (char c : fileName.toCharArray()) {
				if (Character.isLetter(c) && "aeiouAEIOU".indexOf(c) == -1) {
					consonantCount++;
				}
				if (consonantCount >= 3) {
					System.out.println("[DEBUG] prendo File: " + fileName);
					filteredFiles[index++] = fileName;
					break;
				}
			}
		}

		return filteredFiles;
	}

	// String[] esito = "";
	// int i = 0;

	// File dir = new File(dirName);
	// File file;
	// System.out.println("[DEBUG] lista_file ");

	// if (!dir.exists()) {
	// System.out.println("[DEBUG] Directory non esistente");
	// throw new RemoteException("Directory non esistente");
	// }

	// if (!dir.isDirectory()) {
	// System.out.println("[DEBUG] Non è una directory");
	// throw new RemoteException("Non è una directory");
	// }

	// if (!dir.canRead()) {
	// System.out.println("[DEBUG] Directory non leggibile");
	// throw new RemoteException("Directory non leggibile");
	// }

	// for (String fileName : dir.list()) {
	// file = new File(dirName + "/" + fileName);
	// System.out.println("[DEBUG] File: " + fileName);
	// if (file.isFile() || file.isDirectory()) {
	// for (char c : fileName.toCharArray()) {
	// if (c != 'a' && c != 'A' && c != 'e' && c != 'E' && c != 'i' && c != 'I' && c
	// != 'o' && c != 'O'
	// && c != 'u' && c != 'U') {
	// System.out.println("[DEBUG] prendo File: " + fileName);
	// esito[i++] = fileName;
	// }
	// }
	// }
	// }

	// return esito;
	// }

	@Override
	public int numerazione_righe(String fileName) throws RemoteException {
		int esito = -1;
		System.out.println("[DEBUG] numerazione_righe");

		File file = new File(fileName);
		String line;
		File tempFile = new File(file.getAbsolutePath() + ".tmp");

		int i = 1;

		if (!file.exists() || !file.isFile() || !file.canRead()) {
			System.out.println("[DEBUG] File non esistente");
			return esito;
		}

		esito = 0;
		String newLine;

		try {
			BufferedReader br = new BufferedReader(new FileReader(file));
			BufferedWriter bw = new BufferedWriter(new FileWriter(tempFile));
			while ((line = br.readLine()) != null) {
				if (i % 2 != 0) { // stampa solo le righe dispari
					System.out.println("[DEBUG] riga dispari : " + line);
					newLine = i + " " + line;
					bw.write(newLine);
					bw.newLine();
					esito++; // incrementa il contatore per le righe numerate
				} else {
					bw.write(line);
					bw.newLine();
				}
				i++;
			}
			br.close();
			bw.close();

			// Sovrascrivi il file originale con il file temporaneo
			if (!file.delete()) {
				System.out.println("[DEBUG] Errore durante l'eliminazione del file originale");
				esito = -1;
			} else if (!tempFile.renameTo(file)) {
				System.out.println("[DEBUG] Errore durante la rinominazione del file temporaneo");
				esito = -1;
			}
		} catch (IOException e) {
			System.out.println("[DEBUG] Errore di I/O");
			esito = -1;
		}

		return esito;
	}

}