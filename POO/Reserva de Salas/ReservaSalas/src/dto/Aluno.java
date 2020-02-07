
package dto;

public class Aluno extends Usuario{
    private String matricula; 
    private String semestreInicio;
    public Cursos curso;
    public int indexCurso;
    
    public Aluno (String nome, String sobrenome, String dataNascimento, String CPF, String matricula, String semestreInicio, int n){
        super (nome, sobrenome, dataNascimento, CPF);
        setMatricula(matricula);
        setSemestreInicio(semestreInicio);
        setCurso(n);
    }
    

    public String getMatricula() {
        return matricula;
    }
    
    public void setMatricula(String matricula){
        this.matricula = matricula;
    }

    public String getSemestreInicio() {
        return semestreInicio;
    }    
    
    public void setSemestreInicio(String semestreInicio){
        this.semestreInicio = semestreInicio;
    }
    
    public Cursos getCurso(){
        return curso;
    }
    
    public void setCurso(int i){
        Cursos c = Cursos.CIVIL;
        switch(i){
            case 0: c=Cursos.CIVIL;
                    break;
            case 1: c=Cursos.ELETRICA;
                    break;
            case 2: c=Cursos.PRODUCAO;
                    break;
            case 3: c=Cursos.MECANICA;
                    break;
            case 4: c=Cursos.COMPUTACAO;
                    break;
            case 5: c=Cursos.TADS;
                    break;
            case 6: c=Cursos.FISICA;
                    break;
            case 7: c=Cursos.MATEMATICA;
                    break;
            case 8: c=Cursos.QUIMICA;
                    break;
        }
        this.curso = c;
        this.indexCurso = i;
    }

    public int getIndexCurso(){
        return this.indexCurso;
    }
 
}
