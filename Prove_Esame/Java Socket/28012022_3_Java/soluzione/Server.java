/*
 * Server.java
 */

import java.io.*;
import java.net.*;
 
class ServerThread extends Thread {
    private Socket clientSocket = null;
 
    public ServerThread(Socket clientSocket) {
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

                String servizio,parola,nomeFile,line,nuovaLinea,dirName;
                String[] parole;
                File fileInput,fileOutput,dir;
                int res,nFiles,nread;
                BufferedReader br;
                BufferedWriter bw;
                long soglia,dimFile,nByte;
                FileInputStream fileStream;
                byte [] buffer = new byte[4096];
 
                while ((servizio = inSock.readUTF()) != null) {
                    if(servizio.equalsIgnoreCase("E")){
                        //Eliminazione occorrenze di una parola in un file
 
                        //Leggo parola
                        parola = inSock.readUTF();
                        System.out.println("Richiesta parola " + parola);
 
                        //Leggo nome file
                        nomeFile = inSock.readUTF();
                        System.out.println("Richiesto file " + nomeFile);

                        //Leggo nel file
                        res=0;
                        try{
                            fileInput = new File(nomeFile);
                            fileOutput = new File("temp.txt");

                            br = new BufferedReader(new FileReader(fileInput));
                            bw = new BufferedWriter(new FileWriter(fileOutput));

                            while((line = br.readLine()) != null){
                                parole = line.split("\\s+");

                                for(String p:parole){
                                    if(p.equals(parola)){
                                        res++;
                                    }
                                }

                                nuovaLinea = line.replaceAll(parola,"");
                                bw.write(nuovaLinea);
                                bw.newLine();
                            }

                            br.close();
                            bw.close();
                            
                            if(fileInput.delete() && fileOutput.renameTo(fileInput)){
                                System.out.println("Eliminate le occorrenze della parola " + parola + " nel file " + nomeFile);
                            }else{
                                System.out.println("Errore nella modifica del file");
                                res=-1;
                                br.close();
                                bw.close();
                                fileOutput.delete();
                            }

                        }catch(IOException e){
                            e.printStackTrace();
                            res = -1;
                        }

                        //Scrivo il numero di parole eliminate
                        outSock.writeInt(res);
                    }else if(servizio.equalsIgnoreCase("T")){
                        //Trasferisco i file con byte maggiori di una certa soglia
                        System.out.println("Richiesto il trasferimento di file");
 
                        //Leggo direttorio 
                        dirName = inSock.readUTF();

                        //Leggo soglia
                        soglia = inSock.readLong();

                        dir = new File(dirName);
 
                        if(!dir.exists() || !dir.canRead() || !dir.isDirectory()){
                            //dir non esiste o non è valida
                            outSock.writeBoolean(false);
                            continue;
                        }
 
                        //dir esiste ed è valida
                        outSock.writeBoolean(true);
 
                        //Conto il numero di file con bute>soglia presenti in dir
                        nFiles=0;
                        for(File f:dir.listFiles()){
                            if(f.canRead() && f.isFile()){
                                dimFile = f.length();
                                if(dimFile > soglia){
                                    nFiles++;
                                }
                            }
                        }
 
                        //Invio numero file
                        outSock.writeInt(nFiles);
 
                        for(File f:dir.listFiles()){
                            if(f.canRead() && f.isFile() && (f.length()>soglia)){
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
 
public class Server {
 
    public static void main(String[] args) throws IOException {
        int port = -1;
 
        try {
            if (args.length == 1) {
                port = Integer.parseInt(args[0]);
                // controllo che la porta sia nel range consentito 1024-65535
                if (port < 1024 || port > 65535) {
                    System.out.println("Usage: java Server [serverPort>1024]");
                    System.exit(1);
                }
            } else {
                System.out.println("Usage: java Server port");
                System.exit(1);
            }
        } catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("Usage: java Server port");
            System.exit(1);
        }
 
        ServerSocket serverSocket = null;
        Socket clientSocket = null;
 
        try {
            serverSocket = new ServerSocket(port);
            serverSocket.setReuseAddress(true);
            System.out.println("Server: avviato ");
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
                    new ServerThread(clientSocket).start();
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