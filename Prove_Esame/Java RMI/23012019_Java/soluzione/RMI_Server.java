/**
 * RMI_Server.java
 * 	Implementa i servizi dichiarati in RMI_InterfaceFile.
 *  + eliminare una prenotazione
 *  + visualizzare tutte le prenotazioni di un certo tipo di veicolo (camper o auto) dopo una certa targa(ED)
 * */

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.io.File;

public class RMI_Server extends UnicastRemoteObject implements RMI_interfaceFile {
    private static final int MAX = 10;
	private static Prenotazione[] prenotazioni;

	// Costruttore
	public RMI_Server() throws RemoteException {
		super();
	}

	// Elimino una prenotazione
	public int elimina_prenotazione(String targa) throws RemoteException {
		System.out.println("Server RMI: richiesta eliminazione della targa = " + targa);

        File immagine = null;

        for(int i=0;i<MAX;i++){
            if(prenotazioni[i].getTarga().equals(targa)){
                prenotazioni[i] = new Prenotazione();
                immagine = new File(prenotazioni[i].getImmagine());
                if(immagine.exists()){
                    immagine.delete();
                }
                return 0;
            }
        }

        return -1;
	}

	// Visualizzo tutti i veicoli dopo il 2011 (ED) di un solo tipo 
	public Prenotazione[] visualizza_prenotazioni(String tipo) throws RemoteException {

        if(!tipo.equals("camper") && !tipo.equals("auto")){
            throw new RemoteException("invalid tipo veicolo"); 
        }

        Prenotazione[] result = new Prenotazione[MAX];
        String targa = null;
        int i=0;

        for(Prenotazione p:prenotazioni){
            targa = p.getTarga();
            System.out.println(targa);

            if(p.getTipo().equals(tipo)){
                if(targa.charAt(0) > 'E' || (targa.charAt(0) == 'E' && targa.charAt(1) > 'D')){
                    result[i] = p;
                    System.out.println(p.getTarga() + "|" + p.getPatente() + "|" + p.getTipo() + "|" + p.getImmagine());
                }
            }

        }

        return result;
	}

	// Avvio del Server RMI
	public static void main(String[] args) {

        prenotazioni = new Prenotazione[MAX];
        prenotazioni[0] = new Prenotazione("AN745NL","00254","auto","AN745NL_img/");
        prenotazioni[1] = new Prenotazione("DG547BV","65861","auto","DG547BV_img/");
        prenotazioni[2] = new Prenotazione("EC123IU","69713","camper","EC123IU_img/");
        prenotazioni[3] = new Prenotazione("EY211LA","25486","camper","EY211LA_img/");
        prenotazioni[4] = new Prenotazione("GB456IK","36578","auto","GB456IK_img/");
        prenotazioni[5] = new Prenotazione("ED698OK","46857","camper","ED698OK_img/");
        prenotazioni[6] = new Prenotazione();
        prenotazioni[7] = new Prenotazione();
        prenotazioni[8] = new Prenotazione();
        prenotazioni[9] = new Prenotazione();

		final int REGISTRYPORT = 1099;
		String registryHost = "localhost";
		String serviceName = "RMI_interfaceFile"; // lookup name...

		// Registrazione del servizio RMI
		String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
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