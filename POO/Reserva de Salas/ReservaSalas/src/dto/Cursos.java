package dto;

public enum Cursos {
    CIVIL{
        @Override
        public String toString(){
            return "Civil";
        }
    }, ELETRICA{
        @Override
        public String toString(){
            return "Eletrica";
        }
    }, PRODUCAO{
        @Override
        public String toString(){
            return "Produção";
        }
    }, MECANICA{
        @Override
        public String toString(){
            return "Mecânica";
        }
    }, COMPUTACAO{
        @Override
        public String toString(){
            return "Computação";
        }
    }, TADS{
        @Override
        public String toString(){
            return "TADS";
        }
    }, FISICA{
        @Override
        public String toString(){
            return "Física";
        }
    }, MATEMATICA{
        @Override
        public String toString(){
            return "Matemática";
        }
    }, 
    QUIMICA{
        @Override
        public String toString(){
            return "Química";
        }
    }, COMPUTAÇÃOAPLICADA{
        @Override
        public String toString(){
            return "Computação Aplicada";
        }
    }, ENGENHARIAMATERIAIS{
        @Override
        public String toString(){
            return "Engenharia de Materiais";
        }
    }, QUIMICAAPLICADA{
        @Override
        public String toString(){
            return "Química aplicada";
        }
    };
}
