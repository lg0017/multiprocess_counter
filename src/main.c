/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
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
	pid_t filho[3];
	int entrada[100];
	int qtd = 0, i = 0, j = 0, k = 0, saida = 0, inteiro;
	char caracter;

	/* Definir flags de protecao e visibilidade de memoria */
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;

	/* Criar area de memoria compartilhada */
	int *contador; /* Variavel contadora visivel para todos os processos */
	contador = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);
	int *numero; /* Variavel contadora visivel para todos os processos */
	numero = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);

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
		filho[k] = fork();
		if (filho[k] == 0) {
			(*numero) = primo(entrada[j]);
			(*contador) = (*contador)+(*numero);
			exit(0);
		}

		/* Verifica se atingiu o limite de processos permitidos */
		if (k == 3){
			for (i = 0; i < k; i++){
				waitpid(filho[k], NULL, 0);
				k = 0 ;     
			}
		}

		k++;
		j++;
	}
	for (i = 0; i < qtd; i++){
	    waitpid(filho[i], NULL, 0);
	}

	printf ("%d\n", (*contador)); 

	return 0;

}
