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

        String parola, fileName, dirName;
        int response = 0, fileNumber, nRead;
        File inputFile, outputFile, dir;
        FileInputStream fileStream;
        BufferedReader bReader;
        BufferedWriter bWriter;
        String line, newLine;
        Boolean found;
        int occorrenzeParola;
        String[] parole;

        long byteNumber, byteRequested;
        byte[] byteBuffer = new byte[4096];

        try {
            while (true) {
                System.out.println("[DEBUG] pre ricezione");
                char service = inSock.readChar();
                System.out.println("[DEBUG]ricevuto richiesta di servizio " + service);

                if (service == '1') {
                    // SERVIZIO 1: elimina occorrenze di parole
                    // input: filename e parola, output: num eliminazioni
                    fileName = inSock.readUTF();
                    response = 0;
                    found = false;

                    parola = inSock.readUTF();

                    try {
                        inputFile = new File(fileName);
                        outputFile = new File("temp.txt");

                        if (inputFile.isFile() && inputFile.canRead() && inputFile.getName().endsWith(".txt")) {
                            bReader = new BufferedReader(new FileReader(inputFile));
                            bWriter = new BufferedWriter(new FileWriter(outputFile));

                            while ((line = bReader.readLine()) != null) {
                                parole = line.split("\\s");

                                for (String p : parole) {
                                    if (p.equals(parola)) {
                                        response++;
                                    }
                                }

                                newLine = line.replaceAll(parola, "");
                                bWriter.write(newLine);
                                bWriter.newLine();
                            }

                            bReader.close();
                            bWriter.close();

                            if (inputFile.delete() && outputFile.renameTo(inputFile)) {
                                System.out.println("fatto");
                            } else {
                                response = -1;
                                System.out.println("errore");
                                bReader.close();
                                bWriter.close();
                                outputFile.delete();
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        response = -1;
                    }

                    outSock.writeInt(response);
                    // FINE
                } else if (service == '2') {
                    // SERVIZIO 2: trasferimento file maggiori di N byte
                    // input: dirName e N , output: i file

                    dirName = inSock.readUTF();

                    dir = new File(dirName);

                    if (!dir.exists() || !dir.canRead() || !dir.isDirectory()) {
                        outSock.writeBoolean(false);
                        continue;
                    }

                    outSock.writeBoolean(true);

                    byteRequested = inSock.readLong();

                    fileNumber = 0;
                    for (File f : dir.listFiles()) {
                        if (f.canRead() && f.isFile() && !f.getName().endsWith(".txt") && f.length() >= byteRequested) {
                            fileNumber++;
                        }
                    }

                    // Invio numero file
                    outSock.writeInt(fileNumber);

                    for (File f : dir.listFiles()) {
                        if (f.canRead() && f.isFile() && !f.getName().endsWith(".txt") && f.length() >= byteRequested) {
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
                    // FINE
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