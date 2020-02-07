package dao;
import dto.Aluno;
import dto.Bloco;
import dto.Mestrando;
import dto.Reserva;
import dto.Sala;
import dto.Servidor;
import dto.Usuario;
import java.util.ArrayList;


public class DAOCollections implements DAOInterface {
    
    private static DAOCollections instance = null;
    private ArrayList<Usuario> usuarios;
    private ArrayList<Bloco> blocos;
    private ArrayList<Reserva> reservas;

    private DAOCollections() {
        usuarios = new ArrayList();
        blocos = new ArrayList();
        reservas = new ArrayList();
    }

    public static synchronized DAOCollections getInstance() {
        if (instance == null)
            instance = new DAOCollections();
        return instance;
    }

    @Override
    public boolean cadastrarUsuario(Usuario u) {
        boolean res = usuarios.add(u);
        return res;
    }

    //@Override
    public boolean alterarUsuario(Usuario u, String cpf){
        for(Usuario aux : usuarios){
            if(aux.getCPF().equals(cpf)){
                aux.setNome(u.getNome());
                aux.setSobrenome(u.getSobrenome());
                aux.setDataNascimento(u.getDataNascimento());
                if(aux instanceof Servidor){
                    ((Servidor) aux).setCargo(((Servidor) u).getCargo());
                    ((Servidor) aux).setIdentificacao(((Servidor) u).getIdentificacao());
                }else{//Aluno ou Graduando
                        ((Aluno) aux).setCurso(((Aluno) u).getIndexCurso());
                        ((Aluno) aux).setMatricula(((Aluno) u).getMatricula());
                        ((Aluno) aux).setSemestreInicio(((Aluno) u).getSemestreInicio());
                    }
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean excluirUsuario(String cpf) {
        for( Usuario u : usuarios ){
            if( u.getCPF().equals(cpf))
                return usuarios.remove( u );
        }
        return false;
    }

    @Override
    public Usuario consultarUsuario(String cpf) {
        for( Usuario u : usuarios ){
            if( u.getCPF().equals(cpf))
                return u;
        }
        return null;
    }
    
    @Override
    public Usuario[] resgatarUsuarios(){
        Usuario[] u = new Usuario[ usuarios.size() ];
        usuarios.toArray(u);
        return u;
    }
    
    @Override
    public boolean inserirBloco(Bloco b) {
        boolean res = blocos.add(b);
        return res;
    }
    
    @Override
    public Bloco[] resgatarBlocos(){
        Bloco[] b = new Bloco[ blocos.size() ];
        blocos.toArray(b);
        return b;
    }
    
    @Override
    public Sala[] resgatarSalas(Bloco b){
        Sala[] s = new Sala[ b.getSalas().size() ];
        b.getSalas().toArray(s);
        return s;
    }
    
    @Override
    public boolean fazerReserva(Reserva r) {
        boolean res = reservas.add(r);
        return res;
    }
    
    @Override
    public Reserva consultarReserva(String codigo) {
        for( Reserva r : reservas ){
            if( r.getCodigo().equals(codigo))
                return r;
        }
        return null;
    }
    
    @Override
    public Reserva[] resgatarReservas(){
        Reserva[] r = new Reserva[ reservas.size() ];
        reservas.toArray(r);
        return r;
    }
    
    @Override
    public Sala regatarSala(Bloco b, int n){
        Sala[] salas = negocio.NegocioFacade.resgatarSalas(b);
        for(Sala s : salas){
            if(s.getNumero()==n)
                return s;
        }
        return null;
    }
    
    @Override
    public boolean excluirReserva(String codigo) {
        for( Reserva r : reservas ){
            if( r.getCodigo().equals(codigo))
                return reservas.remove( r );
        }
        return false;
    }
    
}
