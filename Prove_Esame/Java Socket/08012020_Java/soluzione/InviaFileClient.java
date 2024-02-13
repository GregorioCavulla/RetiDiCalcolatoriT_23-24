import java.net.*;
import java.io.*;

public class InviaFileClient {

    public static void main(String[] args) throws IOException {
        InetAddress addr = null;
        int port = -1;

        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

        try {
            if (args.length == 2) {
                addr = InetAddress.getByName(args[0]);
                port = Integer.parseInt(args[1]);
                if (port < 1024 || port > 65535) {
                    System.out.println(
                            "Usage: java InviaFileClient serverAddr serverPort");
                    System.exit(1);
                }
            } else {
                System.out.println("Usage: java InviaFileClient serverAddr serverPort");
                System.exit(1);
            }
        } catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("Usage: java InviaFileClient serverAddr serverPort");
            System.exit(2);
        }

        // Variables for sockets
        Socket socket = null;
        DataInputStream inSock = null;
        DataOutputStream outSock = null;

        try {
            socket = new Socket(addr, port);
            socket.setSoTimeout(30000);
            System.out.println("Creata la socket: " + socket);
            inSock = new DataInputStream(socket.getInputStream());
            outSock = new DataOutputStream(socket.getOutputStream());
        } catch (IOException ioe) {
            System.out.println("Problemi nella creazione degli stream su socket: ");
            ioe.printStackTrace();
            System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, solo invio per continuare: ");
            System.exit(1);
        }

        String command,carattere,occ_String,dirName,fileName;
        int occorrenze,res,numFile,nread,buffer_size = 4096;
        boolean esiste;
        long fileLength;
        FileOutputStream fileStream;
        byte [] buffer = new byte[buffer_size];

        try {
            System.out.print("Inserire (C:Conta occorrenze,B:Trasferisci file binari)");
            while (( command =  stdIn.readLine()) != null ) {
                outSock.writeUTF(command);
    
                if ( command.equals("C") ) {
                    //Conta occorrenze

                    System.out.print("Inserire carattere: ");
                    carattere = stdIn.readLine();
                    System.out.println("Inserire occorrenze: ");
                    occ_String = stdIn.readLine();

                    try{
                        occorrenze = Integer.parseInt(occ_String);
                    }catch(NumberFormatException e){
                        System.out.println("L'input non è un numero");
                        System.out.println("Inserire (C:Conta occorrenze,B:Trasferisci file binari)");
                        continue;
                    }

                    //Scrivo carattere e occorrenze
                    outSock.writeChar(carattere.charAt(0));
                    outSock.writeInt(occorrenze);

                    //Ricevo caratteri trovati
                    res = inSock.readInt();

                    System.out.println("Trovate " + res + " occorrenze del carattere " + carattere);

                }  else if ( command.equals("B") ) {
                    System.out.print("Inserire direttorio: ");
                    dirName = stdIn.readLine();
    
                    //Invio nome direttorio
                    outSock.writeUTF(dirName);
                    
                    //Leggo se il direttorio esiste ed è valido
                    esiste = inSock.readBoolean();

                    if(esiste){
                        //Ricevo numero file
                        numFile = inSock.readInt();
                        for(int i=0;i<numFile;i++){
                            //Leggo nome file
                            fileName = inSock.readUTF();

                            //Leggo lunghezza file
                            fileLength = inSock.readLong();

                            //Scrivo byte
                            fileStream = new FileOutputStream(fileName);

                            while (fileLength > 0) {
                                nread = inSock.read(buffer,0,fileLength > buffer_size ? buffer_size : (int)fileLength);
                                fileStream.write(buffer,0,nread);

                                fileLength -= nread;
                            }

                            fileStream.close();


                        }
                    } else {
                        System.out.println("Errore!, dir non esiste o non è valido");
                    }
                }
                System.out.print("Inserire (C:Conta occorrenze,B:Trasferisci file binari)");
            }

        } catch (IOException ioe) {
            System.out.println("Problemi Di Operazioni Input ");
            ioe.printStackTrace();
            socket.close();
            System.out.println("Chiudo ed esco...");
            System.exit(2);
        }
      
        socket.close();

    }
}