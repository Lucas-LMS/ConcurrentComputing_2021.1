/*
     
    Laboratorio 8
    Aluno: Lucas Moreno Silva
    DRE: 119140949
    Prof.: Silvana Rosseto
    Computacao Concorrente
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 4

//condicoes do semafaro
sem_t realizado;
sem_t executando;

//threads e suas tarefas a serem executadas
void *segundaThread (void *t) {

    printf("Seja bem-vindo!\n");
    //liberacao da thread pra sempre executar primeiro
    sem_post(&realizado);
    sem_post(&realizado);
    pthread_exit(NULL);
}

void *primeiraThread (void *t) {

    //bloquear a thread e aguardar a condicao
    sem_wait(&realizado);
    printf("Fique a vontade.\n");
    //desbloquear a thread
    sem_post(&executando);
    pthread_exit(NULL);
}

void *quartaThread (void *t) {

    sem_wait(&realizado);
    printf("Sente-se por favor.\n");
    sem_post(&executando);
    pthread_exit(NULL);
}

void *terceiraThread (void *t) {

    sem_wait(&executando);
    sem_wait(&executando);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}
//nomeacoes das thrads mantidas como do pdf, mas ordenadas na logica pedida

//criacao das threads, realizacao do join e aguardo do termino da execucao das thread
void criaThreads(){

    pthread_t tids[NUM_THREADS];
    
    pthread_create(&tids[0], NULL, primeiraThread, NULL);
    pthread_create(&tids[1], NULL, segundaThread, NULL);
    pthread_create(&tids[2], NULL, terceiraThread, NULL);
    pthread_create(&tids[3], NULL, quartaThread, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
      try {
        pthread_join(tids[i], NULL);
      } catch (InterruptedException e) {
        System.err.println(e);
        return;
      }
    }

}

int main(int argc, char *argv[]) {

    //inicilaizacao das condicoes do semaforo
    sem_inicio(&realizado, 0, 0);
    sem_incio(&executando, 0, 0);

    criaThreads();
    
    return 0;
}