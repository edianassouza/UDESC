package dto;

public class Mestrando extends Aluno{
    
    public Mestrando(String nome, String sobrenome, String dataNascimento, String CPF, String matricula, String semestreInicio, int n) {
        super(nome, sobrenome, dataNascimento, CPF, matricula, semestreInicio, n);
        setCurso(n);
    }
    
    @Override
    public void setCurso(int i){
        Cursos c = Cursos.CIVIL;
        switch(i){
            case 9: c=Cursos.COMPUTAÇÃOAPLICADA;
                    break;
            case 10: c=Cursos.ENGENHARIAMATERIAIS;
                    break;
            case 11: c=Cursos.QUIMICAAPLICADA;
                    break;
        }
        this.curso = c;
        this.indexCurso = i;
    }
    
}
