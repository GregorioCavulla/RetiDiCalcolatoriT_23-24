import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

// Consumatore e' un filtro
public class Consumatore {
	public static void main(String[] args) {
		FileReader fileReader = null;
        String regex = args[1];
		char ch;
		int x;
		
		if (args.length != 2){
			System.out.println("Utilizzo: consumatore <inputFilename> <caratteri da rimuovere>");
			System.exit(0);
		}
	  
		try {
			fileReader = new FileReader(args[0]);
		} catch(FileNotFoundException e){
			System.out.println("File non trovato");
			System.exit(1);
		}
		try {

			while ((x = fileReader.read()) >= 0) { 
				ch = (char) x;
                    if(regex.indexOf(ch) == -1){ // se il xitcarattere non è presente nella stringa regex
                        System.out.print(ch);
                    }
			}
			fileReader.close();
		} catch(IOException ex){
			System.out.println("Errore di input");
			System.exit(2);
		}
}}

// done
