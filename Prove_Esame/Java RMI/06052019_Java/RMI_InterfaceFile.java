/**
 * Interfaccia remota di servizio
 */

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_InterfaceFile extends Remote {

	public String[] lista_file (String dirName) throws RemoteException;

	public int numerazione_righe (String fileName) throws RemoteException;

}