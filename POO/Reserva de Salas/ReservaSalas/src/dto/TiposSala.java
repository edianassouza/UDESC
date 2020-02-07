
package dto;


public enum TiposSala {
    NORMAL{
        @Override
        public String toString(){
            return "Normal";
        }
    }, LABORATORIO{
        @Override
        public String toString(){
            return "Laboratorio";
        }
    };
}
