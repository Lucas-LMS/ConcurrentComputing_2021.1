/*
  LAB 1
  
  Aluno: Lucas Moreno Silva 
  DRE: 119140949
  Disciplina: Computacao Concorrente
  Prof.: Silvana Rosseto
  
  Atividade 5
  
*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

//Tamanho do vetor

#define VECTSIZE 10000

//Quantidade de Threads que criaremos

#define NTHREADS 2

//Variaveis globais

int vetor[VECTSIZE];
int vetor_output[VECTSIZE];

typedef struct {
    int inicio, fim;
} thread_Args;

//Iniciando o vetor com valores de 1 a 10000
void inicializarVetor(){
	for(int i = 0; i < VECTSIZE; i++){
		vetor_output[i]= i + 1;
	}
}

//Funcao a ser realizado pelas threads, elevar ao quadrado
void* FunctionVetor(void* arg){
    thread_Args args = *(thread_Args*) arg;

    for(int i = args.inicio; i < args.fim; i++){
        vetor[i] = (i+1)*(i+1);
    }
	free(arg);
	
    pthread_exit(NULL);
}

//Verificando se a operacao de ao quadrado ocorreu corretamente
void verificarVetor(){
    int vetorCorreto = 1;
    for(int i = 0; i < VECTSIZE; i++){
        if(vetor[i] != vetor_output[i] * vetor_output[i]){
            vetorCorreto = 0;
            printf("Erro encontrado!\n");
        }
    }

    if(vetorCorreto){
        printf("Nenhum erro encontrado!\n");
    }

//Imprimindo o vetor
void printVetor(){
    for(int i = 0; i < VECTSIZE; i++){
        printf("%d\n", vetor[i]);
    }
}

//Funcao main

int main(){
	
  //Declaracao dos identificadores	
	
   pthread_t tid[NTHREADS];
   int ident[NTHREADS], i;
   
	
	inicializarVetor();
	
//Criando as duas threads pedidas	
	for(i=0; i < NTHREADS; i++) {
    ident[i] = i + 1;
    if (pthread_create(&tid[i], NULL, FunctionVetor, (void *)&ident[i]))
      printf("Erro na criacao da thread %d!\n", ident[i]);
  }
	
  // Espera as threads terminarem a execução
  for(i=0; i < NTHREADS; i++) {
    if (pthread_join(tid[i], NULL))
      printf("Erro ocorrido no join!\n");
  }
	
	//printVector();

    verificarVetor();

    pthread_exit(NULL);
	
	
}