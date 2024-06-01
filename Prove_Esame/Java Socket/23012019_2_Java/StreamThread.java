import java.io.*;
import java.net.*;

public class StreamThread extends Thread {

    private Socket clientSocket = null; // socket per comunicare con il client
    private Prenotazione prenotazioni[]; // struttura dati condivisa tra i thread

    public StreamThread(Socket socket, prenotazioni) {
        this.clientSocket = socket;
        this.prenotazioni = prenotazioni;
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
        String targa, targaImgFolder;
        Boolean found = false;
        File currDir, targaDir;
        FileInputStream fileStream;
        long byteNumber;
        byte[] byteBuffer = new byte[4096];
        int nRead, fileNumber;
        try {
            while (true) {
                System.out.println("[DEBUG] pre ricezione");
                char service = inSock.readChar();
                System.out.println("[DEBUG]ricevuto richiesta di servizio " + service);

                if (service == '1') {
                    targa = inSock.readUTF();

                    for (Prenotazione p : prenotazioni) {
                        if (p.getTarga().equals(targa)) {
                            p.setTarga("L");
                            p.setPatente("L");
                            p.setTipo("L");
                            p.setFolder_img("L");
                            found = true;
                        }
                    }

                    outSock.writeBoolean(found);

                } else if (service == '2') {
                    targa = inSock.readUTF();

                    targaImgFolder = targa+"_img/"

                    currDir = new File(".");

                    for (Prenotazione p : prenotazioni) {
                        if (p.getTarga().equals(targa)) {
                            found = true;
                            outSock.writeBoolean(found);
                        }
                    }

                    if(found){
                        for(File f : currDir.listFiles()){
                            if(f.isDirectory() && f.canRead() && f.getName().equals(targaImgFolder)){
                                targaDir = new File("."+targaImgFolder);
                                for(File file : targaDir.listFiles()){
                                    if (file.canRead() && file.isFile() && !file.getName().endsWith(".txt")) {
                                        fileNumber ++;
                                    }
                                }
                                outSock.writeInt(fileNumber);
                                for(File file : targaDir.listFiles()){
                                    if (file.canRead() && file.isFile() && !file.getName().endsWith(".txt")) {
                                        outSock.writeUTF(file.getName());
                                        fileStream = new FileInputStream(file);
            
                                        byteNumber = file.length();
            
                                        outSock.writeLong(byteNumber);
            
                                        while (byteNumber > 0) {
                                            nRead = fileStream.read(byteBuffer);
                                            outSock.write(byteBuffer, 0, nRead);
            
                                            byteNumber -= nRead;
                                        }
            
                                        fileStream.close();
                                    }
                                }
                            }
                        }
                    }

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