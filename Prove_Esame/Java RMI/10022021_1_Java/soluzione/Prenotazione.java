import java.io.Serializable;

public class Prenotazione implements Serializable{
    
    private String identificatore;
    private String data;
    private int giorni;
    private String modello;
    private int costoGiornaliero;
    private String nomeFileFoto;

    public Prenotazione(){
        this.identificatore="L";
        this.data="-1/-1/-1";
        this.giorni=-1;
        this.modello="-1";
        this.costoGiornaliero=-1;
        this.nomeFileFoto="L";
    }

    public Prenotazione(String id,String data,int giorni,String modello,int costo,String foto){
        this.identificatore=id;
        this.data=data;
        this.giorni=giorni;
        this.modello=modello;
        this.costoGiornaliero=costo;
        this.nomeFileFoto=foto;
    }

    public String getIdentificatore() {
        return identificatore;
    }

    public String getData() {
        return data;
    }

    public int getGiorni() {
        return giorni;
    }

    public String getModello() {
        return modello;
    }

    public int getCostoGiornaliero() {
        return costoGiornaliero;
    }

    public String getNomeFileFoto() {
        return nomeFileFoto;
    }

    public void setIdentificatore(String identificatore) {
        this.identificatore = identificatore;
    }

    public void setData(String data) {
        this.data = data;
    }

    public void setGiorni(int giorni) {
        this.giorni = giorni;
    }

    public void setModello(String modello) {
        this.modello = modello;
    }

    public void setCostoGiornaliero(int costoGiornaliero) {
        this.costoGiornaliero = costoGiornaliero;
    }

    public void setNomeFileFoto(String nomeFileFoto) {
        this.nomeFileFoto = nomeFileFoto;
    }

    public void impostaData(int anno,int mese,int giorno){
        this.data = anno + "/" + mese + "/" + giorno;
    }
}
