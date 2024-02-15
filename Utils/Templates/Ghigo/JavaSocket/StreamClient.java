import java.io.*;
import java.net.*;

public class StreamClient {
    public static void main(String[] args) {
        
        InetAddress addr = null;            // indirizzo del server
        int port = -1;                      // porta del server

        Socket socket = null;               // socket
        DataInputStream inSock = null;      // stream di input
        DataOutputStream outSock = null;    // stream di output

        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in)); // stream di input da tastiera

        String buffer;  // stringa per la comunicazione
     
        // CONTROLLO ARGOMENTI

        try {
            if (args.length == 2) {
                addr = InetAddress.getByName(args[0]);
                port = Integer.parseInt(args[1]);
                if (port < 1024 || port > 65535) {
                    System.out.println("[ERROR] la porta deve essere compresa nel range valido [1024-65535 ");
                    System.exit(1);
                }
            } else {
                System.out.println("[ERROR] java StreamClient serverAddr serverPort ");
                System.exit(1);
            }
        }
        catch (Exception e) {
            System.out.println("[ERROR] Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("java StreamClient serverAddr serverPort ");
            System.exit(2);
        }

        // CREAZIONE SOCKET
        try {
            socket = new Socket(addr, port);    // creazione socket
            socket.setSoTimeout(300000);    // timeout 5 minuti
            System.out.println("[DEBUG] Creata la socket: " + socket + " sulla porta locale " + socket.getLocalPort());
        } catch (Exception e) {
            System.out.println("[ERROR] Problemi nella creazione della socket: ");
            e.printStackTrace();
            System.exit(1);

        }
        //CREAZIONE STREAM
        try {
            inSock = new DataInputStream(socket.getInputStream());  // creazione stream di input
            outSock = new DataOutputStream(socket.getOutputStream());   // creazione stream di output
        } catch (IOException e) {
            System.out.println("[ERROR] Problemi nella creazione degli stream su socket: ");
            e.printStackTrace();
            System.exit(1);
        }

        //CICLO INTERAZIONE CLIENT
        try {
            System.out.println("Operazioni: 1: <operazione1>, 2: <operazione2>; ^D per terminare: \n");
            
            while ((buffer = stdIn.readLine()) != null) { // ciclo di lettura da tastiera

                if(buffer.equals("1")) {
                    outSock.writeChar('1');   // invio operazione
                    System.out.println("[DEBUG] Inviato: " + buffer);

                    buffer = inSock.readUTF();   // attesa risposta
                    System.out.println("[DEBUG] Ricevuto: " + buffer);
                }
                else if(buffer.equals("2")) {
                    outSock.writeChar('2');  // invio operazionw
                    System.out.println("[DEBUG] Inviato: " + buffer);

                    buffer = inSock.readUTF();   // attesa risposta
                    System.out.println("[DEBUG] Ricevuto: " + buffer);
                }
                else {
                    System.out.println("[ERROR] Operazione non riconosciuta\n");
                }
                System.out.println("Operazioni: 1: <operazione1>, 2: <operazione2>; ^D per terminare: \n");
            } // end while
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Errore nella comunicazione");
            System.exit(1);
        }

        // CHIUSURA SOCKET
        try {
            socket.shutdownInput(); // chiusura stream di input
            socket.shutdownOutput();    // chiusura stream di output
            socket.close(); // chiusura socket
        } catch (IOException e) {
            System.out.println("[ERROR] Problemi nella chiusura della socket: ");
            e.printStackTrace();
            System.exit(1);
        }
    }
}