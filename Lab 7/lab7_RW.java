/*
     
    Laboratorio 7 -- Reader and Writer (leitor e escritor) version
    Aluno: Lucas Moreno Silva
    DRE: 119140949
    Prof.: Silvana Rosseto
    Computacao Concorrente
*/

// thread ReaderWriter (Escritora e Leitora)
class ReaderWriter extends Thread{
    int id;
    Monitor monitor;
    int Reader_Writer;
    int IThreads;

    // Construtor

    ReaderWriter(int id, Monitor monitor, int Reader_Writer, int IThreads) {
        this.id = id; // identificador da thread
        this.monitor = monitor;
        this.Reader_Writer = Reader_Writer;
        this.IThreads = IThreads;
    }

    // Metodo executado pela thread
    @Override
    public void run() {
        try{
            for(int i = 0; i < IThreads; i++) {
                this.monitor.ReaderWriterIn(this.id);
                int n = this.monitor.getN();
                // verificacao de paridade do numero
                if (n % 2 == 0) {
                    System.out.println(";LE" + id + " numero " + n + " eh par");
                } else {
                    System.out.println(";LE" + id + " numero " + n + " eh impar");
                }
                System.out.println("$LE" + id + " Sobreescrevendo para " + (n * 2));
                this.monitor.setN(n * 2);
                this.monitor.ReaderWriterOut(this.id);
                sleep(Reader_Writer);
            }
        }catch (InterruptedException e){

        }
    }
}

class Main {
	public static final int IThreads = 10; // n interacoes das threads, evitar execucao infinita
	private static final int Reader_Writer = 5; // n threads leitoras e escritoras

	public static void main(String[] args) {
		Monitor monitor = new Monitor(); // objeto compartilhado entre, tanto os leitores quando os escritores
		ReaderWriter[] readersWriters = new ReaderWriter[Reader_Writer]; // leitores e escritores

		// Criando os leitores/escritores
		for (int i = 0; i < RW; i++) {
			readersWriters[i] = new ReaderWriter(i, monitor, IThreads, Reader_Writer);
			readersWriters[i].start();
		}
	}
}