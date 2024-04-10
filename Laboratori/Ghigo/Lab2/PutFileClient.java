import java.net.*;
import java.io.*;

public class PutFileClient {
    public static void main(String[] args) throws FileNotFoundException, IOException {

        InetAddress addr = null;
        int port = -1;

        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
        String nomeFolder = null;
        int dim_min = 0;

        int buffer_size = 4096; // Default buffer size

        // Check arguments
        try {
            if (args.length == 3 || args.length == 4) { // Check if the number of arguments is correct

                addr = InetAddress.getByName(args[0]); // Get the address from the first argument
                port = Integer.parseInt(args[1]); // Get the port from the second argument

                if (port < 1024 || port > 65535) {
                    System.out.println("Ivalid port number. Please use a port number between 1024 and 65535.");
                    System.out.println(
                            "Usage: java PutFileClient <serverAddr> <serverPort> <minFileSizeInBytes> [transferBufferSize]");
                    System.exit(1);
                }
                dim_min = Integer.parseInt(args[2]); // Get the minimum file size from the third argument

                if (args.length == 4) { // Check if the user has specified a buffer size
                    buffer_size = Integer.parseInt(args[3]); // Get the buffer size from the fourth argument
                }
            } else {
                System.out.println(
                        "Usage: java PutFileClient <serverAddr> <serverPort> <minFileSizeInBytes> [transferBufferSize]");
                System.exit(1);
            }
        } catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.out
                    .println(
                            "Usage: java PutFileClient <serverAddr> <serverPort> <minFileSizeInBytes> [transferBufferSize]");
            System.exit(2);

        }

        // Variables for reading and transferring files
        byte[] buffer = new byte[buffer_size];
        int cont = 0;
        int read_bytes = 0;

        // Variables for sockets
        Socket socket = null;
        DataInputStream inSock = null;
        DataOutputStream outSock = null;

        // creo la socket
        try {
            socket = new Socket(addr, port);
            socket.setSoTimeout(30000); // Set the timeout for the socket to 30 seconds
            System.out.println("Creata la socket: " + socket);
            inSock = new DataInputStream(socket.getInputStream());
            outSock = new DataOutputStream(socket.getOutputStream());
        } catch (IOException e) {
            System.out.println("Problemi nella creazione degli stream su socket: ");
            e.printStackTrace();
            System.out.println("Chiudo ed esco...");
            System.exit(3);
        }

        // Invio il nome della cartella
        System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, altrimenti immetti il nome della cartella: ");

        // ciclo di lettura dei nomi dei file
        while ((nomeFolder = stdIn.readLine()) != null) { // Read the folder name from the user
            File dirCorr = new File(nomeFolder); // Create a new file object with the folder name
            if (dirCorr.exists() && dirCorr.isDirectory()) { // Check if the folder exists and is a directory
                File[] files = dirCorr.listFiles(); // Get the list of files in the directory
                for (int i = 0; i < files.length; i++) { // Iterate over the list of files
                    File fileCorr = files[i];
                    if (fileCorr.isFile() && dim_min <= fileCorr.length()) { // Check if the file is a regular file and
                                                                             // its size is greater than the minimum
                                                                             // size specified by the user
                        // Send the file to the server
                        System.out.println("Invio il file " + fileCorr.getName() + " di dimensione " + fileCorr.length()
                                + " bytes");
                        outSock.writeUTF(fileCorr.getName());
                        String risposta = inSock.readUTF();

                        if (risposta.equals("attiva")) { // if server has not the current file
                            System.out.println(
                                    "Il file " + fileCorr.getName() + " non esiste sul server. Invio in corso...");
                            outSock.writeLong(fileCorr.length()); // Send the file size to the server
                            DataInputStream filInputStream = new DataInputStream(new FileInputStream(fileCorr)); // Create
                                                                                                                 // a
                                                                                                                 // file
                                                                                                                 // input
                            // stream to
                            // read the file
                            // data
                            // output
                            // stream to
                            // write to
                            // the
                            // socket
                            cont = 0; // Initialize the counter
                            try {
                                while (cont < fileCorr.length()) { // Read the file and write it to the socket
                                    read_bytes = filInputStream.read(buffer);
                                    outSock.write(buffer, 0, read_bytes);
                                    cont += read_bytes;
                                }
                                outSock.flush();
                                System.out.println("Byte trasferiti: " + cont);
                            } catch (EOFException e) {
                                System.out.println("Problemi, i seguenti: ");
                                e.printStackTrace();
                            }
                            filInputStream.close();
                        } else {
                            System.out.println("Il file " + fileCorr.getName() + " esiste già sul server. Salto...");
                        }
                    } else {
                        System.out.println(
                                "Il file " + fileCorr.getName() + " non è un file regolare o è troppo piccolo.");
                    }
                } // end for
            } else {
                System.out.println("La cartella specificata non esiste o non è una cartella.");
            }
            System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, altrimenti immetti il nome della cartella: ");
        } // end while
    } // end main
} // end class