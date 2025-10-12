#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int reverse_str(char msg[]);
void read_line(char msg[]);

int main(){
	
	return 0;
}

void les_1() {
	char msg[] = "something";
	int err = reverse_str(msg);
	if(err) {
		fprintf(stderr, "ERROR: reversing string\n");
		return;
	}
	printf("Reversed string is %s\n", msg);
}

int reverse_str(char msg[]){
	int len = (int) strlen(msg);
	char *cpy = calloc(len, sizeof(char));
	if(cpy == NULL){
		fprintf(stderr, "ERROR: allocating memory\n");
		return 1;
	}
	strcpy(cpy, msg);
	msg[0] = 'a';

	for(int i = 0; i<len; i++) {
		// msg[i] = *(cpy + (len -i-1));
		msg[i] = cpy[len-1-i];
	}

	free(cpy);
	return 0;
}
