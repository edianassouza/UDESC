
package dto;


public class Sala {
    private int numero;
    private TiposSala tipo;
    
    public Sala(int numero, int tipo){
        setNumero(numero);
        setTipo(tipo);
    }
    
    public int getNumero(){
        return numero;
    }
    
    public void setNumero(int n){
        this.numero = n;
    }
    
    public TiposSala getTipo(){
        return tipo;
    }
    
    public void setTipo(int n){
        if(n==0)
            this.tipo = TiposSala.NORMAL;
        else
            this.tipo = TiposSala.LABORATORIO;
    }
    
}
