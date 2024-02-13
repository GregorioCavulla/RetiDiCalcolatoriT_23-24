
/**
 * RMI_Client.java
 *
 */

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.Arrays;
 
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
             System.out.print("Servizio (L=Lista file,N=Numerazione righe): ");
 
             /* ciclo accettazione richieste utente */
             while ((service = stdIn.readLine()) != null) {
 
                 if (service.equals("L")) {
                     String direttorio,result[];
                     System.out.print("Nome direttorio: ");
                     direttorio = stdIn.readLine();

                     try{
                        result = serverRMI.lista_file(direttorio);

                        if(result==null){
                            System.out.println("Errore, non è stato recuperato nessun file");
                        }else {
                            System.out.println("File nel direttorio con >=3 consonanti\n" + Arrays.toString(result));
                        }

                     }catch(RemoteException e){
                        System.out.println("Errore remoto: " + e.toString());
                     }
                 } // L=LIsta file
 
                 else if (service.equals("N")) {
                     String file;
                     System.out.print("File: ");
                     file = stdIn.readLine();

                     int res=0;
                     try{
                        res = serverRMI.numerazione_righe(file);
                         if(res==-1){
                             System.out.println("Errore durante la numerazione delle righe");
                         }else{
                             System.out.println("Trovate " + res + " righe dispari");
                         }
                     }catch(RemoteException r){
                         r.printStackTrace();
                     }
                 }// V=Visualizza
 
                 else
                     System.out.println("Servizio non disponibile");
 
                 System.out.print("Servizio (L=Lista file,N=Numerazione righe): ");
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