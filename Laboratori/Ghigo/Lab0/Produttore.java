import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

// Proposta, rendere produttore un filtro

public class Produttore {
	public static void main(String[] args) {		
		BufferedReader in = null; // dichiarazione di variabile in di tipo BufferedReader

		if (args.length != 1){ // se la lunghezza dell'array args è diversa da 1
			System.out.println("Utilizzo: produttore <inputFilename>");
			System.exit(0);
		}

		System.out.println("Inserisci la nuova riga, EOF per terminare");

		FileWriter fout;
		String inputl = null;
		
		try {
			fout = new FileWriter(args[0]); // inizializzazione di fout con un nuovo FileWriter che ha come parametro args[0]
            in = new BufferedReader(new InputStreamReader(System.in)); // inizializzazione di in con un nuovo BufferedReader che ha come parametro un nuovo InputStreamReader che ha come parametro System.in	
			while((inputl = in.readLine()) != null){ //till EOF (CTRL+D)
				fout.write(inputl+"\n", 0, inputl.length()+1);
			}	
			fout.close();
		}
	    catch (IOException e) { 
			e.printStackTrace();
			System.exit(2); // uscita con errore, intero positivo a livello di sistema Unix
		}
	}
}

// done