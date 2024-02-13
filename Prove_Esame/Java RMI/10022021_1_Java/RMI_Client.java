
/**
 * RMI_Client.java
 *
 */

 import java.io.BufferedReader;
 import java.io.InputStreamReader;
 import java.rmi.Naming;
 import java.rmi.NotBoundException;
 import java.rmi.RemoteException;
 
 class RMI_Client {
 
     public static void main(String[] args) {
         final int REGISTRYPORT = 1099;
         String registryHost = null; // host remoto con registry
         String serviceName = "";
         BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
 
         // Controllo dei parametri della riga di comando
         if (args.length != 2) {
             System.out.println("Sintassi: RMI_Registry_IP ServiceName");
             System.exit(1);
         }
         registryHost = args[0];
         serviceName = args[1];
 
         System.out.println("Invio richieste a " + registryHost + " per il servizio di nome " + serviceName);
 
         // Connessione al servizio RMI remoto
         try {
             String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
             RMI_interfaceFile serverRMI = (RMI_interfaceFile) Naming.lookup(completeName);
             System.out.println("ClientRMI: Servizio \"" + serviceName + "\" connesso");
 
             System.out.println("\nRichieste di servizio fino a fine file");
 
             String service;
             System.out.print("Servizio (E=Elimina, N=Noleggia): ");
 
             /* ciclo accettazione richieste utente */
             while ((service = stdIn.readLine()) != null) {
 
                 if (service.equals("E")) {
                     System.out.print("Identificatore: ");
                     String id = null;
                     id = stdIn.readLine();
 
                     if(serverRMI.elimina_sci(id)==0){
                         System.out.println(
                             "Eliminazione di " + id + " avvenuta con successo");
                     }else{
                         System.out.println(
                             "Impossibile eliminare " + id);
                     }
                 } // E=Elimina
 
                 else if (service.equals("N")) {
                     System.out.print("Identificatore: ");
                     String id = null;
                     id = stdIn.readLine();
                     System.out.print("Giorno: ");
                     int giorno = 0;
                     giorno = Integer.parseInt(stdIn.readLine());
                     System.out.print("Mese: ");
                     int mese = 0;
                     mese = Integer.parseInt(stdIn.readLine());
                     System.out.print("Anno: ");
                     int anno = 0;
                     anno = Integer.parseInt(stdIn.readLine());
                     System.out.print("Durata: ");
                     int durata = 0;
                     durata = Integer.parseInt(stdIn.readLine());
                     try{
                         int esito = serverRMI.noleggia_sci(id,giorno,mese,anno,durata);
                         if(esito < 0){
                             System.out.println("Impossibile effettuare una prenotazione per " + id + " in data " + giorno + "/" + mese + "/" + anno + " per " + durata + " giorni");
                         }else{
                             System.out.println("Prenotazione effettuata con successo");
                         }
                     }catch(RemoteException r){
                         r.printStackTrace();
                     }
                 }// V=Visualizza
 
                 else
                     System.out.println("Servizio non disponibile");
 
                 System.out.print("Servizio (E=Elimina, N=Noleggia): ");
             } // while (!EOF), fine richieste utente
 
         } catch (NotBoundException nbe) {
             System.err.println("ClientRMI: il nome fornito non risulta registrato; " + nbe.getMessage());
             nbe.printStackTrace();
             System.exit(1);
         } catch (Exception e) {
             System.err.println("ClientRMI: " + e.getMessage());
             e.printStackTrace();
             System.exit(1);
         }
     }
 }