
/**
 * Dato.java
 * classe per la gestione dei dati da inviare al server RMI in caso di strutture
 * dati complesse (e.g. Noleggio sci/Prenotazioni auto)
 */

public class Dato {
    // Attributi
    private String id;
    private int data[];
    private String dataString;
    private int giorni;
    private String modello;
    private double costoGiornaliero;
    private String nomeFileFoto;

    public Dato() { // costruttore con init a valori di default
        // costruttore di default
        this.id = "L";
        this.data = new int[3];
        this.data[0] = -1;
        this.data[1] = -1;
        this.data[2] = -1;
        this.dataString = this.data[0] + "/" + this.data[1] + "/" + this.data[2];
        this.giorni = -1;
        this.modello = "-1";
        this.costoGiornaliero = -1;
        this.nomeFileFoto = "L";

    }

    public Dato(String id, int[] data, int giorni, String modello, double costoGiornaliero, String nomeFileFoto) {
        this.id = id;
        this.data = data;
        this.dataString = data[0] + "/" + data[1] + "/" + data[2];
        this.giorni = giorni;
        this.modello = modello;
        this.costoGiornaliero = costoGiornaliero;
        this.nomeFileFoto = nomeFileFoto;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public int[] getData() {
        return data;
    }

    public void setData(int[] data) {
        this.data = data;
        this.setDataString(data[0] + "/" + data[1] + "/" + data[2]);
    }

    public String getDataString() {
        return dataString;
    }

    public void setDataString(String dataString) {
        this.dataString = dataString;
    }

    public int getGiorni() {
        return giorni;
    }

    public void setGiorni(int giorni) {
        this.giorni = giorni;
    }

    public String getModello() {
        return modello;
    }

    public void setModello(String modello) {
        this.modello = modello;
    }

    public double getCostoGiornaliero() {
        return costoGiornaliero;
    }

    public void setCostoGiornaliero(double costoGiornaliero) {
        this.costoGiornaliero = costoGiornaliero;
    }

    public String getNomeFileFoto() {
        return nomeFileFoto;
    }

    public void setNomeFileFoto(String nomeFileFoto) {
        this.nomeFileFoto = nomeFileFoto;
    }

    @Override
    public String toString() {
        return "|" + id + "|" + dataString + "|" + giorni + "|" + modello + "|" + costoGiornaliero + "|" + nomeFileFoto
                + "|";

    }

}
