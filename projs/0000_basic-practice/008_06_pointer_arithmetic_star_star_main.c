#include<stdio.h>

int main() {
	char *h = "hello";
	char *s = " ";
	char *w = "world";

	char *msg[] = {h, s, w};

	int len = sizeof(msg)/sizeof(msg[0]);
	for(int i = 0; i < len ; i++) {
		// printf("%s", msg[i]);

	}
	// printf("\n test complete\n");

	char **p = msg;
	printf("%s\n", *p);

	printf("%s \n", ++p);
}

