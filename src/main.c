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
	pid_t filho[4];

	int entrada[100];
	int qtd = 0, i = 0, j = 0, k = 0, saida = 0, inteiro;
	char caracter;
	
	/* Definir flags de protecao e visibilidade de memoria */
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;

	int *contador; 
	contador = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);
	int *numero; 
	numero = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);
	int *libera; 
	libera = (int*) mmap(NULL, sizeof(int)*4, protection, visibility, 0, 0);

	for(i = 0; i < 4; i++){
		libera[i] = 0;
	}

	i = 0;
	
	while (saida != -1){
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
   	
	while(j < qtd){

		for(k = 0; k < 4; k++){
			if(libera[k] == 0){
				filho[k] = fork();
				libera[k] = 1;
				
				if(filho[k] == 0){
					(*numero) = primo(entrada[j]);	
					(*contador) = (*contador)+(*numero);

					libera[k] = 0;
					exit(0);
				}
				j++;
				break;
			}
		}
	}

	for (i = 0; i < 4; i++){
	    waitpid(filho[i], NULL, 0);
	}
		
	printf ("%d\n", (*contador)); 
	
	return 0;
	
}
