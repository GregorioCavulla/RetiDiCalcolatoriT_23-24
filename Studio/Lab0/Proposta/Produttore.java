import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

// voglio rendere produttore un Filtro, fancendo terminare l'insert delle righe con EOF (Ctrl+Z)
public class Produttore {
	public static void main(String[] args) {		
		BufferedReader in = null;
		int res = 0;
		
		if (args.length != 1){
			System.out.println("Utilizzo: produttore <inputFilename>");
			System.exit(0);
		}
        System.out.println("Inserire il contenuto del file, termina con EOF (ctrl+D)");	
		FileWriter fout;
		String inputl = null;
        try {
			fout = new FileWriter(args[0]);
            in = new BufferedReader(new InputStreamReader(System.in)); //leggo da stdin
            while((inputl = in.readLine()) != null){ //till EOF (CTRL+D)
                fout.write(inputl+"\n", 0, inputl.length()+1);
            }
			fout.close();

	    }catch (IOException e) { 
			e.printStackTrace();
			System.exit(2); // uscita con errore, intero positivo a livello di sistema Unix
		}
	}
}

// done