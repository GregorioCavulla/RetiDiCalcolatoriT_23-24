/**
 * InviaFileServer.java
 *  + contare il numero delle linee che contengono almeno uno specificato numero di occorrenz di un carattere
 *  + trasferire dal server al client tutti i file di binari di un direttorio
 * */

import java.io.*;
import java.net.*;

class InviaFileServerThread extends Thread {
    private Socket clientSocket = null;

    public InviaFileServerThread(Socket clientSocket) {
        this.clientSocket = clientSocket;
    }

    public void run() {
        System.out.println("Attivazione figlio: " + Thread.currentThread().getName());

        DataInputStream inSock;
        DataOutputStream outSock;

        try {
            inSock = new DataInputStream(clientSocket.getInputStream());
            outSock = new DataOutputStream(clientSocket.getOutputStream());
        } catch (IOException ioe) {
            System.out.println("Problemi nella creazione degli stream di input/output su socket: ");
            ioe.printStackTrace();
            return;
        }
        try {
            try {
                String servizio,dirName;
                char carattere;
                int occorrenze,res,occorrenze_linea,cread,nFiles,nread;
                File dirCorr,dir;
                boolean trovato;
                FileReader fr;
                FileInputStream fileStream;
                byte [] buffer = new byte[4096];
                long nByte;

                while ((servizio = inSock.readUTF()) != null) {
                    if(servizio.equalsIgnoreCase("C")){
                        //Conteggio numero righe con occorrenze 

                        //Leggo carattere
                        carattere = inSock.readChar();

                        //Leggo occorrenze
                        occorrenze = inSock.readInt();

                        //Leggo dir corrente
                        dirCorr = new File(".");
                        res = 0;
                        trovato = false;

                        for(File f:dirCorr.listFiles()){
                            if(f.isFile() && f.canRead() && f.getName().endsWith(".txt")){
                                fr = new FileReader(f);

                                occorrenze_linea=0;

                                while ((cread=fr.read())!=-1) {
                                    if(cread=='\n'){
                                        trovato=false;
                                        occorrenze_linea=0;
                                    }else if(cread == carattere && !trovato){
                                        occorrenze_linea++;

                                        if(occorrenze_linea==occorrenze){
                                            trovato=true;
                                            res++;
                                        }
                                    } 
                                }
                                fr.close();
                            }
                        }

                        //Scrivo il numero di linee trovate che soddisfano la condizione 
                        outSock.writeInt(res);
                    }else if(servizio.equalsIgnoreCase("B")){
                        //Trasferisco tutti i file binari di un direttorio

                        //Leggo direttorio 
                        dirName = inSock.readUTF();

                        dir = new File(dirName);

                        if(!dir.exists() || !dir.canRead() || !dir.isDirectory()){
                            //dir non esiste o non è valida
                            outSock.writeBoolean(false);
                            continue;
                        }

                        //dir esiste ed è valida
                        outSock.writeBoolean(true);

                        //Conto il numero di file binari presenti in dir
                        nFiles=0;
                        for(File f:dir.listFiles()){
                            if(f.canRead() && f.isFile() && !f.getName().endsWith(".txt")){
                                nFiles++;
                            }
                        }

                        //Invio numero file
                        outSock.writeInt(nFiles);

                        for(File f:dir.listFiles()){
                            if(f.canRead() && f.isFile() && !f.getName().endsWith(".txt")){
                                //Scrivo il nome del file
                                outSock.writeUTF(f.getName());

                                //Apro stream del file
                                fileStream = new FileInputStream(f);

                                nByte = f.length();

                                //Scrivo lunghezza file
                                outSock.writeLong(nByte);

                                //Invio file
                                while (nByte>0) {
                                    nread = fileStream.read(buffer);
                                    outSock.write(buffer,0,nread);

                                    nByte -= nread;
                                }

                                fileStream.close();
                            }
                        }
                    }
                } // while
            } catch (EOFException eof) {
                System.out.println("Raggiunta la fine delle ricezioni, chiudo...");
                clientSocket.close();
                System.out.println("InviaFileServer: termino...");
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

public class InviaFileServer {

    public static void main(String[] args) throws IOException {
        int port = -1;

        try {
            if (args.length == 1) {
                port = Integer.parseInt(args[0]);
                // controllo che la porta sia nel range consentito 1024-65535
                if (port < 1024 || port > 65535) {
                    System.out.println("Usage: java InviaFileServerCon [serverPort>1024]");
                    System.exit(1);
                }
            } else {
                System.out.println("Usage: java InviaFileServerCon port");
                System.exit(1);
            }
        } catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("Usage: java InviaFileServerCon port");
            System.exit(1);
        }

        ServerSocket serverSocket = null;
        Socket clientSocket = null;

        try {
            serverSocket = new ServerSocket(port);
            serverSocket.setReuseAddress(true);
            System.out.println("InviaFileServerCon: avviato ");
            System.out.println("Server: creata la server socket: " + serverSocket);
        } catch (Exception e) {
            System.err.println("Server: problemi nella creazione della server socket: " + e.getMessage());
            e.printStackTrace();
            serverSocket.close();
            System.exit(1);
        }

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
                    new InviaFileServerThread(clientSocket).start();
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