/**
 * Interfaccia remota di servizio
 */

 import java.rmi.Remote;
 import java.rmi.RemoteException;
 
 public interface RMI_interfacciaFile extends Remote {
 
     public String[] lista_filetesto (String dirName) throws RemoteException;
 
     public int elimina_occorrenze (String fileName) throws RemoteException;
 
 }