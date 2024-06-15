
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
		// for (Dato d : dati) {
		// System.out.println(d.toString());
		// }
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
	public int elimina_occorrenze(String fileName) throws RemoteException { // Implementazione del metodo 1 dichiarato
																			// in RMI_interfaceFile

		int esito = -1;
		System.out.println("[DEBUG] elimina_occorrenze ");

		File file = new File(fileName);

		File tempFile = new File(file.getName() + "_temp.txt");

		if (!file.isFile() || !file.exists() || !file.canRead()) {
			return esito;
		}

		esito = 0;

		String line, newLine = "";

		try {

			BufferedReader br = new BufferedReader(new FileReader(file));
			BufferedWriter bw = new BufferedWriter(new FileWriter(tempFile));
			while ((line = br.readLine()) != null) {
				System.out.println("[DEBUG]: letta riga: " + line);
				for (char c : line.toCharArray()) {
					if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7'
							&& c != '8' && c != '9') {
						newLine += c;
						esito++;
					}
				}
				bw.write(newLine);
				bw.newLine();
				newLine = "";
			}
			br.close();
			bw.close();

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

	@Override
	public String[] lista_file_carattere(String dirName, char carattere, int occorrenze) throws RemoteException { // Implementazione
																													// del
																													// metodo
																													// 2
																													// dichiarato
																													// in
																													// RMI_interfaceFile

		System.out.println("[DEBUG] lista_file_carattere ");

		File dir = new File(dirName);

		if (!dir.exists() || !dir.isDirectory() || !dir.canRead()) {
			throw new RemoteException("[ERRORE]: directory inesistente");
		}

		String fileName;
		int occTrovate = 0, count = 0;

		System.out.println("[DEBUG]: Stampo il tree di dir: \n");
		for (File f : dir.listFiles()) {
			if (f.isFile()) {
				System.out.println(" file: " + f.getName());
			}
		}

		for (File f : dir.listFiles()) {
			if (f.isFile()) {
				fileName = f.getName();
				for (char c : fileName.toCharArray()) {
					if (c == carattere) {
						occTrovate++;
					}
					if (occTrovate == occorrenze) {
						count++;
						break;
					}
				}
			}
		}

		System.out.println("[DEBUG]: ho trovato " + count + "file compliant");

		String[] esito = new String[count];
		int i = 0;
		occTrovate = 0;

		for (File f : dir.listFiles()) {
			if (f.isFile()) {
				fileName = f.getName();
				for (char c : fileName.toCharArray()) {
					if (c == carattere) {
						occTrovate++;
					}
					if (occTrovate == occorrenze) {
						esito[i] = fileName;
						i++;
						break;
					}
					if (i == count) {
						break;
					}
				}
			}
		}

		System.out.println("[DEBUG]: restituisco esito: \n");
		for (String s : esito) {
			System.out.println(" file: " + s);
		}

		return esito;
	}

}