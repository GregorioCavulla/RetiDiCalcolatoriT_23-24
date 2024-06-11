
/**
 * Interfaccia remota di servizio
 * definisce i metodi che possono essere invocati da remoto
 */

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_interfaceFile extends Remote {

	/**
	 * rinomina metodo 1, con relativi parametri
	 **/
	public /*<T> questo*/(/*<T> questo*/) throws RemoteException;

	/**
	 * rinomina metodo 2, con relativi parametri
	 **/
	public /*<T> questo*/(/*<T> questo*/) throws RemoteException;
}