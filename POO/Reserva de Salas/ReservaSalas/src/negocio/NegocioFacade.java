
package negocio;

import dao.DAOCollections;
import dao.DAOInterface;
import dto.Bloco;
import dto.Reserva;
import dto.Sala;
import dto.Usuario;


public class NegocioFacade {
    
    static DAOInterface dao = DAOCollections.getInstance();
    
    public static boolean cadastrarUsuario( Usuario u ){
        Usuario[] usuarios = dao.resgatarUsuarios();
        for( Usuario aux : usuarios ){
            if( aux.getCPF().equals(u.getCPF() ))
                return false;
        }
        dao.cadastrarUsuario(u);
        return true;
    }
    
    public static Usuario[] resgatarUsuarios(){
        return dao.resgatarUsuarios();
    }
    
    public static Usuario consultarUsuario(String cpf) {
        return dao.consultarUsuario(cpf);
    }
    

    public static boolean alterarUsuario(Usuario u, String cpf){
        return dao.alterarUsuario(u, cpf);
    }
    
   
    public static boolean excluirUsuario(String cpf){
        return dao.excluirUsuario(cpf);
    }
    
    public static boolean inserirBloco(Bloco b){
        return dao.inserirBloco(b);
    }
    
    public static Bloco[] resgatarBlocos(){
        return dao.resgatarBlocos();
    }
    
    public static Sala[] resgatarSalas(Bloco b){
        return dao.resgatarSalas(b);
    }
    
    public static Sala regatarSala(Bloco b, int n){
        return dao.regatarSala(b, n);
    }
    
    public static boolean fazerReserva(Reserva r){
        return dao.fazerReserva(r);
    }
    
    public static Reserva[] regatarReservas(){
        return dao.resgatarReservas();
    }
    
    public static Reserva consultarReserva(String codigo){
        return dao.consultarReserva(codigo);
    }
    
    public static boolean excluirReserva(String codigo){
        return dao.excluirReserva(codigo);
    }
    
}
