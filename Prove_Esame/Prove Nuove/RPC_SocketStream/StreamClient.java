import java.io.*;
import java.net.*;

public class StreamClient {
    public static void main(String[] args) {
        InetAddress addr = null;
        int port = -1;

        Socket socket = null;
        DataInputStream inSock = null;
        DataOutputStream outSock = null;

        String fileName;
        long fileLength;
        int nRead;
        byte[] byteBuffer = new byte[4096];
        FileOutputStream fileStream;

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

        try {
            System.out.println("Servizi: 1: conteggio linee, 2: trasferimento file binari ^D per terminare");

            while ((buffer = stdIn.readLine()) != null) {

                if (buffer.equals("1")) { // Servizio 1
                    outSock.writeChar('1');
                    char carattere;
                    do {
                        System.out.println("inserisci un carattere maiuscolo: ");
                        buffer = stdIn.readLine();
                        carattere = buffer.charAt(0);
                    } while (!Character.isUpperCase(carattere));

                    outSock.writeChar(carattere);

                    System.out.println("inserisci il numero di occorrenze: ");
                    buffer = stdIn.readLine();
                    try {
                        int occorrenze = Integer.parseInt(buffer);
                        outSock.writeInt(occorrenze);
                    } catch (NumberFormatException e) {
                        System.err.println("errore input: " + e);
                        continue;
                    }
                    // risposta dal server
                    int risposta = inSock.readInt();

                    System.out.println("ho contato " + risposta + " linee.");

                } else if (buffer.equals("2")) { // Servizio 2
                    outSock.writeChar('2');

                    System.out.println("inserisci il nome del direttorio: ");
                    buffer = stdIn.readLine();

                    outSock.writeUTF(buffer); // mando il nome del direttorio
                    boolean exists = inSock.readBoolean();
                    if (exists) {
                        int fileNumber = inSock.readInt();

                        for (int i = 0; i < fileNumber; i++) {
                            fileName = inSock.readUTF();
                            fileLength = inSock.readLong();
                            fileStream = new FileOutputStream(fileName);

                            while (fileLength > 0) {

                                nRead = inSock.read(byteBuffer, 0, fileLength > 4096 ? 4096 : (int) fileLength);
                                fileStream.write(byteBuffer, 0, nRead);
                                fileLength -= nRead;
                            }

                            System.out.println("ricevuto: " + fileName);
                            fileStream.close();
                        }
                    }
                    // risposta dal server, lettura file binari
                    inSock.readUTF();

                } else
                    System.out.println("Nessun metodo corrispondente");

                System.out.println("Servizi: 1: conteggio linee, 2: trasferimento file binari ^D per terminare");

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