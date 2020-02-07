
package dao;

import dto.Bloco;
import dto.Reserva;
import dto.Sala;
import dto.Usuario;


public interface DAOInterface {
    boolean cadastrarUsuario( Usuario u );
    boolean alterarUsuario( Usuario u, String cpf );
    boolean excluirUsuario( String cpf );
    Usuario consultarUsuario( String cpf );
    Usuario[] resgatarUsuarios();
    public boolean inserirBloco(Bloco b);
    public Bloco[] resgatarBlocos();
    public Sala[] resgatarSalas(Bloco b);
    public boolean fazerReserva(Reserva r);
    public Reserva consultarReserva(String codigo);
    public Reserva[] resgatarReservas();
    public Sala regatarSala(Bloco b, int n);
    public boolean excluirReserva(String codigo);
}

