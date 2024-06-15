
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
	public int elimina_occorrenze(String fileName) throws RemoteException;

	/**
	 * rinomina metodo 2, con relativi parametri
	 **/
	public String[] lista_file_carattere(String dirName, char carattere, int occorrenze) throws RemoteException;
}