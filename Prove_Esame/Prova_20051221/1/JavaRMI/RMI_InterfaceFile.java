
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
	public boolean aggiungi_stanza(String nomeStanza, char tipoComunicazione) throws RemoteException;

	/**
	 * rinomina metodo 2, con relativi parametri
	 **/
	public Dato[] elimina_utente(String nomeUtente) throws RemoteException;
}