import java.io.*;
import java.net.*;
import java.lang.Character;

public class StreamThread extends Thread {

    private Socket clientSocket = null; // socket per comunicare con il client
    private String[][] dati; // struttura dati condivisa tra i thread

    public StreamThread(Socket socket, String[][] dati) {
        this.clientSocket = socket;
        this.dati = dati;
    }

    public StreamThread(Socket socket) {
        this.clientSocket = socket;
    }

    @Override
    public void run() {
        System.out.println("[DEBUG] Attivazione figlio: " + Thread.currentThread().getName());

        DataInputStream inSock; // stream di input
        DataOutputStream outSock; // stream di output

        // CREAZIONE STREAM INPUT E OUTPUT
        try {
            inSock = new DataInputStream(clientSocket.getInputStream());
            outSock = new DataOutputStream(clientSocket.getOutputStream());
        } catch (IOException ioe) {
            System.out.println("[ERROR] Problemi nella creazione degli stream di input/output ");
            ioe.printStackTrace();
            return;
        }

        // CORPO DELLA THREAD

        char carattere, startChar = ' ';
        String dirName;
        int occorrenze, occ_linea, res, cread, fileNumber, nRead;
        File currDir, dir;
        Boolean found, newLine, lineaBuona;
        FileReader fr;
        FileInputStream fileStream;
        long byteNumber;
        byte[] byteBuffer = new byte[4096];

        try {
            while (true) {
                System.out.println("[DEBUG] pre ricezione");
                char service = inSock.readChar();
                System.out.println("[DEBUG]ricevuto richiesta di servizio " + service);

                if (service == '1') {
                    // Servizio 1, invio a client il numero di righe che iniziano con maiuscola e
                    // contengono un determinato numero di occorrenze di un determinato carattere
                    System.out.println("[DEBUG] eseguo servizio 1");
                    outSock.writeUTF("eseguo servizio 1");
                    // restiutuisce il numero di righe che contengono le occorrenze richieste di un
                    // determinato carattere
                    // leggo gli input da socket
                    carattere = inSock.readChar();
                    occorrenze = inSock.readInt();

                    System.out.println(
                            "ricevuto carattere: " + carattere + " e numero di occorrenze richieste: " + occorrenze);

                    // creao direttorio corrente
                    currDir = new File("./direttorio");
                    res = 0; // response
                    found = false;
                    newLine = true;
                    lineaBuona = false;

                    for (File f : currDir.listFiles()) { // leggo 1 file alla volta del direttorio
                        if (f.isFile() && f.canRead() && f.getName().endsWith(".txt")) { // se il file è leggibile e di
                                                                                         // testo
                            fr = new FileReader(f);

                            System.out.println("[DEBUG] trovato file: " + f.getName());
                            occ_linea = 0;
                            while ((cread = fr.read()) != -1) {

                                // System.out.println("char letto: " + (char) cread);

                                // int a; char b = (char) a

                                if (cread == '\n') { // se vado a capo resetto i conteggi della linea
                                    found = false;
                                    newLine = true;
                                    lineaBuona = false;
                                    occ_linea = 0;
                                }

                                if (newLine) {
                                    startChar = (char) cread;
                                    // System.out.println("[DEBUG] carattere iniziale riga: " + startChar);
                                    if (Character.isUpperCase(startChar)) {
                                        // System.out.println("trovata linea buona, starting with: " + startChar);
                                        lineaBuona = true;
                                        newLine = false;
                                    }
                                }

                                if (cread == carattere && !found && lineaBuona) { // se trovo un cararttere uguale a
                                                                                  // richiesto
                                    occ_linea++; // aggiorno le occorrenze della linea

                                    if (occ_linea == occorrenze) { // se raggiungo le occorrenze richieste
                                        found = true; // trovo una linea "corretta"

                                        System.out.println("trovata linea buona, starting with: " + startChar
                                                + " containing: " + carattere + " at least " + occ_linea + " times");
                                        res++; // aggiungo uno al conteggio della risposta
                                    }
                                }
                            }
                            fr.close(); // chiudo il reader
                        }
                    }
                    System.out.println("[DEBUG] termino servizio 1, invio risposta: " + res);
                    outSock.writeInt(res); // invio la risposta

                    // FINE
                } else if (service == '2') {
                    System.out.println("[DEBUG] eseguo servizio 2");
                    // invio i file binari nel direttorio richiesto dal cliente
                    dirName = inSock.readUTF();

                    dir = new File(dirName);

                    if (!dir.exists() || !dir.canRead() || !dir.isDirectory()) {
                        outSock.writeBoolean(false);
                        continue;
                    }

                    outSock.writeBoolean(true);

                    fileNumber = 0;
                    for (File f : dir.listFiles()) {
                        if (f.canRead() && f.isFile() && !f.getName().endsWith(".txt")) {
                            fileNumber++;
                        }
                    }

                    // Invio numero file
                    outSock.writeInt(fileNumber);

                    for (File f : dir.listFiles()) {
                        if (f.canRead() && f.isFile() && !f.getName().endsWith(".txt")) {
                            outSock.writeUTF(f.getName());
                            fileStream = new FileInputStream(f);

                            byteNumber = f.length();

                            outSock.writeLong(byteNumber);

                            while (byteNumber > 0) {
                                nRead = fileStream.read(byteBuffer);
                                outSock.write(byteBuffer, 0, nRead);

                                byteNumber -= nRead;
                            }

                            fileStream.close();
                        }
                    }
                    // FINE
                    System.out.println("[DEBUG] termino servizio 2");
                }
            } // fine ciclo interazione

        } catch (EOFException e) {
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