
/**
 * RMI_Server.java
 * 	Implementa i servizi dichiarati in RemOp.
 * */

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.io.*;
import java.util.*;

public class RMI_Server extends UnicastRemoteObject implements RMI_interfaceFile { // estende UnicastRemoteObject per
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
		// 	dati[i] = new Dato();
		// }
		// dati[1] = new Dato();

		int registryPort = 1099; // Porta di default del registry
		String registryHost = "localhost"; // Host del registry
		String serviceName = "ServerRMI"; // Nome del servizio

		// Controllo parametri
		if (args.length != 0 && args.length != 1) {
			System.out.println("Sintassi: RMI_Server [registryPort]");
			System.exit(1);
		}else if(args.length == 0){
			System.out.println("[DEBUG]: Parametri vuoti, utilizzo valori di default");
		}		
		if (args.length == 1) {
			System.out.println("[DEBUG]: Parametri passati 1: "+args[0]+"registryPort");
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
			System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato, porta: "+ registryPort+".");
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
	public /*<T> metodo 1*/(/*<T> param */) throws RemoteException { // Implementazione del metodo 1 dichiarato in RMI_interfaceFile

		int esito = -1;
		System.out.println("[DEBUG] /*<T> metodo 1*/ ");

		if (id == null){
			return esito;
		}

		return esito;
	}

	@Override
	public  /*<T> metodo 1*/(/*<T> param */) throws RemoteException {  // Implementazione del metodo 2 dichiarato in RMI_interfaceFile
		
		int esito = -1;
		System.out.println("[DEBUG] /*<T> metodo 1*/ ");

		return esito;
	}

}