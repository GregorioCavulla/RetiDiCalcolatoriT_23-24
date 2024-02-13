import java.io.Serializable;

public class Prenotazione implements Serializable{

    private String targa;
    private String patente;
    private String tipo;
    private String immagine;

    public Prenotazione(){
        this.targa="L";
        this.patente="0";
        this.tipo="L";
        this.immagine="L";
    }

    public Prenotazione(String targa,String patente,String tipo,String immagine){
        this.targa=targa;
        this.patente=patente;
        this.tipo=tipo;
        this.immagine=immagine;
    }

    public String getTarga(){
        return targa;
    }

    public String getPatente(){
        return patente;
    }

    public String getTipo(){
        return tipo;
    }

    public String getImmagine(){
        return immagine;
    }
}