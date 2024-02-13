
/**
 * RMI_Server.java
 * 	Implementa i servizi dichiarati in RMI_InterfaceFile.
 *  + ricevere la lista dei file di un direttorio remoto i cui nomi contengano 3 o più consonanti
 *  + aggiungere la numerazione delle righe dispari in un file di testo remoto
 * */

import java.io.*;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.*;

public class RMI_Server extends UnicastRemoteObject implements RMI_InterfaceFile {

	// Costruttore
	public RMI_Server() throws RemoteException {
		super();
	}

	public String[] lista_file(String dirName) throws RemoteException {
        File dir = new File(dirName);

        if(!dir.isDirectory() || !dir.exists() || !dir.canRead()){
            throw new RemoteException("Il direttorio passato non esiste o non è un direttorio");
        }

        if(dir.listFiles().length == 0){
            throw new RemoteException("Il direttorio non contiene file");
        } 
        
        int numeroFile = dir.listFiles().length;
        String temp [] = new String[numeroFile];
        String result [];
        char fileName[];

        int consonanti,j=0;

        for(File f:dir.listFiles()){
            fileName = f.getName().toLowerCase().toCharArray();
            consonanti=0;

            for(int i=0;i<fileName.length;i++){
                if(fileName[i]!='a'||fileName[i]!='e'||fileName[i]!='i'||fileName[i]!='o'||fileName[i]!='u'){
                    consonanti++;
                    if(consonanti > 3){
                        temp[j++]=f.getName();
                        break;
                    }
                }
            }
        }

        result = new String[j];
        
        for(int i=0;i<result.length;i++){
            result[i] = temp[i];
        }

        return result;

	}

	public int numerazione_righe(String fileName) throws RemoteException {
		int riga = 0,result=0;
        File f = new File(fileName);
        File out,in;
        FileWriter fileOut;
        String line;
        String outFileName = fileName.substring(0, (fileName.length() - 4)) + "_modified.txt";
        BufferedReader br;

        if(!f.exists()||!f.isFile()||!f.canRead()){
            return -1;
        }

		try {
			br = new BufferedReader(new FileReader(f));
			out = new File(outFileName);
			fileOut = new FileWriter(out);
		} catch (FileNotFoundException e) {
			return -1;
		} catch (IOException e) {
			throw new RemoteException(e.toString());
		}

		try {
			riga = 0;
			while ((line = br.readLine()) != null) {
				riga++;
				if (riga % 2 != 0) {
					fileOut.write(riga + " " + line + "\n");
                    result++;
				}
			}
			br.close();
			fileOut.close();

			// Replace the original file with the tmp
			in = new File(fileName);
			out.renameTo(in);
			in.delete();

		} catch (IOException e) {
			throw new RemoteException(e.toString());
		}
        return result;
	}

	public static void main(String[] args) {

		int registryPort = 1099;
		String registryHost = "localhost";
		String serviceName = "Server";

		// Controllo parametri
		if (args.length != 0 && args.length != 1) {
			System.out.println("Sintassi: RMI_Server [registryPort]");
			System.exit(1);
		}
		if (args.length == 1) {
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
			System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato");
		} catch (Exception e) {
			System.err.println("Server RMI \"" + serviceName + "\": " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}