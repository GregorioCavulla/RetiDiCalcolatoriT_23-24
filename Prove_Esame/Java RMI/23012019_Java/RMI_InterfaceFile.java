
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
	public int elimina_prenotazione(String targa) throws RemoteException;

	/**
	 * rinomina metodo 2, con relativi parametri
	 **/
	public Dato[] visualizza_prenotazioni(String tipo) throws RemoteException;
}