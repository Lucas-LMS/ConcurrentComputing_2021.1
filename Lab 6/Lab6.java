/*
     
    Laboratorio 6
    Aluno: Lucas Moreno Silva
    DRE: 119140949
    Prof.: Silvana Rosseto
    Computacao Concorrente
*/
     

import java.util.Random;


class Counter {
  private int total = 0; //variavel para armazenar o n de pares no array

  //realizando a exclusao muttua para garantir a condicao de corrida
  public synchronized void totalPar() {
    this.total++;
  }

  public synchronized int getTotal() {
    return this.total;
  }
}

// Runnable
class paridade implements Runnable {
  private int idThread; // identificador da thread
  private int nThreads; // total de threads
  private int[] array; // array de inteiros
  private Counter counter; // objeto compartilhado com outras threads

  // Construtor
  public paridade(int idThread, int nThreads, int[] array, Counter counter) {
    this.idThread = idThread; //id das threads
    this.nThreads = nThreads; //n de threads
    this.array = array; //array de inteiros
    this.counter = counter; //contador
  }

  /*teste padrao de divisibilidade por 2, verificacao de paridade, caso sim, adiciona ao
  vetor*/  
  @Override
  public void run() {
    for (int i = idThread; i < array.length; i += nThreads) {
      
      if (array[i] % 2 == 0) this.counter.totalPar();
    }
  }
}


class Main {
  static final int SIZE = 1000000;
  static final int NTHREADS = 4;

  //realizando a contagem de n pares de foma sequencial
  static int parSeq(int[] array) {
    int totalParSeq = 0;
    for (int i = 0; i < array.length; i++) {
      if (array[i] % 2 == 0)
        totalParSeq++;
    }
    return totalParSeq;
  }


  public static void main(String[] args) {
    //alocacao dos vetores  
    Thread[] threads = new Thread[NTHREADS];
    int[] array = new int[SIZE]; 

    
    Counter counter = new Counter();

    // atraves de um gerador aleatorio, inicializa um array de inteiros
    Random rand = new Random();
    for (int i = 0; i < SIZE; i++) {
      array[i] = rand.nextInt();
    }

    // cria a thread a partir do runnable
    for (int i = 0; i < threads.length; i++) {
      threads[i] = new Thread(new EvenNumbers(i, NTHREADS, array, counter));
    }

    // Inicia as threads
    for (int i = 0; i < threads.length; i++) {
      threads[i].start();
    }

    //realiza o join e aguarda o termino da execucao das threads
    for (int i = 0; i < threads.length; i++) {
      try {
        threads[i].join();
      } catch (InterruptedException e) {
        System.err.println(e);
        return;
      }
    }

    // teste de corretude
     if (totalParSeq == totalConc) {
      System.out.println("A funcao concorrente encontrou o valor correto!");
    } else {
      System.out.println("A funcao concorrente encontrou O valor errado!");
    }
  
  }
}