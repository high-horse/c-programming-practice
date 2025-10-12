#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])  {
	int res = 0;
	for(int i=0; i < argc; i++){
		char *endptr;
		int temp = strtol(argv[i], &endptr, 10);
		res = res + temp;
		if(*endptr != '\0'){
			printf("extra %s\n", endptr);
		}
		
	}
	printf("%d\n", res);
}
