import java.io.*;
import java.net.*;

public class StreamClient {
    public static void main(String[] args) {
        InetAddress addr = null;
        int port = -1;

        Socket socket = null;
        DataInputStream inSock = null;
        DataOutputStream outSock = null;

        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

        String buffer;

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
        } catch (Exception e) {
            System.out.println("[ERROR] Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("java StreamClient serverAddr serverPort ");
            System.exit(2);
        }

        // CREAZIONE SOCKET
        try {
            socket = new Socket(addr, port);
            socket.setSoTimeout(300000);
            System.out.println("[DEBUG] Creata la socket: " + socket + " sulla porta locale " + socket.getLocalPort());
        } catch (Exception e) {
            System.out.println("[ERROR] Problemi nella creazione della socket: ");
            e.printStackTrace();
            System.exit(1);

        }
        // CREAZIONE STREAM
        try {
            inSock = new DataInputStream(socket.getInputStream());
            outSock = new DataOutputStream(socket.getOutputStream());
        } catch (IOException e) {
            System.out.println("[ERROR] Problemi nella creazione degli stream su socket: ");
            e.printStackTrace();
            System.exit(1);
        }

        // CICLO INTERAZIONE CLIENT

        String carattere, occorenze_String, dirName, fileName;
        int occorenze, response, fileNumber, nread, buffer_size = 4096;
        long fileLength;
        boolean esiste;
        FileOutputStream fileStream;
        byte[] byteBuffer = new byte[buffer_size];

        try {
            System.out.println(
                    "Servizi: 1: conteggio linee con maiuscola e occorrenze di carattere, 2: trasferimento file binari ^D per terminare");

            while ((buffer = stdIn.readLine()) != null) {

                if (buffer.equals("1")) { // Servizio 1
                    outSock.writeChar('1');

                    System.out.println("[DEBUG] Inviato servizio: " + buffer);

                    System.out.println("Inserire carattere: "); // richiesta a utente
                    carattere = stdIn.readLine();
                    System.out.println("Inserire occorrenze: "); // richiesta a utente
                    occorenze_String = stdIn.readLine();

                    try {
                        occorenze = Integer.parseInt(occorenze_String); // parse dell'integer
                    } catch (NumberFormatException e) {
                        System.err.println("l'input non è un intero");
                        System.out
                                .println("Operazioni: 1: conta occorrenze , 2: trasferisci file; ^D per terminare: \n");
                        continue;
                    }

                    outSock.writeChar(carattere.charAt(0)); // output su socket carattere
                    outSock.writeInt(occorenze); // output su socket occorrenze

                    buffer = inSock.readUTF(); // attesa risposta
                    System.out.println("[DEBUG] Ricevuto: " + buffer);

                    response = inSock.readInt();
                    System.out.println("Trovate " + response + " linee con occorrenze del carattere " + carattere);

                    // risposta dal server
                    inSock.readUTF();

                } else if (buffer.equals("2")) { // Servizio 2
                    outSock.writeChar('2'); // invio servizio 2
                    System.out.println("[DEBUG] Inviato: " + buffer);
                    System.out.println("inserire direttorio: "); // richiesat a utente
                    dirName = stdIn.readLine(); // leggo direttorio da stdin

                    outSock.writeUTF(dirName); // output su socket direttorio

                    esiste = inSock.readBoolean(); // conferma esistenza da server

                    if (esiste) {
                        fileNumber = inSock.readInt(); // input da server numero di file
                        for (int i = 0; i < fileNumber; i++) {
                            fileName = inSock.readUTF(); // input da server nome file

                            fileLength = inSock.readLong(); // input da server lunghezza file

                            fileStream = new FileOutputStream(fileName); // creo filestream

                            while (fileLength > 0) { // leggo il file
                                nread = inSock.read(byteBuffer, 0,
                                        fileLength > buffer_size ? buffer_size : (int) fileLength);
                                fileStream.write(byteBuffer, 0, nread);
                                fileLength -= nread;
                            }

                            System.out.println("ricevuto: " + fileName);
                            fileStream.close();
                        }
                        // risposta dal server
                        inSock.readUTF();

                    } else

                        System.out.println("Nessun metodo corrispondente");

                    System.out.println(
                            "Servizi: 1: conteggio linee con maiuscola e occorrenze di carattere, 2: trasferimento file binari ^D per terminare");
                }

            }
        } catch (Exception e1) {
            System.out.println("Problemi nel ciclo di interazione");
            e1.printStackTrace();
        }

        // fine ciclo

        // CHIUSURA SOCKET
        try {
            socket.shutdownOutput();
            socket.shutdownInput();
            socket.close();
        } catch (IOException e) {
            System.out.println("Problemi nella chiusura delle socket");
            e.printStackTrace();
            System.exit(1);
        }

    }// main

}