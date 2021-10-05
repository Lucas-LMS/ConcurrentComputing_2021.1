/*
     
    Laboratorio 9
    Aluno: Lucas Moreno Silva
    DRE: 119140949
    Prof.: Silvana Rosseto
    Computacao Concorrente
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define Tamanho_Buffer 10

//variaveis para a mutual exclusion e uso do semaforo
sem_t fullSlots, emptySlots;
sem_t mutexP, mutexC;

int buffer[Tamanho_Buffer], index, P, C; 
// indice para produtores e n de threads produtoras/consumidoras


// controle do semaforo para o produtor
void insere(int elemento, int id){

  // produtor pronto pra iniciar e o semaforo pra bloquear 
  printf("Produtor %d esta tentando produzir\n", id);
  sem_wait(&mutexP);
  sem_wait(&emptySlots);

  printf("Produtor %d produzindo na posição %d\n", id, index);
  buffer[index] = elemento;
  index = (index + 1) % Tamanho_Buffer;

  // semaforo libera o que estava bloqueado
  sem_post(&fullSlots);
  sem_post(&mutexP);
}


// controle do semaforo para o cosumidor
int* saiu(int id){
  int *saida = (int *)malloc(sizeof(int) * N);
  printf("Consumidor %d esta tentando consumir\n", id);
  sem_wait(&mutexC);
  for(int i = 0; i < Tamanho_Buffer; i++){
    sem_wait(&fullSlots);
    saida[i] = buffer[i];
    sem_post(&emptySlots);
  }
  printf("Consumidor %d terminou seu consumo\n", id);
  sem_post(&mutexC);
  return saida;
}

// funcao do produtor
void *produtor(void *arg){
  int id = (long long int)arg;
  int elemento;
  while(1){
    printf("Produtor %d produzindo\n", id);
    element = id;
    insere(elemento, id);
  }
  pthread_exit(NULL);  
}

// funcao do consumidor
void *consumidor(void *arg){
  int id = (long long int)arg;
  int *elementos;
  while(1){
    elementos = saiu(id);
    printf("Consumidor %d consumindo\n", id);
  }
  pthread_exit(NULL);
}

// alocacao padrao das threads e verificacao 
pthread_t* criarTid(int n) { 
  pthread_t* tid = (pthread_t *) malloc(sizeof(pthread_t) * n);
  if (tid == NULL) {
    printf("ERROR...createTid");
    exit(1);
  }
  return tid;
}

// criacao das threads com pthread_create
void criarThreads(pthread_t* tid, int n, void *task) {
  for (ll i = 0; i < n; i++) {
    if (pthread_create(tid+i, NULL, task, (void *)i)) {
      printf("ERROR...createThreads\n");
      exit(2);
    }
  }
}

// aguardando as threads terminarem e juntando elas com join
void aguardandoThreads(pthread_t* tid, int n) {
  for (int i = 0; i < n; i++) {
    if (pthread_join(tid[i], NULL)) {
      printf("ERROR...pthread_join");
      exit(3);
    }
  }
}
 
 int main(int argc, char* argv[]) {

    // validando a entrada recebida e armazenando ela
    if (argc < 3) {
		printf("Entrada invalida!\n Deve ser digitado: <o numero de threads consumidoras> <o numero de threads produtoras>\n", argv[0]);
		exit(1);
	}
	C = atoi(argv[1]);
	P = atoi(argv[2]);

	in = 0;

	// iniciando o semaforo
    sem_init(&fullSlots, 0, 0);
	sem_init(&emptySlots, 0, Tamanho_Buffer);
	sem_init(&mutex, 0, 1);

    // sequencia de criacao dos tids e threads e aguardo de finalizacao das threads 
    pthread_t* prod  = criarTid(P);
    pthread_t* cons = criarTid(C); 

    criarThreads(prod, P, produtor);
    criareThreads(cons, C, consumidor);
  
    aguardandoThreads(prod, P);
    aguardandoThreads(cons, C);

    // encerrando as variaveis usadas
    sem_destroy(&fullSlots);
    sem_destroy(&emptySlots);
    sem_destroy(&mutexCons);
    sem_destroy(&mutexProd);

    
	return 0;
}