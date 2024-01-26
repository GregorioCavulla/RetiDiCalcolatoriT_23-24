import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;

// Consumatore e' un filtro
public class Consumatore {
	public static void main(String[] args) {
		BufferedReader in = null;
		String filterString;
		int found, c;
		char ch;


		if (args.length < 1 || args.length>2){ //Invocation error
			System.out.println("Utilizzo: consumatore <prefix> [filename]");
			System.out.println("OR");
			System.out.println("Utilizzo: consumatore <prefix> < [filename]");
			System.exit(0);
		}

        if(args.length == 1){
            		System.out.println("Esecuzione con solo prefisso.");
			System.out.println("Prefisso = "+args[0] + " di lunghezza "+args[0].length());
			
             in = new BufferedReader(new InputStreamReader(System.in));
        } else if(args.length == 2){System.out.println("Esecuzione con prefisso e file.");
			System.out.println("Prefisso = "+args[0] + " di lunghezza "+args[0].length()+
					"\nFile Path = "+args[1]);
                    try {
                    in = new BufferedReader(new FileReader(args[1]));
                } catch(FileNotFoundException e){
                    System.out.println("File non trovato");
                    System.exit(1);
                }
        }
	  

		filterString = args[0];

		try {
            found = 0;
			while ((c = in.read()) > 0) { 
                ch = (char) c;
                for(int i = 0; i<filterString.length(); i++){
                    if(ch == filterString.charAt(i)){
                        found = 1;
                    }
                }
                if(found==0){
                    System.out.print(ch);
                }
                found=0;
			}
			in.close();
		} catch(IOException ex){
			System.out.println("Errore di input");
			System.exit(2);
		}
}}

// done