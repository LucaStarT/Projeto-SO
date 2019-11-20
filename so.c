#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#define LINHA 2
#define COLUNA 500
#define N 16

#define GET_MS(inicio, final) ((final.tv_sec * 1000000 + final.tv_usec) \
                                - (inicio.tv_sec * 1000000 + inicio.tv_usec)) //definicoes para a funcao 'gettimeofday'
        struct timeval inicio, fim;

int MatrizBubble[LINHA][COLUNA];
int *al;
int cont=2;
int z=2;
int memoria=1000; 

typedef struct{ //struct que passa valores para a thread
    int *inVet; //comeco
    int fimVet; //fim
}thread_arg, *ptr_thread_arg;

int troca(int a, ptr_thread_arg targ, int j){

    return a;
}

void *bubbleSort (void *arg){
	ptr_thread_arg targ = (ptr_thread_arg)arg;
	int i,j, x;
    for (i=z; i<=targ->fimVet; i++){
        x = targ->inVet[i];
        j = i-1;
        targ->inVet[1] = x;
        while (x < targ->inVet[j]){
            targ->inVet[j+1] = targ->inVet[j];
            j--;
        }
        targ->inVet[j+1] = x;
    }
}

void fun_ord_thread (int T){
    gettimeofday(&inicio, NULL); //inicia a contagem do tempo
    pthread_t thread[T];
    thread_arg arguments[T];
    for (int j=0; j<T; j++){
        arguments[j].inVet = al;
        int a=j+1;
        arguments[j].fimVet=(((cont-1)/T)*a) + (cont-1)%T;
        pthread_create(&thread[j], NULL, bubbleSort, &(arguments[j]));
        pthread_join(thread[j], NULL);
        z=arguments[j].fimVet;
    }
    gettimeofday(&fim, NULL); //encerra a contagem do tempo e mostra em ms
    printf("Tempo de execucao: %ld ms\n\n", GET_MS(inicio, fim));
}

int main(int arqc, char* arqv[]){

	int i=0,j=0,ordem=2,a,qtdeThr;
	pthread_t T[N];
	FILE *arqVet;
	FILE *arq2; //define arquivo a ser escrito     
    arq2=fopen(arqv[arqc-1],"w"); //cria o arquivo
    int numero;
    for(int i=2; i<arqc-1;i++) { //lê arquivos de entrada
        FILE *arq1; 
        arq1=fopen(arqv[i],"r");      
        while(!feof(arq1)) 
        { 
            fscanf(arq1,"%d",&numero); //le o arquivo e aloca na variavel numero
            fprintf(arq2,"%d ",numero); //escreve no arquivo .out
        }
        fclose(arq1);
    }
    fclose(arq2); //fecha o arquivo .out

	#define TAMANHO ordem
	
	//alocar vetor de 1k em 1k
    arqVet = fopen(arqv[arqc-1],"r");

    al = (int *) calloc(1000, sizeof(int));   //aloca 1000 espacos 

    while(!feof(arqVet)){                          //enquanto houver numeros no arquivo, ele aloca no vetor aloc
        fscanf(arqVet,"%d\n", &al[cont]);
        cont++;
        if(cont == memoria){                        //se a variavel 'cont' for igual a 'mem', aument-se mais 1000 espaços
            memoria = cont + 1000;
            al = realloc(al, memoria*sizeof(int));
        }
    }
    fclose(arqVet); //fecha arquivo

	//Imprime valores ja ordenados da Matriz
    FILE *arqFim;
    arqFim=fopen(arqv[arqc-1],"w");
    for(int i=2;i<cont;i++){ //escreve todos os numeros
        fprintf(arqFim,"%d ",al[i]);
    } 
    fclose(arqFim); //fecha o arquivo 
	free(al);
}
