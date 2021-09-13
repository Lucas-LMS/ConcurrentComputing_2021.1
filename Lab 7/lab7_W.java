/*
     
    Laboratorio 7 -- Only Writer (escritor) version
    Aluno: Lucas Moreno Silva
    DRE: 119140949
    Prof.: Silvana Rosseto
    Computacao Concorrente
*/

// thread Writer (escritora)
class Writer extends Thread{
    int id;
    Monitor monitor;
    int WRITERS;
    int IThreads;

    // Construtor

    Writer(int id, Monitor monitor, int WRITERS, int IThreads) {
        this.id = id; // identificador da thread
        this.monitor = monitor;
        this.WRITERS = WRITERS;
        this.IThreads = IThreads;
    }

    // Metodo executado pela thread
    @Override
    public void run() {
        try{
            for(int i = 0; i < IThreads; i++){
                this.monitor.WriterIn(this.id);
                this.monitor.setN(id);
                System.out.println("*Writer"+id+" Sobreescrevendo para "+id);
                this.monitor.WriterOut(this.id);
                sleep(WRITERS);
            }
        }catch (InterruptedException e){

        }
    }
}

class Main {
	public static final int IThreads = 10; //  n interacoes das threads, evitar execucao infinita
	private static final int WRITERS = 5; // n threads escritoras
	
	public static void main(String[] args) {
		Monitor monitor = new Monitor(); // objeto compartilhado entre, tanto os leitores quando os escritores
		Writer[] writers = new Writer[WRITERS]; // escritores
		

		// Criando os escritores
		for (int i = 0; i < WRITERS; i++) {
			writers[i] = new Writer(i, monitor, IThreads, WRITERS);
			writers[i].start();
		}

	}
}