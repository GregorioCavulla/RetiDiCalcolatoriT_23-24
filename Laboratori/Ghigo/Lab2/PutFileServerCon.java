import java.io.*;
import java.net.*;

class PutFileServerThread extends Thread {
    private Socket clientSocket = null;
    private int buffer_size = 4096;

    public PutFileServerThread(Socket clientSocket) {
        this.clientSocket = clientSocket;
    }

    public PutFileServerThread(Socket clientSocket, int buffer_size) {
        this.clientSocket = clientSocket;
        this.buffer_size = buffer_size;
    }

    // metodo run del thread
    public void run() {
        System.out.println("Attivazione figlio: " + Thread.currentThread().getName());

        // Dichiarazione di stream di input e output su socket
        DataInputStream inSock;
        DataOutputStream outSock;

        // Dichiarazione di variabili per la lettura e scrittura di file
        byte[] buffer = new byte[buffer_size];
        int cont = 0;
        int read_bytes = 0;
        DataOutputStream dest_stream = null;

        // creazione stream di input e output su socket
        try {
            inSock = new DataInputStream(clientSocket.getInputStream());
            outSock = new DataOutputStream(clientSocket.getOutputStream());
        } catch (IOException ioe) {
            System.out.println("Problemi nella creazione degli stream di input/output su socket: ");
            ioe.printStackTrace();
            return;
        }

        // Ricezione dei fprivate ile
        try {
            try {
                // dichiarazione di variabili per la lettura e scrittura di file
                String nomeFileRicevuto;
                long numeroByte;
                File fileCorr;
                FileOutputStream outFileCorr;

                // ciclo di ricezione dei file
                while ((nomeFileRicevuto = inSock.readUTF()) != null) {
                    fileCorr = new File(nomeFileRicevuto);
                    if (fileCorr.exists()) { // se il file esiste già sul server dico al client di saltare
                        outSock.writeUTF("salta file");
                    } else { // altrimenti dico al client di inviare il file
                        outSock.writeUTF("attiva");
                        numeroByte = inSock.readLong();
                        System.out.println("Scrivo il file " + nomeFileRicevuto + " di " + numeroByte + " byte");
                        outFileCorr = new FileOutputStream(nomeFileRicevuto); // Apro il file in scrittura

                        // Ricevo il file (in linea)
                        dest_stream = new DataOutputStream(outFileCorr); // Apro il file in scrittura
                        cont = 0;

                        // leggo i byte del file
                        try {
                            // esco dal ciclo quando ho letto il numero di byte da trasferire
                            while (cont < numeroByte) {
                                read_bytes = inSock.read(buffer); // leggo i byte dalla socket
                                dest_stream.write(buffer, 0, read_bytes); // scrivo i byte sul file
                                cont += read_bytes; // aggiorno il contatore dei byte trasferiti
                            }
                            dest_stream.flush();
                            System.out.println("Byte trasferiti: " + cont);
                        }
                        // l'eccezione dovrebbe scattare solo se ci aspettiamo un numero sbagliato di
                        // byte da leggere
                        catch (EOFException e) {
                            System.out.println("Problemi, i seguenti: ");
                            e.printStackTrace();
                        }
                        outFileCorr.close();
                    }
                } // while

            } catch (EOFException eof) {
                System.out.println("Raggiunta la fine delle ricezioni, chiudo...");
                clientSocket.close();
                System.out.println("PutFileServer: termino...");
                System.exit(0);
            } catch (SocketTimeoutException ste) {
                System.out.println("Timeout scattato: ");
                ste.printStackTrace();
                clientSocket.close();
                System.exit(1);
            } catch (Exception e) {
                System.out.println("Problemi, i seguenti : ");
                e.printStackTrace();
                System.out.println("Chiudo ed esco...");
                clientSocket.close();
                System.exit(2);
            }
        } catch (IOException ioe) {
            System.out.println("Problemi nella chiusura della socket: ");
            ioe.printStackTrace();
            System.out.println("Chiudo ed esco...");
            System.exit(3);
        }
    }

}// thread

public class PutFileServerCon {

    public static void main(String[] args) throws IOException {

        // parametri di default
        int port = -1;
        int buffer_size = 4096;

        // controllo dei parametri
        try {
            if (args.length == 1) {
                port = Integer.parseInt(args[0]);
                // controllo che la porta sia nel range consentito 1024-65535
                if (port < 1024 || port > 65535) {
                    System.out.println("Usage: java PutFileServerCon <ServerPort>1024> [BufferSize]");
                    System.exit(1);
                }
            } else if (args.length == 2) {
                port = Integer.parseInt(args[0]);
                // controllo che la porta sia nel range consentito 1024-65535
                if (port < 1024 || port > 65535) {
                    System.out.println("Usage: java PutFileServerCon <ServerPort>1024> [BufferSize]");
                    System.exit(1);
                }
                buffer_size = Integer.parseInt(args[1]);

            } else {
                System.out.println("Usage: java PutFileServerCon <ServerPort>1024> [BufferSize]");
                System.exit(1);
            }
        } catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("Usage: java PutFileServerCon <ServerPort>1024> [BufferSize]");
            System.exit(1);
        }

        // Dichiarazione di server socket e socket
        ServerSocket serverSocket = null;
        Socket clientSocket = null;

        // creazione server socket
        try {
            serverSocket = new ServerSocket(port);
            serverSocket.setReuseAddress(true);
            System.out.println("PutFileServerCon: avviato ");
            System.out.println("Server: creata la server socket: " + serverSocket);
        } catch (Exception e) {
            System.err.println("Server: problemi nella creazione della server socket: " + e.getMessage());
            e.printStackTrace();
            serverSocket.close();
            System.exit(1);
        }

        // ciclo di creazione dei thread figli
        try {
            while (true) {
                System.out.println("Server: in attesa di richieste...\n");

                try {
                    clientSocket = serverSocket.accept(); // bloccante!!!
                    System.out.println("Server: connessione accettata: " + clientSocket);
                } catch (Exception e) {
                    System.err.println("Server: problemi nella accettazione della connessione: " + e.getMessage());
                    e.printStackTrace();
                    continue;
                }

                try {
                    if (args.length == 1) {
                        new PutFileServerThread(clientSocket).start(); // creo un thread figlio
                    } else if (args.length == 2) {
                        new PutFileServerThread(clientSocket, buffer_size).start(); // creo un thread figlio con buffer
                                                                                    // size
                    }
                } catch (Exception e) {
                    System.err.println("Server: problemi nel server thread: " + e.getMessage());
                    e.printStackTrace();
                    continue;
                }
            } // while true
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Server: termino...");
            System.exit(2);
        }
    }
}
