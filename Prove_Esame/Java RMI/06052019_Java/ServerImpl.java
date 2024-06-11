
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileNotFoundException;

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

	private static final int MAX_DATI = 10;

	// Costruttore
	public ServerImpl() throws RemoteException {
		super();
		System.out.println("[DEBUG] inizializzato");
	}

	public static void main(String[] args) {

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

	/**
	 * Implementazione dei metodi dichiarati in RMI_interfaceFile
	 */

	@Override
	public String[] lista_file(String dirName) throws RemoteException { // Implementazione del metodo 1 dichiarato in
																		// RMI_interfaceFile

		System.out.println("[DEBUG] lista_file ");

		File dir = new File(dirName); // Creazione di un oggetto File per il direttorio richiesto

		if (!dir.isDirectory() || !dir.exists() || !dir.canRead()) {
			throw new RemoteException("Il direttorio non contiene file");
		}

		int numeroFile = dir.listFiles().length; // Numero di file presenti nel direttorio

		String temp[] = new String[numeroFile]; // Array temporaneo per memorizzare i nomi dei file che soddisfano i
												// requisiti
		String resulString[]; // Array di stringhe che conterrà i nomi dei file che soddisfano i requisiti
		char fileName[]; // Array di caratteri per memorizzare il nome del file

		int consonanti, j = 0;

		for (File f : dir.listFiles()) { // per ogni file nel direttorio, verifico che il nome del file contenga più di
											// 3 consonanti, in tal caso lo memorizzo nell'array temp
			fileName = f.getName().toLowerCase().toCharArray();
			consonanti = 0;

			for (int i = 0; i < fileName.length; i++) {
				if (fileName[i] != 'a' || fileName[i] != 'e' || fileName[i] != 'i' || fileName[i] != 'o'
						|| fileName[i] != 'u') {
					consonanti++;
					if (consonanti > 3) {
						temp[j++] = f.getName();
						break;
					}
				}
			}
		}

		resulString = new String[j];

		for (int i = 0; i < resulString.length; i++) { // Copio i nomi dei file che soddisfano i requisiti nell'array
														// resulString
			resulString[i] = temp[i];
		}

		return resulString;

	}

	@Override
	public int numerazione_righe(String fileName) throws RemoteException { // Implementazione del metodo 2 dichiarato in
																			// RMI_interfaceFile

		int esito = -1; // Valore di ritorno del metodo
		int riga = 0; // Contatore delle righe del file
		File f = new File(fileName); // Creazione di un oggetto File per il file richiesto
		File outFile, inFile; // File di output e file di input per la scrittura e la lettura del file
								// modificato
		FileWriter outFileWriter; // Oggetto FileWriter per la scrittura del file di output
		String line; // Stringa per memorizzare la riga letta dal file
		String outFileName = fileName.substring(0, (fileName.length() - 4)) + "_modified.txt"; // Nome del file di
																								// output
		BufferedReader br;

		System.out.println("[DEBUG] numerazione_righe ");

		if (!f.exists() || !f.isFile() || !f.canRead()) {
			return esito;
		}

		try {
			br = new BufferedReader(new FileReader(f));
			outFile = new File(outFileName);
			outFileWriter = new FileWriter(outFile);
		} catch (FileNotFoundException e) {
			return esito;
		} catch (IOException e) {
			throw new RemoteException(e.toString());
		}

		esito = 0;

		try {
			riga = 0;
			while ((line = br.readLine()) != null) {
				riga++;
				if (riga % 2 != 0) {
					outFileWriter.write(riga + " " + line + "\n");
					esito++;
				}
			}
			br.close();
			outFileWriter.close();

			inFile = new File(fileName);
			outFile.renameTo(inFile);
			inFile.delete();
		} catch (IOException e) {
			throw new RemoteException(e.toString());
		}

		return esito;
	}

}