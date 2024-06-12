
/**
 * Dato.java
 * classe per la gestione dei dati da inviare al server RMI in caso di strutture
 * dati complesse (e.g. Noleggio sci/Prenotazioni auto)
 */

public class Dato {
    // Attributi
    private String nome;
    private String stato;
    private String Utente1;
    private String Utente2;
    private String Utente3;
    private String Utente4;
    private String Utente5;

    public Dato() { // costruttore con init a valori di default
        this.nome = "L";
        this.stato = "L";
        this.Utente1 = "L";
        this.Utente2 = "L";
        this.Utente3 = "L";
        this.Utente4 = "L";
        this.Utente5 = "L";
    }

    public Dato(String nome, String stato, String Utente1, String Utente2, String Utente3, String Utente4,
            String Utente5) { // costruttore con init a valori passati
        this.nome = nome;
        this.stato = stato;
        this.Utente1 = Utente1;
        this.Utente2 = Utente2;
        this.Utente3 = Utente3;
        this.Utente4 = Utente4;
        this.Utente5 = Utente5;
    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public String getStato() {
        return stato;
    }

    public void setStato(String stato) {
        this.stato = stato;
    }

    public String getUtente1() {
        return Utente1;
    }

    public void setUtente1(String utente1) {
        Utente1 = utente1;
    }

    public String getUtente2() {
        return Utente2;
    }

    public void setUtente2(String utente2) {
        Utente2 = utente2;
    }

    public String getUtente3() {
        return Utente3;
    }

    public void setUtente3(String utente3) {
        Utente3 = utente3;
    }

    public String getUtente4() {
        return Utente4;
    }

    public void setUtente4(String utente4) {
        Utente4 = utente4;
    }

    public String getUtente5() {
        return Utente5;
    }

    public void setUtente5(String utente5) {
        Utente5 = utente5;
    }

    @Override
    public String toString() {
        return "|" + nome + "|" + stato + "|" + Utente1 + "|" + Utente2 + "|" + Utente3 + "|" + Utente4 + "|" + Utente5
                + "|";
    }

}
