package Stream;
import java.io.*;
import java.net.*;

public class StreamThread extends Thread {
    private Socket clientSocket=null;
    private String[][] dati;

    public StreamThread(Socket socket, String [][] dati) {
        this.clientSocket = socket;
        this.dati=dati;
    }

    @Override
    public void run() {
        System.out.println("[DEBUG] Attivazione figlio: "+ Thread.currentThread().getName());

        DataInputStream inSock;
        DataOutputStream outSock;
        
        //CREAZIONE STREAM INPUT E OUTPUT 
        try {
            inSock = new DataInputStream(clientSocket.getInputStream());
            outSock = new DataOutputStream(clientSocket.getOutputStream());
        }
        catch (IOException ioe) {
            System.out.println("[ERROR] Problemi nella creazione degli stream di input/output " );
            ioe.printStackTrace();
            return;
        }

        //CORPO DELLA THREAD
        try {
           
            while (true) {
                System.out.println("[DEBUG] pre ricezione");
                char service = inSock.readChar();
                System.out.println("[DEBUG]ricevuto richiesta di servizio " + service);

                if (service == 'E') {
                //SERVIZIO
                    
                //FINE
                } else if (service == 'T'){
                //SERVIZIO
                

                //FINE
                }
               
                
            }

            // fine ciclo interazione

        }  catch (EOFException e) {
            System.out.println("Terminazione. (Thread=" + getName() + ")");
        } catch (SocketTimeoutException e) {
            System.out.println("[ERROR]: Socket timed out. (Thread=" + getName() + ")");
            e.printStackTrace();
        } catch (IOException e) {
            System.out.println("[ERROR]: Lettura/Scrittura su stream fallita. (Thread=" + getName() + ")");
            e.printStackTrace();
        } catch (Exception e) {
            System.out.println("[ERROR]: Errore irreversibile. (Thread=" + getName() + ")");
            e.printStackTrace();
        } finally {
            System.out.println("[DEBUG]: Terminazione server thread. (Thread=" + getName() + ")");
            try {
                clientSocket.close();
            } catch (IOException e) {
                System.out.println("[ERROR]: Chiusura socket fallita. (Thread=" + getName() + ")");
                e.printStackTrace();
            }
        }
    }
}
