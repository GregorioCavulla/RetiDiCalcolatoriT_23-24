import java.net.ServerSocket;
import java.net.Socket;

public class ServerStream {

    public static final int DF_PORT = 1050; // default port

    // main del server stream
    public static void main(String[] args){

        int port = -1;

        //CONTROLLO ARGOMENTI 
        try {
            if (args.length == 1) {
                port = Integer.parseInt(args[0]);
                if (port < 1024 || port > 65535) {
                    System.out.println("[ERROR] la porta deve essere compresa nel range valido [1024-65535]");
                    System.exit(1);
                }
            } else if (args.length == 0) {
                System.out.println("[DEBUG] Server avviato con porta a default");
                port = DF_PORT;
            } else {
                System.out.println("[ERROR] Usage: java InviaFileServerCon <port>");
                System.exit(1);
            }
        }//try
        catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.err.println("Java Server porta");
            System.exit(1);
        }
        
        ServerSocket serverSocket = null;
        Socket clientSocket = null;

        // CREAZIONE SERVER SOCKET
        try{
            serverSocket = new ServerSocket(port);
            serverSocket.setReuseAddress(true);
            System.out.println("Server: avviato ");
            System.out.println("Server: creata la server socket: " + serverSocket);
        } catch (Exception e ){
            System.err.println("Server: problemi nella creazione della server socket: " + e.getMessage());
            e.printStackTrace();
            serverSocket.close();
            System.exit(1);
        }

        //CICLO DI VITA DEL SERVER
        try{
            while(true){
                System.out.println("Server: in attesa di richieste...\n");

                try {
                    // bloccante!!! fino a quando non avviene una connessione
                    clientSocket = serverSocket.accept();
                    clientSocket.setSoTimeout(600000); // 10 minuti
                    System.out.println("Server: connessione accettata: " + clientSocket);
                } catch (Exception e) {
                    System.err.println("Server: problemi nella accettazione della connessione: " + e.getMessage());
                    e.printStackTrace();
                    continue;
                }

                //SERVE PER GESTIRE PIU' RICHIESTE CONCORRENTEMENTE
                StreamThread serverThread = new StreamThread(clientSocket);
                serverThread.start();
            } // while true
            
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Server: termino...");
            System.exit(2);
        }
    }
}