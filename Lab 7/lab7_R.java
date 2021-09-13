/*
     
    Laboratorio 7 --Only Reader (leitor) version
    Aluno: Lucas Moreno Silva
    DRE: 119140949
    Prof.: Silvana Rosseto
    Computacao Concorrente
*/

// thread Reader (leitora)
class Reader extends Thread{
    int id;
    Monitor monitor;
    int READERS;
    int IThreads;

    // Construtor

    Reader(int id, Monitor monitor, int READERS, int IThreads) {
        this.id = id; // identificador da thread
        this.monitor = monitor;
        this.READERS = READERS;
        this.IThreads = IThreads;
    }

    // funcao que faz a vericacao de primalidade de n
    // testa sua divisibilidade 
    public boolean isPrime(int n){
        if(n == 1) return false;
        if(n == 2) return true;
        if(n % 2 == 0) return false;
        for(int i = 3; i*i < n; i+=2){
            if(n % i == 0) return false;
        }
        return true;
    }

    // Metodo executado pela thread
    @Override
    public void run() {
        try{
            for(int i = 0; i < IThreads; i++){
                this.monitor.ReaderIn(this.id);
                int n = this.monitor.getN();
                if(isPrime(n)){
                    System.out.println("*Reader"+id+" numero "+n+" eh primo");
                }else{
                    System.out.println("*Reader"+id+" numero "+n+" nao eh primo");
                }
                this.monitor.ReaderOut(this.id);
                sleep(READERS);
            }
        }catch (InterruptedException e){

        }
    }
}

class Main {
	public static final int IThreads = 10; // n interacoes da thread, evitar execucao infinita
	private static final int READERS = 5; // n threads leitoras
	

	public static void main(String[] args) {
		Monitor monitor = new Monitor(); // objeto compartilhado entre, tanto os leitores quando os escritores
		Reader[] readers = new Reader[READERS]; // leitores

		// Criando os leitores
		for (int i = 0; i < READERS; i++) {
			readers[i] = new Reader(i, monitor, IThreads, READERS);
			readers[i].start();
		}
			
	}
}
