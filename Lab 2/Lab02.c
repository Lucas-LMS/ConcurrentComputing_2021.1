/* 
Lab 2 
Aluno: Lucas Moreno Silva
DRE: 119140949
Disciplina: Computação Concorrente 2021.1
Professora: Silvana Rosseto
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *matrix, *matrix2,*out;
int nthreads;

typedef struct{
    int id;
    int dim;
} threadsArgs;

void * function(void *arg){
    int i, j, k;
    threadsArgs *args = (threadsArgs*) arg;
    for(i = args->id; i<args->dim;i+=nthreads)
        for(j=0; j<args->dim; j++)
            for(k=0; k<args->dim; k++)
                out[i*(args->dim)+j] += matrix[i*(args->dim)+j]*matrix2[i*(args->dim)+j];
    pthread_exit(NULL);            
}


int main(int argc, char* argv[]) {
	int dim, i, j;
	pthread_t *tid;
	threadsArgs *args;
	double inicio, fim, delta;
	
	GET_TIME(inicio);
	
	
	if(argc<3) {
		printf("Digite: %s <dim da matriz> <número de threads>\n", argv[0]);
		return 1;
	}
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);
	if (nthreads > dim) nthreads = dim;
	
	
	matrix = (float *) malloc(sizeof(float) * dim * dim);
	if (matrix == NULL) {printf("ERRO -- malloc  (matriz)\n"); return 2;}
	matrix2 = (float *) malloc(sizeof(float) * dim * dim);
	if (matrix == NULL) {printf("ERRO -- malloc  (matriz2)\n"); return 2;}
	out = (float *) malloc(sizeof(float) * dim * dim);
	if (matrix == NULL) {printf("ERRO -- malloc  (saida)\n"); return 2;}
	
	
	for(i=0; i<dim; i++) {
		for(j=0; j<dim; j++){
			matrix[i*dim+j] = 2;  
			matrix2[i*dim+j] = 2;
			out[i*dim+j] = 0;
		}
	}
	
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo de Inicializacao: %lf\n", delta);
	
	
	GET_TIME(inicio)
	tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	if(tid==NULL) {puts("ERRO -- malloc"); return 2;}
	args = (threadsArgs*) malloc(sizeof(threadsArgs)*nthreads);
	if(args==NULL) {puts("ERRO -- malloc"); return 2;}
	
	
	for(i=0; i<nthreads; i++) {
		(args+i)->id = i;
		(args+i)->dim = dim;
		if(pthread_create(tid+i, NULL, function, (void*) (args+i))){
			puts("ERRO -- pthread_create"); return 3;
		}
	}
	

	for(i=0; i<nthreads; i++) {
		pthread_join(*(tid+i), NULL);
	}
	
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo da Multiplicacao: %lf\n", delta);
	

	/*//Corretude
	for(i=0; i<dim; i++) {
		for(j=0; j<dim; j++){
			if(out[i*dim+j] != matrix[i*dim+j]*matrix2[i*dim+j]*dim){
				printf("Resultado Errado! %f \n", out[i*dim+j]);
				return 4;
			}
		}
	}
	
	printf("All Clear!\n");*/
	
	
	
	
	
	
	GET_TIME(inicio);
	
	free(matrix);
	free(matrix2);
	free(out);
	free(args);
	free(tid);
	
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo de Finalizacao: %lf\n", delta);
	
	return 0;
}

