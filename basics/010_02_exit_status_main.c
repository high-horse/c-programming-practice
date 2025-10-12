#include <stdio.h>
#include <stdlib.h>

int main(int args, char *argv[]){
	printf("hello world\n");
	if(args != 2){
		printf("Faliure\n");
		return EXIT_FAILURE;
	}
	printf("success \n");

	return EXIT_SUCCESS;
}

