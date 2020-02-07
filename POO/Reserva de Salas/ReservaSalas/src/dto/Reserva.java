
package dto;


public class Reserva {
    private Usuario usuario;
    private Bloco bloco;
    private Sala sala;
    private String periodo;
    private String data;
    private String codigo;

    public Reserva(Usuario u, Bloco b, Sala s, String periodo, String data, String codigo){
        usuario = u;
        bloco = b;
        sala = s;
        this.periodo = periodo;
        this.data = data;
        this.codigo = codigo;
    }
    
    public Reserva(){};
    
    public Sala getSala(){
        return sala;
    }
    
    public String getPeriodo(){
        return periodo;
    }
    
    public String getData(){
        return data;
    }
    
    public Usuario getUsuario(){
        return usuario;
    }
    
    public Bloco getBloco(){
        return bloco;
    }
    
    public String getCodigo(){
        return codigo;
    }
    
}
