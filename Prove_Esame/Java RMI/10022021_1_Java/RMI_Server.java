/**
 * RMI_Server.java
 * 	Implementa i servizi dichiarati in RMI_InterfaceFile.
 *  + eliminare la preenotazione di uno sci
 *  + noleggiare uno sci
 * */

 import java.rmi.Naming;
 import java.rmi.RemoteException;
 import java.rmi.server.UnicastRemoteObject;
 import java.io.File;
 
 public class RMI_Server extends UnicastRemoteObject implements RMI_interfaceFile {
     private static final int MAX = 10;
     private static Prenotazione[] prenotazioni;
 
     // Costruttore
     public RMI_Server() throws RemoteException {
         super();
     }
 
     // Elimino la prenotazione di uno sci
     public int elimina_sci(String identificatore) throws RemoteException {
         System.out.println("Server RMI: richiesta eliminazione dello sci con identificatore = " + identificatore);

         int result =  -1;
 
         File immagine = null;
 
         for(int i=0;i<MAX;i++){
             if(prenotazioni[i].getIdentificatore().equals(identificatore)){
                 immagine = new File(prenotazioni[i].getNomeFileFoto());
                 if(immagine.exists()){
                     immagine.delete();
                 }
                 prenotazioni[i] = new Prenotazione();
                 result = 0;
             }
         }

         for(int i=0;i<MAX;i++){
            System.out.println(prenotazioni[i].getIdentificatore()+"|"+prenotazioni[i].getData()+"|"+prenotazioni[i].getGiorni()+"|"+prenotazioni[i].getModello()+"|"+prenotazioni[i].getCostoGiornaliero()+"|"+prenotazioni[i].getNomeFileFoto());
         }
 
         return result;
     }
 
     // Noleggio di uno sci
     public int noleggia_sci(String identificatore,int giorno,int mese,int anno,int durata)throws RemoteException {
 
         int result = -1;

         System.out.println("Server RMI: richiesto noleggio sci");
         System.out.println("Identificatore = " + identificatore);
         System.out.println("Dal " + giorno+"/"+mese+"/"+anno);
         System.out.println("Durata = " + durata);

         String dataString = "-1/-1/-1";
         String nuovaData = giorno + "/" + mese + "/" + anno;
 
         for(int i=0;i<MAX;i++){
            if(prenotazioni[i].getIdentificatore().equals(identificatore) && prenotazioni[i].getData().equals(dataString) && (prenotazioni[i].getGiorni() == -1)){
                prenotazioni[i].setGiorni(durata);
                prenotazioni[i].setData(nuovaData);
                result = 0;
                break;
            }
         }

         for(int i=0;i<MAX;i++){
            System.out.println(prenotazioni[i].getIdentificatore()+"|"+prenotazioni[i].getData()+"|"+prenotazioni[i].getGiorni()+"|"+prenotazioni[i].getModello()+"|"+prenotazioni[i].getCostoGiornaliero()+"|"+prenotazioni[i].getNomeFileFoto());
         }
 
         return result;
     }
 
     // Avvio del Server RMI
     public static void main(String[] args) {
 
         prenotazioni = new Prenotazione[MAX];
         prenotazioni[0] = new Prenotazione("X123","12/12/2022",15,"GoodSnow",10,"GoodSnow.jpg");
         prenotazioni[1] = new Prenotazione("A123","01/12/2022",5,"BadSnow",2,"BadSnow.jpg");
         prenotazioni[2] = new Prenotazione("B123","05/12/2022",10,"Perfect",10,"Perfect.jpg");
         prenotazioni[3] = new Prenotazione("C123","20/12/2022",2,"Medium",8,"Medium.jpg");
         prenotazioni[4] = new Prenotazione("D123","09/12/2022",3,"Excelent",12,"Excelent.jpg");
         prenotazioni[5] = new Prenotazione("E123","-1/-1/-1",-1,"Bob",7,"Bob.jpg");
         prenotazioni[6] = new Prenotazione();
         prenotazioni[7] = new Prenotazione();
         prenotazioni[8] = new Prenotazione();
         prenotazioni[9] = new Prenotazione();
 
         final int REGISTRYPORT = 1099;
         String registryHost = "localhost";
         String serviceName = "RMI_interfaceFile"; // lookup name...
 
         // Registrazione del servizio RMI
         String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
         try {
             RMI_Server serverRMI = new RMI_Server();
             Naming.rebind(completeName, serverRMI);
             System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato");
         } catch (Exception e) {
             System.err.println("Server RMI \"" + serviceName + "\": " + e.getMessage());
             e.printStackTrace();
             System.exit(1);
         }
     }
 }