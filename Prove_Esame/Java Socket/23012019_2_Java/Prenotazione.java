public class Prenotazione {
    private String targa;
    private String patente;
    private String tipoVeicolo;
    private String imgFolder;

    public Prenotazione(String targa, String patente, String tipoVeicolo, String imgFolder) {
        this.targa = targa;
        this.patente = patente;
        this.tipoVeicolo = tipoVeicolo;
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

    public String getTipoVeicolo() {
        return tipoVeicolo;
    }

    public void setTipoVeicolo(String tipoVeicolo) {
        this.tipoVeicolo = tipoVeicolo;
    }

    public String getImgFolder() {
        return imgFolder;
    }

    public void setImgFolder(String imgFolder) {
        this.imgFolder = imgFolder;
    }

    

}

