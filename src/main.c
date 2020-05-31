/* Data de submissao: 31/05/2020
 * Nome: Luiz Guilherme Silva Moreira
 * RA: 202391
 */

#include <stdio.h>
#include <stdlib.h> /* para utilizar exit */
#include <sys/types.h> /* para utilizar pid_t */
#include <sys/wait.h> /* parar utilizar waitpid */
#include <sys/mman.h> /* para utilizar mmap */
#include <unistd.h> /* para utilizar fork */

/* Funcao que verifica dado um parametro se o numero n e primo */
int primo(int n){
	int aux = 2;
	int verificacao = 1;
	
	if (n <= 1){
		verificacao = 0;
	}
	
    	while (verificacao == 1 && aux <= n / 2) {
		if (n % aux  == 0){
			verificacao = 0;
		}
		aux = aux + 1;
    	}
	
	return verificacao;
}
	
int main() {
	/* Numero maximo de processos */
	pid_t filho[4];

	/* Variaveis de auxilio geral */
	int entrada[100];
	int qtd = 0, i = 0, j = 0, k = 0, saida = 0, inteiro;
	char caracter;
	
	/* Definir flags de protecao e visibilidade de memoria */
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;

	/* Criar area de memoria compartilhada */
	int *contador; /* Variavel contadora visivel para todos os processos */
	contador = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);
	int *numero; /* Variavel verificadora de primos visivel para todos os processos */
	numero = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);
	int *libera; /* Variavel controladora de processos visivel para todos os processos */
	libera = (int*) mmap(NULL, sizeof(int)*4, protection, visibility, 0, 0);

	/* Preenchendo o vetor liberador de zeros */
	/* Legenda: 0 - processo livre 1 - processo ocupado */
	for(i = 0; i < 4; i++){
		libera[i] = 0;
	}

	i = 0;

	/* Tratamento da entrada */	
	while (saida != -1){
		/* Variavel caracter serve para ler espacos */
		scanf("%d%c", &inteiro, &caracter);
		if(caracter == '\n'){
			entrada[i] = inteiro;
			saida = -1;
			qtd++;
		}else{
			entrada[i] = inteiro;
			i++;
			qtd++;
		} 
	}
   	
	/* Inicio dos processos para verificacao de primos */  
	while(j < qtd){

		/* Numero maximo de processos ao mesmo tempo */
		for(k = 0; k < 4; k++){
			/* Processo livre */
			if(libera[k] == 0){
				filho[k] = fork();
				/* Processo ocupado */
				libera[k] = 1;
				
				/* Estamos no filho */	
				if(filho[k] == 0){
					/* Verificacao e contagem */
					(*numero) = primo(entrada[j]);	
					(*contador) = (*contador)+(*numero);
					/* Processo novamente livre */					
					libera[k] = 0;
					exit(0);
				}
				/* Anda no vetor de entradas */
				j++;
				break;
			}
		}
	}

	/* Aguarda todos os processos terminarem */
	for (i = 0; i < 4; i++){
	    waitpid(filho[i], NULL, 0);
	}
	
	/* Saida */	
	printf ("%d\n", (*contador)); 
	
	return 0;
	
}
