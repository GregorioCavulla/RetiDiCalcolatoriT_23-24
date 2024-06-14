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
            System.out
                    .println("Servizi: 1: conta linee con occorrenze di carattere, 2: <operazione 2> ^D per terminare");

            while ((buffer = stdIn.readLine()) != null) {

                if (buffer.equals("1")) { // Servizio 1
                    outSock.writeChar('1');

                    System.out.println("inserisci il direttorio:");
                    String dirName;
                    dirName = stdIn.readLine();
                    if (dirName == null) {
                        System.err.println("readline: ");
                        continue;
                    }

                    outSock.writeUTF(dirName);

                    System.out.println("inserisci il carattere:");
                    String carattereString;
                    carattereString = stdIn.readLine();
                    if (carattereString == null) {
                        System.err.println("readline: ");
                        continue;
                    }
                    char carattere = carattereString.charAt(0);

                    outSock.writeChar(carattere);

                    System.out.println("inserisci il numero occorrenze:");
                    String occString;
                    int occ = -1;
                    occString = stdIn.readLine();
                    if (occString == null) {
                        System.err.println("readline: ");
                        continue;
                    }
                    try {
                        occ = Integer.parseInt(occString);
                    } catch (NumberFormatException e) {
                        System.err.println("errore, numero occorrenze deve essere intero");
                        continue;
                    }

                    if (occ == -1) {
                        System.err.println("readline: ");
                        continue;
                    }

                    outSock.writeInt(occ);
                    // risposta dal server
                    // leggo il numero di file trovato
                    int fileTrovati = inSock.readInt();

                    if (fileTrovati < 0) {
                        System.err.println("errore server");
                    } else {
                        System.out.println("trovati " + fileTrovati + " files:");
                        for (int i = 0; i < fileTrovati; i++) {
                            // leggo i nomi dei file
                            String fileName = inSock.readUTF();
                            System.out.println("file " + i + " : " + fileName);
                        }

                    }

                    if (inSock.readInt() == -1) {
                        System.out.println("server ha terminato");
                        continue;
                    }

                } else if (buffer.equals("2")) { // Servizio 2
                    outSock.writeChar('2');

                    // risposta dal server
                    inSock.readUTF();

                } else
                    System.out.println("Nessun metodo corrispondente");

                System.out.println(
                        "Servizi: 1: conta linee con occorrenze di carattere, 2: <operazione 2> ^D per terminare");

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