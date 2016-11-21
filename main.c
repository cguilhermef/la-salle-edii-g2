#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compactarArquivo () {
	FILE * originalFile = fopen("teste.txt", "r");
	if (originalFile == NULL) {
		printf("\n Erro ao abrir o arquivo de origem!\n");
		return;
	}
	/** 
	* Arquivo de destino � aberto com w+ para garantir 
	* que ele ser� criado vazio.
	*/
	FILE * newFile = fopen("compactado.txt", "w+");
	fclose(newFile);
	/**
	* Ent�o � aberto novamente, em modo append 
	*/
	newFile = fopen("compactado.txt", "a");
	if (newFile == NULL) {
		printf("\n Erro ao criar arquivo de destino!\n");
		return;
	}
	/**
	* Percorre todo o arquivo original, caracter a caracter,
	* comparando o caracter anterior com o atual, identificando
	* assim, sequ�ncias que podem ser otimizadas.
	*/
	char now;
	char before = '@';
	int count = 0;
	while(!feof(originalFile)) {
		now = getc(originalFile);
		if (count == 0) {
			/**
			* se a contagem � zero, significa que o caracter anterior
			* j� foi resolvido e deve ser iniciado um novo ciclo
			*/
			before = now;
			count++;
		} else if (now == before) {
			/**
			* se o caracter atual � igual ao anterior, temos o in�cio
			* de uma sequ�ncia. Apenas � incrementado o contador
			*/
			count++;
		} else if (count == 1) {
			/** 
			* Caso o caracter atual seja diferente do atual,
			* e o caracter anterior ocorreu apenas uma vez,
			* n�o houve sequ�ncia. O caracter anterior � impresso
			* e o atual passa a ser o anterior.
			*/
			putc('@', newFile);
			putc(before, newFile);
			before = now;
		} else {
			/**
			* N�o caindo em nenhum dos IF anteriores, significa que 
			* h� uma sequ�ncia de caracter repetido, mas que o caracter
			* atual difere do anterior. A sequ�ncia ent�o � resumida e 
			* impressa, com o caracter anterior. O caracter atual passa
			* a ser o anterior.
			*/
			fprintf(newFile, "@%d%c", count, before);
			before = now;
			count = 1;
		}
	}
	fclose(originalFile);
	fclose(newFile);
	return;
}

void descompactarArquivo() {
	FILE * compressedFile = fopen("compactado.txt", "r");
	if (compressedFile == NULL) {
		printf("\n Erro ao abrir o arquivo de origem!\n");
		return;
	}
	
	/** 
	* Arquivo de destino � aberto com w+ para garantir 
	* que ele ser� criado vazio.
	*/
	FILE * uncompressedFile = fopen("descompactado.txt", "w+");
	fclose(uncompressedFile);
	/**
	* Ent�o � aberto novamente, em modo append 
	*/
	uncompressedFile = fopen("descompactado.txt", "a");
	if (uncompressedFile == NULL) {
		printf("\n Erro ao criar arquivo de destino!\n");
		return;
	}
	
	char buffer[100];
	memset(buffer, '\0', 100);
	char num[100];
	memset(num, '\0', 100);
	
	char ch;
	char now;
	int size = 0;
	int c = 0;
	int n = 0;
	
	while(!feof(compressedFile)) {
		now = getc(compressedFile);
		if (now == '@') {
			if (size == 0) {
				memset(buffer, '\0', 100);
				memset(num, '\0', 100);
			} else {
				if (size == 1) {
					putc(ch, uncompressedFile);
				} else {
					strncpy(num, buffer, size-1);
					n = atoi(num);
					ch = buffer[size-1];
					c = 0;
					while(c < n) {
						fprintf(uncompressedFile, "%c", ch);
						c++;
					}
				}
				memset(buffer, '\0', 100);
				size = 0;
			}
		} else {
			buffer[size] = now;
			size++;
		}
	}
	
	if (size == 1) {
		putc(ch, uncompressedFile);
	}
	if (size > 1){
		strncpy(num, buffer, size-1);
		n = atoi(num);
		ch = buffer[size-1];
		c = 0;
		while(c < n) {
			putc(ch, uncompressedFile);
			c++;
		}
	}
	
	fclose(compressedFile);
	fclose(uncompressedFile);
	return;
}

int main(int argc, char *argv[]) {
	
	int choice = -1;
	
	while(choice != 0) {
		printf("Escolha uma opcao:\n\n");
		printf("1 - Compactar \n");
		printf("2 - Descompactar \n");
		printf("0 - Sair\n\n> ");
		scanf("%d", &choice);
		fflush(stdin);
		
		switch(choice) {
			case 1: {
				printf("Compactando...");
				compactarArquivo();
				printf(" feito!\n");
				break;
			}
			case 2: {
				printf("Descompactando...\n");
				descompactarArquivo();
				break;
			}
			case 0: {
				printf("Saindo...\n");
				break;
			}
			default: {
				printf("Opcao invalida! \n");
				choice = -1;
				break;
			}
		}
	}
	
	return 0;
}
