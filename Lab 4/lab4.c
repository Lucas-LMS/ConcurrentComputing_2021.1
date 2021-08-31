/*
Modulo 2 - Laboratorio 4
Aluno: Lucas Moreno Silva
DRE: 119140949
Prof.: Silvana Rosseto
Computacao Concorrente
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4

//definicao das variaveis globais
int c = 0;
pthread_mutex_t mutex;
pthread_cond_t cond1, cond2;

//sequencia das tarefas de cada thread
//thread A (1)
void *A(void *t){
  pthread_mutex_lock(&mutex);

  //verificacao da condicao de execucao
  if(c == 0){
    pthread_cond_wait(&cond1, &mutex);
  }
  printf("Fique à vontade.\n\n");
  c++;
  pthread_cond_signal(&cond2);
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

//thread B (2)
void *B(void *t){
  printf("Seja bem-vindo!\n\n");
  /*como essa thread precisa ser executada primeiramente,
  nao ha verificacao de condicao e logo eh executada*/

  pthread_mutex_lock(&mutex);
  c++;
  pthread_cond_broadcast(&cond1);
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

//thread C (3)
void *C(void *t){
  pthread_mutex_lock(&mutex);

  //verificacao da condicao de execucao
  while(c != 3){
    pthread_cond_wait(&cond2, &mutex);
  }
  printf("Volte sempre!\n\n");
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}


//thread D (4)
void *D(void *t){
  pthread_mutex_lock(&mutex);

  //verificacao da condicao de execucao
  if(c == 0){
    pthread_cond_wait(&cond1, &mutex);
  }
  printf("Sente-se, por favor.\n\n");
  c++;
  pthread_cond_signal(&cond2);
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}


//alocacao e verificacao de nulidade das threads
pthread_t* createTid() {
  pthread_t* tid = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS);
  if (tid == NULL) {
    printf("ERRO--createTid");
    exit(1);
  }
  return tid;
}


int main(){
  pthread_t *tid;

  tid = init();

  //criacao das threads
  pthread_create(tid, NULL, A, NULL);
  pthread_create(tid+1, NULL, B, NULL);
  pthread_create(tid+2, NULL, C, NULL);
  pthread_create(tid+3, NULL, D, NULL);

  //aguarda as threads concluirem
  for(int i = 0; i < NTHREADS; i++){
    pthread_join(tid[i], NULL);
  }
  
  //liberacao (desalocacao) das variaveis
  free(tid);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond1);
  pthread_cond_destroy(&cond2);

  return 0;
}
