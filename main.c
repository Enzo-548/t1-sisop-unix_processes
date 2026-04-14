#include <stdio.h>
#include <string.h>
int main(void) {
	char frase[256];

	printf("Digite uma frase para o Enzo: ");
	fflush(stdout);

	if (scanf(" %255[^\n]", frase) != 1) {
		return 1;
	}
	printf("%s\n", frase);
	return 0;
}