#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	
	FILE * f = fopen("lorem.txt", "r");
	FILE * cpyF = f;
	printf("Carregando...");
	if (f) {
		printf("feito!\n");
	}
	
	printf("\n %d \n\n", sizeof(f));
	while(!feof(cpyF)) {
		printf("%c", getc(cpyF));
	}
	
	
	return 0;
}
