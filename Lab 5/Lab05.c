/* 
Lab 5
Aluno: Lucas Moreno Silva
DRE: 119140949
Disciplina: Computação Concorrente 2021.1
Professora: Silvana Rosseto
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//variaveis globais
int block; //n de threads bloqueadas
int N; //n de threads a serem criadas
int* vetor; //vetor dos inteiros
pthread_mutex_t mutex; //variavel para garantir a sincronia da "mutual exclusion"
pthread_cond_t cond; //variavel de controle da "condition"


//barreira da threads
void barreira(int nthreads, int threadId, int idBarreira){
    pthread_mutex_lock(&mutex);

    //avisa as threads em aguardo que a ultima thread chegou
    if(block == (nthreads -1)){
        printf("Thread %d liberando execucao das outras\n", threadId);

        //mensagem de status
        char* message = idBarreira == 0 ? "Finalizando as somas\n\n" : "Finalizando atualizacao dos elementos\n\n";
        printf("%s",message);

        //transmissao da condicao
        pthread_cond_broadcast(&cond);
        block = 0;

    }else{
        //caso a ultima thread ainda nao tenha chegado
        block++;

        //verifica se eh a ultima thread a chegar
        printf("Thread %d bloqueada na barreira\n", idBarreira);

        //mandar as threads aguardarem
        pthread_cond_wait(&cond,&mutex);
    }
    //liberacao da exclusao mutua
    pthread_mutex_unlock(&mutex);
}


//tarefa executada pelas threads
void somaDeElementos_atual(void *arg){
    long int id = (long int) arg; //id da thread
    int *soma; //variavel da soma de elementos

    //verificacao de nulidade
    soma = (int *) malloc(sizeof(int));
    if(soma == NULL){
        fprintf(stderr, "ERROR....malloc");
        exit(3);
    }
    *soma = 0;

    //apenas acompanhamento da execucao
    for(int i = 0; i < N; i++){
        printf("Iteracao %d: \nthread numero %ld iniciou execucao...\n",i,id);

        //adiciona os elementos ao vetor
        for(int j = 0; j < N; j++){
            *soma += vetor[j];
        }

        //primeira barreira para esperar as threads realizarem a soma
        barreira(N, id, 0);

        vetor[id] = rand() % 10;

        //segunda barreira para aguarda as threads se atualizarem
        barreira(N, id, 1);
    }

    pthread_exit((void *) soma);
}


//inicia o mutex, cond e a tid
void incilalizador(pthread_mutex_t *mutex, pthread_cond_t *cond, pthread_t **tid, int N){
    pthread_mutex_init(mutex,NULL);
    pthread_cond_init(cond,NULL);

    //verificacao de nulidade
    (*tid) = (pthread_t *) malloc(sizeof(pthread_t) * N);
    if((*tid) == NULL){
        printf("ERROR....malloc\n");
        exit(4);
    }
}

//criando as threads
void criarThreads(pthread_t *tid, int numthread){
    for(long int i = 0; i < N; i++){
        //verificacao da criacao correta das threads
        if(pthread_create(tid+i, NULL, somaDeElementos_atual, (void *) i)){
            printf("ERROR...pthread_create\n");
            exit(5);
        }
    }

}

void joinThreads(pthread_t *tid, int numthread, int **somaDeElementos){
    int *saida;

    for(int i = 0; i < N; i++){
        if(pthread_join(*(tid+i), (void **) &saida)){
            printf("ERROR....pthread_join\n");
            exit(6);
        }
        (somaDeElementos)[i] = *saida;
    }


}

int main(int argc, char *argv[]){
    pthread_t *tid; //id das threads
    int *somaDeElementos; //vetor com os valores da soma

    //validador de entrada
    if(argc < 2){
        printf("Entrada invalida!\n\nPor favor, digite: %s <n threads\n>", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);

    //alocacao do vetor de somas
    (*somaDeElementos) = (int *) malloc(sizeof(int) * N);
    if(*somaDeElementos == NULL){
        printf("ERROR....malloc");
        exit(2);

    }

    //alocacao e inicializacao do vetor de inteiros
    srand(time(NULL));

    (*vetor) = (int *) malloc(sizeof(int) * N);
    if(*vetor == NULL){
        printf("ERROR....malloc");
        exit(2);

    }

    for(int i = 0; i < N; i++){
        (vetor)[i] = rand() % 10;
    }

    //sequencia de execucao das threads
    incilalizador(&mutex, &cond, &tid, N);
    criarThreads(tid, N);
    joinThreads(tid, N, &somaDeElementos);

    //teste de corretude
    int r = somaDeElementos[0];

    printf("Resultados de soma recebidos das threads\n");
    for(int i = 0; i < N; i++){
        printf("Threads %d: %d \n", i, somaDeElementos[i]);
        if(somaDeElementos[i] != r){
            printf("\nValores das somas calculadas pelas threads %d e %d diferem\n",i, 0);
            exit(4);
        }
    }
    printf("Calculos realizados corretamente pelas threads\n");

    //desalocacao das variaveis
    pthread_mutex_destroy(mutex);
    pthread_cond_destroy(cond);

    free(vetor);
    free(somaDeElementos);
    free(tid);

    return 0;
}