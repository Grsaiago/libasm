#include <stdio.h>

extern int asm_strlen(char *str);

int main(void) {
	int i = 0;

	i = asm_strlen("cinco");
	printf("o retorno foi %d\n", i);
	return (0);
}
