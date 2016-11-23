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
	* Arquivo de destino é aberto com w+ para garantir 
	* que ele será criado vazio.
	*/
	FILE * newFile = fopen("compactado.txt", "w+");
	fclose(newFile);
	/**
	* Então é aberto novamente, em modo append 
	*/
	newFile = fopen("compactado.txt", "a");
	if (newFile == NULL) {
		printf("\n Erro ao criar arquivo de destino!\n");
		return;
	}
	/**
	* Percorre todo o arquivo original, caracter a caracter,
	* comparando o caracter anterior com o atual, identificando
	* assim, sequências que podem ser otimizadas.
	*/
	char now;
	char before;
	int count;
	
	if (!feof(originalFile)) {
		before = getc(originalFile);
		count = 1;
	}
	
	while(!feof(originalFile)) {
		now = getc(originalFile);
		/**
		* Se o caracter anterior for igual ao caracter atual,
		* faz parte de uma sequência
		*/
		if (now == before) {
			/**
			* A maior sequência representada será de 9 caracteres, evitando 
			* uma notação ambígua onde @1234@ poderia significar:
			* (12 vezes 3)4
			* 123 vezes 4
			*/
			if (count < 9 ){
				count++;
			} else {
				/**
				* Caso esta seja a décima ocorrência, registra a notação de 
				* 9 ocorrências e reinicia o contador em 1.
				*/
				fprintf(newFile, "@%d%c", count, now);	
				count = 1;
			}
		/**
		* se o caracter anterior for diferente do atual
		*/
		} else {
			/**
			* e countou-se apenas uma ocorrência do caracter anterior, ele é impresso
			* no arquivo de saída. O caracter atual passa a ser o anterior.
			*/
			if (count == 1) {
				fprintf(newFile, "%c", before);
			
			/** 
			* se a contagem do caracter anterior for maior que 1, então é impressa
			* a notação de contagem. O contador é reiniciado para 1 - a primeira
			* ocorrência do caracter atual - e o caracter atual passa a ser o anterior.
			*/
			} else {
				fprintf(newFile, "@%d%c", count, before);	
				count = 1;
			}
			before = now;
			
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
	* Arquivo de destino é aberto com w+ para garantir 
	* que ele será criado vazio.
	*/
	FILE * uncompressedFile = fopen("descompactado.txt", "w+");
	fclose(uncompressedFile);
	/**
	* Então é aberto novamente, em modo append 
	*/
	uncompressedFile = fopen("descompactado.txt", "a");
	if (uncompressedFile == NULL) {
		printf("\n Erro ao criar arquivo de destino!\n");
		return;
	}
	
	char ch;
	int count = 0;
	int n = 0;
	if (!feof(compressedFile)) {
		ch = getc(compressedFile);
	}
	while (!feof(compressedFile)) {
		if (ch == '@') {
			ch = getc(compressedFile);
			n = ch - '0';
			ch = getc(compressedFile);
			count = 0;
			while(count < n) {
				putc(ch, uncompressedFile);
				count++;
			}
		} else {
			putc(ch, uncompressedFile);
		}
		ch = getc(compressedFile);
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
