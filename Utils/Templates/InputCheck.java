import java.io.*;

public class InputCheck {

    BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

    String buffer;

    public int checkGiorno() {
        String input = "";
        boolean ok = false;
        int giorno = 0;
        while (!ok) {
            System.out.println("inserire una giorno valido [1-30]");
            try {
                input = stdIn.readLine();
                giorno = Integer.parseInt(input);
                if (giorno > 0 && giorno <= 30)
                    ok = true;
            } catch (IOException e) {
                System.out.println("[ERROR] errore di input ");
            }
        }
        return giorno;
    }

    public int checkMese() {
        String input = "";
        boolean ok = false;
        int mese = 0;
        while (!ok) {
            System.out.println("inserire una mese valido [1-12]");
            try {
                input = stdIn.readLine();
                mese = Integer.parseInt(input);
                if (mese >= 1 && mese <= 12)
                    ok = true;
            } catch (IOException e) {
                System.out.println("[ERROR] errore di input ");
            }
        }
        return mese;
    }

    public int checkAnno() {
        String input = "";
        boolean ok = false;
        int anno = 0;
        while (!ok) {
            System.out.println("inserire una anno valido (>2000) ");
            try {
                input = stdIn.readLine();
                anno = Integer.parseInt(input);
                if (anno >= 2000)
                    ok = true;
            } catch (IOException e) {
                System.out.println("[ERROR] errore di input ");
            }
        }
        return anno;
    }

    public int checkPositivo(String testo) {
        String input = "";
        boolean ok = false;
        int val = 0;
        while (!ok) {
            System.out.println("inserire " + testo);
            try {
                input = stdIn.readLine();
                val = Integer.parseInt(input);
                if (val >= 0)
                    ok = true;
            } catch (IOException e) {
                System.out.println("[ERROR] errore di input ");
            }
        }
        return val;
    }

}
