
package dto;

import java.util.ArrayList;


public class Bloco {
    private String letra;
    private int numeroSalas;
    private ArrayList<Sala> salas;
    
    public Bloco(String letra, int numeroSalas){
        salas = new ArrayList<>();
        setLetra(letra);
        setNumeroSalas(numeroSalas);
        boolean res = true;
        for(int i=0; i<(numeroSalas/2); i++){
            int n = i+1;
            Sala s = new Sala(n, 0);//Normal
            res = salas.add(s);
        }
        for(int i=numeroSalas/2; i<(numeroSalas); i++){
            int n = i+1;
            Sala s = new Sala(n, 1);//Laboratório
            res = salas.add(s);
        }
    }
    
    public String getLetra(){
        return letra;
    }
    
    public void setLetra(String letra){
        this.letra = letra;
    }
    
    public int getNumeroSalas(){
        return numeroSalas;
    }
    
    public void setNumeroSalas(int numerosSalas){
        this.numeroSalas = numeroSalas;
    }
    
    public ArrayList<Sala> getSalas(){
        return this.salas;
    }
    
}
