/* 
Lab 3 
Aluno: Lucas Moreno Silva
DRE: 119140949
Disciplina: Computação Concorrente 2021.1
Professora: Silvana Rosseto
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long long int elem; //quantidade de elementos do vetor de entrada
int NTREADS; //numero de threads
float *vetor; //vetor de entrada

typedef struct
{
    float max; // maior valor do vetor
    float min; //menor valor do vetor
} vetorcheck;



//fluxo das threads
void * tarefa(void * arg){
    long long int id = (long long int) arg; //identidicador da thread
    long long int tamBloco = elem/NTREADS; //tamanho do bloco de cada thread
    long long int ini = id * tamBloco;  //elemento inicial do bloco da thread
    long long int fim; //elemento final(nao processado) do bloco da thread
    
    vetorcheck *concorrMaxMin;
    concorrMaxMin = malloc(sizeof(vetorcheck));
    

    if(id == NTREADS-1) fim = elem;
    else fim = ini+ tamBloco; //trata o resto se houver
    
    
    //busca o menor e maior valor do vetor no bloco da thread
    long long int k;
    for(k = ini; k < fim; k++){
        if(k == ini){
            concorrMaxMin->max = vetor[k];
            concorrMaxMin->min = vetor[k];
        }else{
            if(vetor[k] > concorrMaxMin->max)
                concorrMaxMin->max = vetor[k];
            if(vetor[k] > concorrMaxMin->min)
                concorrMaxMin->min = vetor[k];

        }
        
    }
        
    //retorna o resultado da busca
    pthread_exit((void *) concorrMaxMin);    
}

//iniciando o vetor de entrada com elementos aleatorios
float randominput(){
    return((float)rand()) / ((float)RAND_MAX);
}
    
float randomgap(float a, float b){
    return randominput() * (b-a) + a; 
}	


//fluxo principal
int main(int argc, char *argv[]) {
    float menorOrd, maiorOrd, maiorConc = 0;
    float menorConco = 0;
    double ini, fim; //tomada de tempo
    pthread_t *tid; //identificadores das threads no sistema
    vetorcheck *retorno; //valor de retorno das threads

    //recebe os parametros de entrada(elementos do vetor, numero de threads)
    if(argc < 3){
        fprintf(stderr, "Digite: %s <elementos do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    elem = atoll(argv[1]);
    NTREADS = atoi(argv[2]);
    //aloca o vetor de entrada
    vetor = (float*) malloc(sizeof(float)*elem);
    if(vetor == NULL){
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    //vetor de entrada
    srand(time(NULL));
        long long int p;
        for(p =0; p< elem; p++){
            vetor[p] = randomgap(0,10);
        }
    

       

    //verificacao sequencial dos elementos
    GET_TIME(ini);
    //funcao para buscar o maior e menor valor do vetor sequencialmente

    long long int l;
    for(l = 0; l < elem; l++ ){
        if(l==0){
            maiorOrd = vetor[l];
            menorOrd = vetor[l];
        }else{
            if(vetor[l] > maiorOrd)
                maiorOrd = vetor[l];
            if(vetor[l] < menorOrd)
                menorOrd = vetor[l];
        }
        
    }

    GET_TIME(fim);
    double seqt;
    seqt = fim - ini;   
    printf("\nTempo sequencial: %lf\n", seqt); 


    //concorrente
    GET_TIME(ini);
    tid = (pthread_t *) malloc(sizeof(pthread_t) * NTREADS);
    if(tid == NULL){
        fprintf(stderr, "ERRO-malloc\n");
        return 2;
    }

    //cria as threads
    long long int m;
    for(m = 0; m < NTREADS; m++){
        if(pthread_create(tid + m, NULL, tarefa, (void*) m)){
            fprintf(stderr, "ERRO-pthread_create\n");
            return 3;
        }
    }

    //aguardar o termino das threads
    long int n;
    for(n = 0; n < NTREADS; n++){
        if(pthread_join(*(tid+ n), (void**) &retorno)){
            fprintf(stderr, "ERRO-pthread_join\n");
            return 3;
        }
        //realizando a atualizacao dos valores de maximo e minimo

        if(n==0){
            maiorConc = retorno->max;
            menorConco = retorno->min;
        }else{
            if(retorno->max > maiorConc)
                maiorConc = retorno->max;
            if(retorno->min < menorConco)
                menorConco = retorno->min;
        } 
        free(retorno); 
        
    }
    

    GET_TIME(fim);
    double conct;
    conct = fim - ini;   
    printf("\nTempo concorrente: %lf\n", conct);

    printf("\nGanho de desempenho: %lf\n", seqt/conct);

    //realizando a verificacao da solucao
    /*printf("\nMaior valor seq: %f\n", maiorOrd);
    printf("Menor valor seq: %f\n", menorOrd);
    printf("\nMaior valor conc: %f\n", maiorConc);
    printf("Menor valor conc: %f\n", menorConco);*/


        

    //libera as areas de memoria alocada
    free(vetor);
    free(tid); 

    //end
    

    return 0;
}

