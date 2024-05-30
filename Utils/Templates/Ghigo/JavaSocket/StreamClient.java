import java.io.*;
import java.net.*;

public class StreamClient {
    public static void main(String[] args) {

        InetAddress addr = null; // indirizzo del server
        int port = -1; // porta del server

        Socket socket = null; // socket
        DataInputStream inSock = null; // stream di input
        DataOutputStream outSock = null; // stream di output

        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in)); // stream di input da tastiera

        String buffer; // stringa per la comunicazione

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
        } catch (Exception e) {
            System.out.println("[ERROR] Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("java StreamClient serverAddr serverPort ");
            System.exit(2);
        }

        // CREAZIONE SOCKET
        try {
            socket = new Socket(addr, port); // creazione socket
            socket.setSoTimeout(300000); // timeout 5 minuti
            System.out.println("[DEBUG] Creata la socket: " + socket + " sulla porta locale " + socket.getLocalPort());
        } catch (Exception e) {
            System.out.println("[ERROR] Problemi nella creazione della socket: ");
            e.printStackTrace();
            System.exit(1);

        }
        // CREAZIONE STREAM
        try {
            inSock = new DataInputStream(socket.getInputStream()); // creazione stream di input
            outSock = new DataOutputStream(socket.getOutputStream()); // creazione stream di output
        } catch (IOException e) {
            System.out.println("[ERROR] Problemi nella creazione degli stream su socket: ");
            e.printStackTrace();
            System.exit(1);
        }

        String carattere, occorenze_String, dirName, fileName;
        int occorenze, response, fileNumber, nread, buffer_size = 4096;
        long fileLength;
        boolean esiste;
        FileOutputStream fileStream;
        byte[] byteBuffer = new byte[buffer_size];

        // CICLO INTERAZIONE CLIENT
        try {
            System.out.println("Operazioni: 1: <operazione1>, 2: <operazione2>; ^D per terminare: \n");

            while ((buffer = stdIn.readLine()) != null) { // ciclo di lettura da tastiera

                if (buffer.equals("1")) {
                    outSock.writeChar('1'); // invio operazione
                    System.out.println("[DEBUG] Inviato: " + buffer);

                    System.out.println("Inserire carattere: ");
                    carattere = stdIn.readLine();
                    System.out.println("Inserire occorrenze: ");
                    occorenze_String = stdIn.readLine();

                    try {
                        occorenze = Integer.parseInt(occorenze_String);
                    } catch (NumberFormatException e) {
                        System.err.println("l'input non è un intero");
                        System.out.println("Operazioni: 1: <operazione1>, 2: <operazione2>; ^D per terminare: \n");
                        continue;
                    }

                    outSock.writeChar(carattere.charAt(0));
                    outSock.writeInt(occorenze);

                    buffer = inSock.readUTF(); // attesa risposta
                    System.out.println("[DEBUG] Ricevuto: " + buffer);

                    response = inSock.readInt();
                    System.out.println("Trovate " + response + " occorrenze del carattere " + carattere);

                } else if (buffer.equals("2")) {
                    outSock.writeChar('2'); // invio operazionw
                    System.out.println("[DEBUG] Inviato: " + buffer);
                    System.out.println("inserire direttorio: ");
                    dirName = stdIn.readLine();

                    outSock.writeUTF(dirName);

                    esiste = inSock.readBoolean();

                    if (esiste) {
                        fileNumber = inSock.readInt();
                        for (int i = 0; i < numFile; i++) {
                            fileName = inSock.readUTF();

                            fileLength = inSock.readLong(buffer);

                            fileStream = new FileOutputStream(fileName);

                            while (fileLength > 0) {
                                nread = inSock.read(byteBuffer, 0,
                                        fileLength > buffer_size ? buffer_size : (int) fileLength);
                                fileStream.write(byteBuffer, 0, nread);
                                fileLength-=nread
                            }

                            fileStream.close();

                        }
                    }

                    buffer = inSock.readUTF(); // attesa risposta
                    System.out.println("[DEBUG] Ricevuto: " + buffer);
                } else {
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
            socket.shutdownOutput(); // chiusura stream di output
            socket.close(); // chiusura socket
        } catch (IOException e) {
            System.out.println("[ERROR] Problemi nella chiusura della socket: ");
            e.printStackTrace();
            System.exit(1);
        }
    }
}