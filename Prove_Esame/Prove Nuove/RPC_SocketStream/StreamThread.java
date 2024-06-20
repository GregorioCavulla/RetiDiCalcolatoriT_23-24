import java.io.*;
import java.net.*;

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
        try {
            while (true) {
                System.out.println("[DEBUG] pre ricezione");
                char service = inSock.readChar();
                System.out.println("[DEBUG] ricevuto richiesta di servizio " + service);

                if (service == '1') {
                    // SERVIZIO 1, mi manda nell'ordine: carattere, occorrenze

                    int lineeTrovate = 0;

                    String dirName = ".";

                    char carInput = inSock.readChar();

                    int occInput = inSock.readInt();

                    System.out.println(
                            "[DEBUG]: dirname: " + dirName + ", car: " + carInput + ", occ: " + occInput + ".");

                    File dir = new File(dirName);
                    int occ;

                    if (dir.isDirectory() && dir.exists() && dir.canRead()) {
                        for (File f : dir.listFiles()) {
                            if (f.isFile() && f.canRead() && f.exists() && f.getName().endsWith(".txt")) {
                                System.out.println("[DEBUG] valido " + f.getName());
                                try (BufferedReader reader = new BufferedReader(new FileReader(f))) {
                                    String line;
                                    while ((line = reader.readLine()) != null) {
                                        occ = 0;
                                        for (char ch : line.toCharArray()) {
                                            if (ch == carInput) {
                                                occ++;
                                            }
                                        }
                                        if (occ >= occInput) {
                                            lineeTrovate++;
                                        }
                                    }
                                } catch (IOException e) {
                                    System.out.println("[ERROR] Problemi nella lettura del file " + f.getName());
                                    e.printStackTrace();
                                }
                            }
                        }
                    } else {
                        System.out.println("[ERROR] Directory non valida o non leggibile");
                    }

                    outSock.writeInt(lineeTrovate);

                } else if (service == '2') {
                    // SERVIZIO 2, mi manda dirName

                    String dirName = inSock.readUTF();

                    File dir = new File(dirName);

                    if (dir.exists() && dir.canRead() && dir.isDirectory()) {
                        outSock.writeBoolean(true);
                        int fileNumber = 0;
                        for (File f : dir.listFiles()) {
                            if (f.canRead() && f.isFile() && f.exists() && !f.getName().endsWith(".txt")) {
                                fileNumber++;
                            }
                        }
                        outSock.writeInt(fileNumber);

                        FileInputStream fileStream;

                        for (File f : dir.listFiles()) {
                            if (f.canRead() && f.isFile() && f.exists() && !f.getName().endsWith(".txt")) {
                                outSock.writeUTF(f.getName());
                                fileStream = new FileInputStream(f);

                                long byteNumber = f.length();

                                outSock.writeLong(byteNumber);

                                byte[] byteBuffer = new byte[4096];

                                while (byteNumber > 0) {
                                    int nRead = fileStream.read(byteBuffer);
                                    outSock.write(byteBuffer, 0, nRead);

                                    byteNumber -= nRead;
                                }
                                fileStream.close();
                            }
                        }

                    } else {
                        outSock.writeBoolean(false);
                        continue;
                    }
                    // FINE
                    System.out.println("[DEBUG]: termino servizio 2");
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
