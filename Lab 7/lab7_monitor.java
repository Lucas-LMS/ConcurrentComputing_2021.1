/*
     
    Laboratorio 7 -- geral monitor class
    Aluno: Lucas Moreno Silva
    DRE: 119140949
    Prof.: Silvana Rosseto
    Computacao Concorrente
*/

public class Monitor {
    private int n;
    private int reader;
    private int writer;

    public Monitor(){
        this.reader = 0;
        this.writer = 0;
        this.n = 0;
    }

    public synchronized int getN() {
        return n;
    }

    public synchronized void setN(int n) {
        this.n = n;
    }

    // Entrada para Writer (escritor)
    public synchronized void WriterIn (int id) {
        try {
            while ((this.reader > 0) || (this.writer > 0)) {
                System.out.println ("X Bloqueando W (E-Escritor)"+id);
                wait();  
            }
            // registrando a escrita
            this.writer++; 
            System.out.println ("Atenção!! Escrevendo W (E-Escritor)"+id);
        } catch (InterruptedException e) { }
    }

    // Saida para Writer (escritor)
    public synchronized void WriterOut (int id) {
        // registrando o fim da escrita
        this.writer--; 
        notify(); // libera aqueles que estiverem bloquado
        System.out.println (">> Saindo W (E-Escritor)"+id);
    }

    // Entrada para Reader (leitor)
    public synchronized void ReaderIn (int id) {
        try {
            while (this.writer > 0) {
                System.out.println ("X Bloqueando R (L-Leitor)"+id);
                wait(); // bloqueia seguindo a condicional
            }
            // registrando a leitura
            this.reader++; 
            System.out.println ("Atenção! Lendo R (L-Leitor)"+id);
        } catch (InterruptedException e) { }
    }

    // Saida para Reader (leitor)
    public synchronized void ReaderOut (int id) {
        // registrando o fim da leitura
        this.reader--; 
        if (this.reader == 0)
            this.notifyAll(); 
        System.out.println (">> Saindo L"+id);
    }


    // Entrada para Reader and Writer (leitor e escritor)
    public synchronized void ReaderWriterIn (int id) {
        try {
            while ((this.reader > 0) || (this.writer > 0)) {
                System.out.println ("X Bloqueando Reader and Writer (LE-Leitor e Escritor)"+id);
                wait();  
            }
            // registrando a escrita (a leitura ocorre simultaneamente)
            this.writer++; 
            System.out.println ("Atenção! Escrevendo Reader and Writer (LE-Leitor e Escritor)"+id);
        } catch (InterruptedException e) { }
    }

    // Saida para Reader and Writer (leitor e escritor)
    public synchronized void ReaderWriterOut (int id) {
        // registrando o fim da escrita (assim como da leitura)
        this.writer--; 
        notifyAll(); 
        System.out.println (">> Saindo Reader and Writer (LE-Leitor e Escritor)"+id);
    }
}