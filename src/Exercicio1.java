public class Exercicio1<T> {
    public T[] elementos;
    public int topo;

    public Exercicio1() {
     elementos = (T[]) new Object[10];
        topo = -1; 
    }

        public boolean inserir(T e) {
            if (!estaCheia()) {
                topo++;
                elementos[topo] = e;
                return true;
            }
            return false;
        }
        public boolean estaVazia() {
            return topo == -1;
        }
        public boolean estaCheia() {
            return this.topo == this.elementos.length - 1;
        }
        public T remover() {
            if (!estaVazia()) {
                return elementos[topo--];
            }
            return null;
    
        }
        public int tamanho() {
            return this.elementos.length;
        }
        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder("[");
    
            for (int i = 0; i <= this.topo; i++) {
                sb.append(this.elementos[i]);
                if (i < this.topo) {
                    sb.append(", ");
                }
            }
    
            sb.append("]");
    
            return sb.toString();
        
    
    }
    

    }


