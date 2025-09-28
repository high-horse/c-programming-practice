#include <stdio.h>

char get_temp_char(char msg[]);
float convertC2F(float c);
float convertF2C(float f);
float convertK2F(float k);
float convertF2K(float f);
float convertC2K(float c);
float convertK2C(float k);

int main(){
	printf("Temperature converter...\n");
	char from = get_temp_char("Please enter from Temperature System\n");
	char to = get_temp_char("Please enter to temperature system.\n");
	float original ;

	printf("Please enter the temperature\n");
	scanf("%f", &original);
	float final;
	if( from == 'C' && to == 'F'){
		final = convertC2F(original);
	} else if(from == 'F' && to == 'C'){
		final = convertF2C(original);
	} else if(from == 'C' && to == 'K'){
		final = convertC2K(original);
	} else if(from == 'K' && to == 'C') {
		final = convertK2C(original);
	} else if(from == 'f' && to == 'K'){
		final = convertF2K(original);
	} else if(from == 'K' && to == 'F'){
		final = convertK2F(original);
	}

	printf("the result is %.2f %c\n", final, to);

}

float convertC2F(float c){
	float result = c * 1.8 ;
	return result + 32.0;
}
float convertF2C(float f){
	float c = f - 32.0;
	return c / 1.8;
}

float convertF2K(float f){
	float op = f + 459.67;
	return op * (5.0/9.0);
}

float convertK2F(float k){
	float op = k * (9.0/5.0);
	return op - 459.67;
}

float convertC2K(float c){
	return c + 273.15;
}

float convertK2C(float k){
	return k - 273.15;
}

char get_temp_char(char msg[]){
   	int x =0;
    	while(msg[x]){
        	putchar(msg[x]);
		x++;
	}
	// putchar('\n');

	char option ;
	scanf("%c", &option);
	getchar();

	while(
		option != 'C' &&
		option != 'F' && 
		option != 'K' 
	) {
		printf("Invalid format. Please enter (F,C,K)\n");
		scanf("%c", &option);
		getchar();
	}

	return option;
}


