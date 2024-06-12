
/**
 * Interfaccia remota di servizio
 * definisce i metodi che possono essere invocati da remoto
 */

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_InterfaceFile extends Remote {

	/**
	 * rinomina metodo 1, con relativi parametri
	 **/
	public int elimina_sci(String id) throws RemoteException;

	/**
	 * rinomina metodo 2, con relativi parametri
	 **/
	public int noleggia_sci(String id, int giorno, int mese, int anno, int durata) throws RemoteException;
}