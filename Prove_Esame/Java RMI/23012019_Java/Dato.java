/**
 * Dato.java
 * classe per la gestione dei dati da inviare al server RMI in caso di strutture
 * dati complesse (e.g. Noleggio sci/Prenotazioni auto)
 */

public class Dato implements java.io.Serializable {
    private static final long serialVersionUID = 1L; // Aggiungi un serialVersionUID

    // Attributi
    private String targa;
    private String patente;
    private String tipo;
    private String imgFolder;

    public Dato() { // costruttore con init a valori di default
        this.targa = "L";
        this.patente = "0";
        this.tipo = "L";
        this.imgFolder = "L";
    }

    public Dato(String targa, String patente, String tipo, String imgFolder) {
        this.targa = targa;
        this.patente = patente;
        this.tipo = tipo;
        this.imgFolder = imgFolder;
    }

    public String getTarga() {
        return targa;
    }

    public void setTarga(String targa) {
        this.targa = targa;
    }

    public String getPatente() {
        return patente;
    }

    public void setPatente(String patente) {
        this.patente = patente;
    }

    public String getTipo() {
        return tipo;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }

    public String getImgFolder() {
        return imgFolder;
    }

    public void setImgFolder(String imgFolder) {
        this.imgFolder = imgFolder;
    }

    @Override
    public String toString() {
        return "|" + this.targa + "|" + this.patente + "|" + this.tipo + "|" + this.imgFolder + "|";
    }
}
