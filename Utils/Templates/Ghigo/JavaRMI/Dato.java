package RMI;


public class Dato {
    private String id;
    private String tipo;
    private String luogo;
    private int data[];
    private int x;
    private String nomeFile;


    public Dato (){
        id="L";
        tipo="L";
        luogo="L";
        data=new int [3];
        data[0]=-1;
        data[1]=-1;
        data[2]=-1;
        x=-1;
        nomeFile="L";
    }
    

    public Dato(String id, String tipo, String luogo, int giorno, int mese, int anno, int x, String nomeFile) {
        this.id = id;
        this.tipo = tipo;
        this.luogo = luogo;
        this.data[0] = giorno;
        this.data[1] = mese;
        this.data[2] = anno;
        this.x = x;
        this.nomeFile = nomeFile;
    }


    @Override
    public String toString() {
        return id + "\t||" + tipo+ "\t||" + luogo + "\t||"+ data[0] + "/" + data[1] + "/" + data[2] +"\t||"  + x +"\t\t||" + nomeFile;
    }
    

  
}
