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

        try {
            System.out.println("Servizi: E(liminazione)\t T(rasfermento)\t ^D per terminare");

            while ((buffer = stdIn.readLine()) != null) {
                // operazione di Elimina
                if (buffer.equals("E")) {
                    outSock.writeChar('E');

                    // risposta dal server
                    inSock.readUTF();

                } else if (buffer.equals("T")) {
                    outSock.writeChar('T');

                    // risposta dal server
                    inSock.readUTF();

                } else
                    System.out.println("Nessun metodo corrispondente");

                System.out.println("Servizi: C(onteggio numero linee)\t T(rasfermento file binari)\t ^D per terminare");

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