package RMI;

/**
 * Interfaccia remota di servizio
 */

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_interfaceFile extends Remote {

	/**

	 **/
	public int elimina_sci (String id) throws RemoteException;

	/**
	
	 **/
	public int noleggia_sci (String id, int giorno, int mese, int anno, int giorniNol) throws RemoteException;
}