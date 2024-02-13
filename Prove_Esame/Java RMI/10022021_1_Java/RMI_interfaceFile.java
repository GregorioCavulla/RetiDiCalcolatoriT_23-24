/*
 * Interfaccia remota di servizio
 */

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_interfaceFile extends Remote{

    int elimina_sci(String identificatore) throws RemoteException;

    int noleggia_sci(String identificatore,int giorno,int mese,int anno,int durata) throws RemoteException;

}